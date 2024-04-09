#include "vault.hh"

#include <SD.h>

#define PANIC(message)                      \
    Serial.println(F("[PANIC]: " message)); \
    for (;;);

namespace vault {
    const uint8_t CHIP_SELECT = 10;
    const char *const FILE_NAME PROGMEM = "vault.bin";
    const uint8_t ZERO_BUF[4] PROGMEM = {0};

    static Mode mode = Mode::locked;
    static uint32_t master_pin = -1;
    static File vault_file;

    void setup(void) {
        if (!SD.begin(CHIP_SELECT)) {
            PANIC("SD setup failed");
        }
    }

    void unlock(uint32_t master) {
        bool exists = SD.exists(FILE_WRITE);
        vault_file = SD.open(FILE_NAME, FILE_WRITE);
        if (!vault_file) {
            PANIC("SD could not open vault file");
        }
        if (!exists) {
            for (uint16_t i = 0; i < 256; i++) {
                vault_file.write(ZERO_BUF, 4);
            }
        }
        mode = Mode::unlocked;
        master_pin = master;
    }

    Mode get_mode() {
        return mode;
    }

    void set_mode(Mode new_mode) {
        mode = new_mode;
    }

    uint32_t load_pin(uint8_t id) {
        uint32_t len = vault_file.size();
        uint32_t pin_offset = (uint32_t)id * 4;
        if (!vault_file.seek(pin_offset)) {
            vault_file.close();
            return -1;
        }
        uint32_t pin = 0;
        if (vault_file.read((void *)(&pin), 4) != 4) {
            vault_file.close();
            return -1;
        }
        return pin;
        // todo - implement
    }

    void store_pin(uint8_t id, uint32_t pin) {
        // todo - implement
    }

    void clear_pin(uint8_t id) {
        // todo - implement
    }
}
