#include "vault.hh"

#include <SD.h>

#define PANIC(message)                      \
    Serial.println(F("[PANIC]: " message)); \
    for (;;);

namespace vault {
    const uint8_t CHIP_SELECT = 4;
    const char *const FILE_NAME PROGMEM = "vault.bin";
    const uint8_t ZERO_BUF[4] PROGMEM = {0};

    static Mode mode = Mode::locked;
    static uint32_t master_pin = -1;
    static Pins pins = { .count = 0 };

    void setup(void) {
        pinMode(CHIP_SELECT, OUTPUT);
        if (!SD.begin(CHIP_SELECT)) {
            PANIC("SD setup failed");
        }
    }

    void unlock(uint32_t master) {
        master_pin = master;
        if (!SD.exists(FILE_NAME)) {
            pins = { .count = 0 };
            mode = Mode::unlocked;
            Serial.println("No file present - clean unlock");
            return;
        }
        File vault_file = SD.open(FILE_NAME, FILE_READ);
        if (!vault_file) {
            PANIC("SD could not open vault file");
        }
        int count = vault_file.read();
        if (count == -1) {
            pins = { .count = 0 };
            mode = Mode::unlocked;
            Serial.println("Empty vault file - clean unlock");
            return;
        }
        pins.count = count;
        for (uint8_t i = 0; i < count; i++) {
            uint8_t id = vault_file.read();
            uint32_t pin;
            vault_file.read((uint8_t *)(&pin), 4);
            xor_pin(id, &pin);
            decapsulate_pin(&pin);
            pins.pins[i] = { .id = id, .pin = pin };
        }
        vault_file.close();
        mode = Mode::unlocked;
        Serial.println("Loaded vault file from disk");
    }

    void lock(void) {
        File vault_file = SD.open(FILE_NAME, O_READ | O_WRITE | O_CREAT);
        if (!vault_file) {
            PANIC("SD could not save vault file");
        }
        vault_file.write(pins.count);
        for (uint8_t i = 0; i < pins.count; i++) {
            Pin *pin = &pins.pins[i];
            vault_file.write(pin->id);
            uint32_t pinpin = pin->pin;
            encapsulate_pin(&pinpin);
            xor_pin(pin->id, &pinpin);
            vault_file.write((uint8_t *)(&pinpin), 4);
        }
        vault_file.flush();
        vault_file.close();
        mode = Mode::locked;
    }

    Mode get_mode(void) {
        return mode;
    }

    void set_mode(Mode new_mode) {
        mode = new_mode;
    }

    uint32_t load_pin(uint8_t id) {
        for (uint8_t i = 0; i < pins.count; i++) {
            Pin *pin = &pins.pins[i];
            if (pin->id == id) {
                return pin->pin;
            }
        }
        return 0;
    }

    void store_pin(uint8_t id, uint32_t new_pin) {
        for (uint8_t i = 0; i < pins.count; i++) {
            Pin *pin = &pins.pins[i];
            if (pin->id == id) {
                pin->pin = new_pin;
                return;
            }
        }
        pins.pins[pins.count] = { .id = id, .pin = new_pin };
        ++pins.count;
    }

    void encapsulate_pin(uint32_t *pin) {
        *pin |= 0b11 << 30; // todo: replace with random bits
    }
    
    void decapsulate_pin(uint32_t *pin) {
        *pin &= ~(0b11 << 30);
    }

    void xor_pin(uint8_t id, uint32_t *pin) {
        uint32_t xor_key = master_pin;
        for (uint8_t i = 0; i <= id; i++) {
            xorshift32(&xor_key);
        }
        *pin ^= xor_key;
    }

    void xorshift32(uint32_t *state) {
        uint32_t x = *state;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        *state = x;
    }
}
