#ifndef _DISPLAY_HH
#define _DISPLAY_HH

#include <stdint.h>

namespace display {
    const uint8_t WIDTH = 128;
    const uint8_t HEIGHT = 64;
    const uint8_t SCREEN_ADDRESS = 0x3c;

    void setup(void);

    void show_title(char *title);

    void show_input(uint32_t input, uint8_t digits);

    void update(void);
}

#endif
