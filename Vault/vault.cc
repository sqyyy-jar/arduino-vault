#include "vault.hh"

#include <SD.h>

namespace vault {
    static uint32_t master_pin = -1;

    void setup(void) {
        if (!SD.begin(CHIP_SELECT)) {
            Serial.println(F("[ERR]: SD setup failed"));
            for (;;);
        }
    }

    bool is_locked() {
        return master_pin == -1;
    }

    void unlock(uint32_t master) {
        master_pin = master;
    }

    uint32_t load_pin(uint8_t id) {
        File vault_file = SD.open("vault.bin", FILE_READ);
        if (!vault_file) {
            return -1;
        }
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
    }

    void store_pin(uint8_t id, uint32_t pin) {
        File vault_file = SD.open("vault.bin", FILE_WRITE);
        if (!vault_file) {
            return; // todo - error
        }
        // todo - implement
    }

    void clear_pin(uint8_t id) {
        File vault_file = SD.open("vault.bin", FILE_WRITE);
        if (!vault_file) {
            return; // todo - error
        }
        // todo - implement
    }
}
