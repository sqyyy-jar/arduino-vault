#include "display.hh"
#include "vault.hh"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define PANIC(message)                      \
    Serial.println(F("[PANIC]: " message)); \
    for (;;);

namespace display {
    const uint32_t POWERS_OF_TEN[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    const uint8_t WIDTH = 128;
    const uint8_t HEIGHT = 64;
    const uint8_t SCREEN_ADDRESS = 0x3c;
    const char *const TITLE_LOCKED PROGMEM = "Locked";
    const char *const TITLE_UNLOCKED PROGMEM = "Unlocked";
    const char *const TITLE_EDIT PROGMEM = "Edit pin";

    static Adafruit_SSD1306 controller(WIDTH, HEIGHT);
    static char *title = TITLE_LOCKED;
    static uint32_t input = 0;
    static uint8_t input_digits = 0;

    void setup(void) {
        if (!controller.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            PANIC("SSD1306 setup failed");
        }
        controller.clearDisplay();
        controller.display();
    }

    void set_mode(vault::Mode mode) {
        switch (mode) {
            case vault::Mode::locked:
                title = TITLE_LOCKED;
                break;
            case vault::Mode::unlocked:
                title = TITLE_UNLOCKED;
                break;
            case vault::Mode::edit:
                title = TITLE_EDIT;
                break;
        }
    }

    void set_input(uint32_t new_input, uint8_t new_input_digits) {
        input = new_input;
        input_digits = new_input_digits;
    }

    void update(void) {
        controller.clearDisplay();
        controller.setTextSize(2);
        controller.setTextColor(SSD1306_WHITE);
        controller.setCursor(0, 0);
        controller.println(title);
        for (uint8_t digit = input_digits; digit > 0; digit--) {
            controller.print((char)(input / POWERS_OF_TEN[digit - 1] % 10 + '0'));
        }
        controller.display();
    }
}
