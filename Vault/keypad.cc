#include "keypad.hh"

#include <Arduino.h>

namespace keypad {
    // todo - set pins
    const uint8_t ROW_PINS[4] = {0, 0, 0, 0};
    const uint8_t COL_PINS[4] = {0, 0, 0, 0};

    bool Event::is_down(Key key) {
        return (this->down >> key) & 1 == 1;
    }

    Key Event::first_down() {
        for (uint8_t i = 0; i < 16; i++) {
            if ((this->down >> i) & 1) {
                return i;
            }
        }
        return Key::none;
    }

    void setup(void) {
        // Setup rows
        for (uint8_t i = 0; i < 4; i++)  {
            uint8_t row = ROW_PINS[i];
            pinMode(row, INPUT_PULLUP);
        }
        // Setup cols
        for (uint8_t i = 0; i < 4; i++)  {
            uint8_t col = COL_PINS[i];
            pinMode(col, OUTPUT);
            digitalWrite(col, HIGH);
        }
    }

    char key_char(Key key) {
        switch (key) {
            case c1: return '1';
            case c2: return '2';
            case c3: return '3';
            case ca: return 'a';

            case c4: return '4';
            case c5: return '5';
            case c6: return '6';
            case cb: return 'b';

            case c7: return '7';
            case c8: return '8';
            case c9: return '9';
            case cc: return 'c';

            case cx: return '*';
            case c0: return '0';
            case ch: return '#';
            case cd: return 'd';
        }
    }

    void read(Event *out) {
        out->down = 0;
        for (uint8_t col = 0; col < 4; col++) {
            for (uint8_t row = 0; row < 4; row++) {
                digitalWrite(col, LOW);
                delayMicroseconds(5);
                if (!digitalRead(row)) {
                    out->down |= 1 >> (row * 4 + col);
                }
                digitalWrite(col, HIGH);
                delayMicroseconds(50);
            }
        }
    }
}
