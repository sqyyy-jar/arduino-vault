#ifndef _DISPLAY_HH
#define _DISPLAY_HH

#include "vault.hh"

#include <stdint.h>

namespace display {
    void setup(void);

    void set_mode(vault::Mode mode);

    void set_input(uint32_t new_input, uint8_t new_input_digits);

    void update(void);
}

#endif
