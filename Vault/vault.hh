#ifndef _VAULT_HH
#define _VAULT_HH

#include <stdint.h>

namespace vault {
    enum class Mode {
        locked,
        unlocked,
        set_pin,
        load_pin,
    };

    void setup(void);

    void unlock(uint32_t master);

    Mode get_mode();

    void set_mode(Mode new_mode);

    uint32_t load_pin(uint8_t id);

    void store_pin(uint8_t id, uint32_t pin);

    void clear_pin(uint8_t id);
}

#endif
