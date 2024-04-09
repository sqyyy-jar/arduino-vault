#ifndef _KEYPAD_HH
#define _KEYPAD_HH

#include <stdint.h>

namespace keypad {
    enum Key {
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
    
        bool is_down(Key i);

        Key first_down();
    };

    void setup(void);

    char key_char(Key key);

    void read(Event *out);
}

#endif
