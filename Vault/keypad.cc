#include "keypad.hh"

#include <Arduino.h>
#include <Keypad.h>

namespace keypad {
    const char KEYS[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'},
    };
    const uint8_t ROW_PINS[4] = {22, 23, 24, 25};
    const uint8_t COL_PINS[4] = {26, 27, 28, 29};
    static Keypad keypad(makeKeymap(KEYS), ROW_PINS, COL_PINS, 4, 4);

    char get_key() {
        return keypad.getKey();
    }
}
