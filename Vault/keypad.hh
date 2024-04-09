#ifndef _KEYPAD_HH
#define _KEYPAD_HH

#include <stdint.h>

namespace keypad {
    // todo: set pins
    const uint8_t ROW_PINS[4] = {0, 0, 0, 0};
    const uint8_t COL_PINS[4] = {0, 0, 0, 0};

    enum Button {
        c1,
        c2,
        c3,
        ca,

        c4,
        c5,
        c6,
        cb,

        c7,
        c8,
        c9,
        cc,

        cx, // *
        c0,
        ch, // #
        cd,

        none = 0xff
    };

    struct Event {
        uint16_t down;
    
        bool is_down(Button i);

        Button first_down();
    };

    void setup(void);

    char button_char(Button button);

    void read(Event *out);
}

#endif
