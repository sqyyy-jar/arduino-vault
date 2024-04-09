//     Keyboard layout
//
// 1 2 3 A
// 4 5 6 B
// 7 8 9 C
// * 0 # D
//
//     Modes
// 
// Locked:
// - [0-9]: enter master pin
// - [#]:   unlock vault
// - [C]:   clear pin
// - [D]:   delete digit
//
// Unlocked:
// - [0-9]: enter pin id
// - [*]:   lock vault
// - [A]:   go to set pin mode
// - [B]:   go to load pin mode
// - [C]:   clear pin id
// - [D]:   delete pin id digit
// 
// Set pin:
// - [0-9]: enter pin
// - [*]:   go to unlocked mode
// - [#]:   submit pin
// - [C]:   clear pin
// - [D]:   delete digit
//
// Load pin:
// - [*]:   go to unlocked mode
// - [C-D]: remove pin

#include "display.hh"
#include "keypad.hh"
#include "vault.hh"

enum Mode {
    locked,
    unlocked,
    set_pin,
    load_pin,
};

static Mode mode = locked;
static uint32_t input = 0; // Input/output number
static uint32_t input_multiplier = 1; // Multiplier for input (= 10^input_digits)
static uint8_t input_digits = 0; // Amount of digits in input number
static keypad::Button last_button = keypad::none; // Last pressed button

void handle_locked(keypad::Button button) {
    char c = keypad::button_char(button);
    if (input_digits < 9 && c >= '0' && c <= '9') { // Maximum of 9 digits for pins
        input += input_multiplier * (c - '0');
        input_multiplier *= 10;
        input_digits += 1;
        return;
    }
    if (input_digits == 0) { // No input yet
        return;
    }
    switch (button) {
        case keypad::ch:
            vault::unlock(input);
            break;
        case keypad::cc:
            // todo - implement
            break;
        case keypad::cd:
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
    }
}

void handle_unlocked(keypad::Button button) {}

void handle_set_pin(keypad::Button button) {}

void handle_load_pin(keypad::Button button) {
    switch (button) {
        case keypad::cx:
            mode = unlocked;
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
        case keypad::cc:
        case keypad::cd:
            vault::clear_pin(0); // todo - provide pin id
            mode = unlocked;
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
    }
}

void setup(void) {
    Serial.begin(9600);
    display::setup(); // Setup display
    keypad::setup(); // Setup keypad
    vault::setup(); // Setup vault
}

void loop(void) {
    keypad::Event event;
    keypad::read(&event);
    keypad::Button button = event.first_down();
    // Anti repeat
    if (last_button == button) {
        return;
    }
    last_button = button;
    if (button == keypad::none) {
        return;
    }
    switch (mode) {
        case locked:
            handle_locked(button);
            break;
        case unlocked:
            handle_unlocked(button);
            break;
        case set_pin:
            handle_set_pin(button);
            break;
        case load_pin:
            handle_load_pin(button);
            break;
    }
    display::update();
}
