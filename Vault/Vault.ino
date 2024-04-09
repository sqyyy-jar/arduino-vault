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
// - [A-B]:   go to edit pin mode
// - [C]:   clear pin id
// - [D]:   delete pin id digit
// 
// Edit pin:
// - [0-9]: enter pin
// - [*]:   go to unlocked mode
// - [#]:   submit pin
// - [C]:   clear pin
// - [D]:   delete digit

#include "display.hh"
#include "keypad.hh"
#include "vault.hh"

using vault::Mode;

static uint32_t input = 0; // Input/output number
static uint32_t input_multiplier = 1; // Multiplier for input (= 10^input_digits)
static uint8_t input_digits = 0; // Amount of digits in input number
static uint8_t pin_id = 0;
static keypad::Key last_key = keypad::Key::none; // Last pressed key

void handle_locked(keypad::Key key) {
    char c = keypad::key_char(key);
    if (input_digits < 9 && c >= '0' && c <= '9') { // Maximum of 9 digits for pins
        input += input_multiplier * (c - '0');
        input_multiplier *= 10;
        input_digits += 1;
        return;
    }
    if (input_digits == 0) { // No input yet
        return;
    }
    switch (key) {
        case keypad::Key::ch: // # -> unlock vault
            vault::unlock(input);
            break;
        case keypad::Key::cc: // c -> clear input
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
        case keypad::Key::cd: // d -> delete input digit
            input_multiplier /= 10;
            input -= input / input_multiplier * input_multiplier;
            input_digits -= 1;
            break;
    }
}

void handle_unlocked(keypad::Key key) {
    char c = keypad::key_char(key);
    if (input_digits < 3 && c >= '0' && c <= '9') { // Maximum of 3 digits for ids
        uint32_t new_input = input + input_multiplier + (c - '0');
        if (new_input > 255) { // Maximum id
            return;
        }
        input = new_input;
        input_multiplier *= 10;
        input_digits += 1;
        return;
    }
    switch (key) {
        case keypad::Key::cx: // * -> lock
            vault::set_mode(Mode::locked);
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
        case keypad::Key::ca:
        case keypad::Key::cb: // a or b -> edit pin
            vault::set_mode(Mode::edit);
            pin_id = input;
            input = vault::load_pin(pin_id);
            input_multiplier = 1000000000;
            input_digits = 9;
            break;
        case keypad::Key::cc: // c -> clear input
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
        case keypad::Key::cd: // d -> delete input digit
            input_multiplier /= 10;
            input -= input / input_multiplier * input_multiplier;
            input_digits -= 1;
            break;
    }
}

void handle_edit(keypad::Key key) {
    char c = keypad::key_char(key);
    if (input_digits < 9 && c >= '0' && c <= '9') { // Maximum of 9 digits for pins
        input += input_multiplier * (c - '0');
        input_multiplier *= 10;
        input_digits += 1;
        return;
    }
    switch (key) {
        case keypad::Key::cx: // * -> unlock
            vault::set_mode(Mode::unlocked);
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
        case keypad::Key::ch: // # -> save, unlock
            vault::set_mode(Mode::unlocked);
            vault::store_pin(pin_id, input);
            break;
        case keypad::Key::cc: // c -> clear input
            input = 0;
            input_multiplier = 1;
            input_digits = 0;
            break;
        case keypad::Key::cd: // d -> delete input digit
            if (input_digits == 0) {
                return;
            }
            input_multiplier /= 10;
            input -= input / input_multiplier * input_multiplier;
            input_digits -= 1;
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
    keypad::Key key = event.first_down();
    // Anti repeat
    if (last_key == key) {
        return;
    }
    last_key = key;
    if (key == keypad::Key::none) {
        return;
    }
    switch (vault::get_mode()) {
        case Mode::locked:
            handle_locked(key);
            break;
        case Mode::unlocked:
            handle_unlocked(key);
            break;
        case Mode::edit:
            handle_edit(key);
            break;
    }
    display::set_mode(vault::get_mode());
    display::set_input(input, input_digits);
    display::update();
}
