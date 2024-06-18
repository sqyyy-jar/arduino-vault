# Arduino Vault

This project is a PIN-manager for an Arduino Mega 2560.

The UI is shown on an OLED display and PINS are stored in encrypted form on a MicroSD card.
Interacting with the UI is possible through a 4x4 keypad.

## Modes & Keys

- Common (all states have these)
  - You can clear the current input with `C`
  - You can remove the last digit from the input with `D`
- Locked
  - You are here in the beginning
  - You can enter a PIN
  - You can unlock the vault with `#`
- Unlocked
  - You can enter a PIN ID (in the range 0 to 99)
  - You can lock the vault with `*`
  - You can select the PIN with `#`, `A` or `B`
- Edit
  - You can view the selected PIN
  - You can edit the selected PIN
  - You can discard changes and go back to the unlocked state with `*`
  - You can save changes and go back to the unlocked state with `#`

## Pins (Mega 2560)

- MicroSD Card Adapter
  - MISO: 50
  - MOSI: 51
  - SCK: 52
  - CS: 4
- Keypad
  - Pins 22-29

## Sources

- Libraries
  - [SD](https://github.com/arduino-libraries/SD) (library for SD-card reader)
  - [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) (library for OLED display)
  - [Keypad](https://github.com/Chris--A/Keypad) (library for 4x4 membrane keypad)
- Pin numbers
  - [SD](https://github.com/arduino-libraries/SD/blob/master/docs/readme.md)
