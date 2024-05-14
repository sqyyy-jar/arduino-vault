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
- Inspiration
  - [Funduino](https://funduino.de/nr-28-das-sd-karten-modul) (SD-card reader pin configuration)

## TODO

- Fix file io
  - File is bigger than 4 * 256 Bytes
  - File open flags might be incorrect
- Implement encryption
