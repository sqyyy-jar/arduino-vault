#ifndef _VAULT_HH
#define _VAULT_HH

#include <stdint.h>

namespace vault {
    enum class Mode {
        locked,
        unlocked,
        edit,
    };

    struct Pin {
        uint8_t id;
        uint32_t pin;
    };

    struct Pins {
        uint8_t count;
        Pin pins[100];
    };

    void setup(void);

    void unlock(uint32_t master);

    void lock(void);

    Mode get_mode();

    void set_mode(Mode new_mode);

    uint32_t load_pin(uint8_t id);

    void store_pin(uint8_t id, uint32_t pin);

    void encapsulate_pin(uint32_t *pin);
    
    void decapsulate_pin(uint32_t *pin);

    void xor_pin(uint8_t id, uint32_t *pin);

    void xorshift32(uint32_t *state);
}

#endif
