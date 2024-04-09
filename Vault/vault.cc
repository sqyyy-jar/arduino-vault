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
            vault_file.flush();
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
        // todo - handle errors
        // todo - implement encryption and shuffeling
        vault_file.seek(id * 4);
        uint32_t pin;
        vault_file.read((void *)(&pin), 4);
        return pin;
    }

    void store_pin(uint8_t id, uint32_t pin) {
        // todo - handle errors
        // todo - implement encryption and shuffeling
        vault_file.seek(id * 4);
        vault_file.write((char *)(&pin), 4);
        vault_file.flush();
    }
}
