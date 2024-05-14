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
static uint8_t input_digits = 0; // Amount of digits in input number
static uint8_t pin_id = 0;
static char last_key = 0; // Last pressed key

void handle_locked(char key) {
    if (input_digits < 9 && key >= '0' && key <= '9') { // Maximum of 9 digits for pins
        input = input * 10 + (key - '0');
        input_digits += 1;
        return;
    }
    switch (key) {
        case '#': // # -> unlock vault
            if (input_digits == 0) {
                return;
            }
            vault::unlock(input);
            input = 0;
            input_digits = 0;
            break;
        case 'C': // C -> clear input
            input = 0;
            input_digits = 0;
            break;
        case 'D': // D -> delete input digit
            if (input_digits == 0) {
                return;
            }
            input /= 10;
            input_digits -= 1;
            break;
    }
}

void handle_unlocked(char key) {
    if (key >= '0' && key <= '9') { // Maximum of 3 digits for ids
        if (input_digits >= 3) {
            return;
        }
        uint32_t new_input = input * 10 + (key - '0');
        if (new_input > 255) { // Maximum id
            return;
        }
        input = new_input;
        input_digits += 1;
        return;
    }
    switch (key) {
        case '*': // * -> lock
            vault::set_mode(Mode::locked);
            input = 0;
            input_digits = 0;
            break;
        case '#':
        case 'A':
        case 'B': // # or A or B -> edit pin
            if (input_digits == 0) {
                return;
            }
            vault::set_mode(Mode::edit);
            pin_id = input;
            input = vault::load_pin(pin_id);
            input_digits = 9;
            break;
        case 'C': // C -> clear input
            input = 0;
            input_digits = 0;
            break;
        case 'D': // D -> delete input digit
            if (input_digits == 0) {
                return;
            }
            input /= 10;
            input_digits -= 1;
            break;
    }
}

void handle_edit(char key) {
    if (input_digits < 9 && key >= '0' && key <= '9') { // Maximum of 9 digits for pins
        input = input * 10 + (key - '0');
        input_digits += 1;
        return;
    }
    switch (key) {
        case '*': // * -> unlock
            vault::set_mode(Mode::unlocked);
            input = 0;
            input_digits = 0;
            break;
        case '#': // # -> save, unlock
            vault::set_mode(Mode::unlocked);
            vault::store_pin(pin_id, input);
            input = 0;
            input_digits = 0;
            break;
        case 'C': // C -> clear input
            input = 0;
            input_digits = 0;
            break;
        case 'D': // D -> delete input digit
            if (input_digits == 0) {
                return;
            }
            input /= 10;
            input_digits -= 1;
            break;
    }
}

void setup(void) {
    Serial.begin(9600);
    while (!Serial);
    display::setup(); // Setup display
    Serial.println(F("Display initialized"));
    vault::setup(); // Setup vault
    Serial.println(F("Vault initialized"));
    display::update();
}

void loop(void) {
    char key = keypad::get_key();
    // keypad::Event event;
    // keypad::read(&event);
    // keypad::Key key = event.first_down();
    // Anti repeat
    if (last_key == key) {
        return;
    }
    last_key = key;
    if (key == 0) {
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
