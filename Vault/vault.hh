#ifndef _VAULT_HH
#define _VAULT_HH

#include <stdint.h>

namespace vault {
    const uint8_t CHIP_SELECT = 10;

    void setup(void);

    bool is_locked();

    void unlock(uint32_t master);

    uint32_t load_pin(uint8_t id);

    void store_pin(uint8_t id, uint32_t pin);

    void clear_pin(uint8_t id);
}

#endif
