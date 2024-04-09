#include "display.hh"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

namespace display {
    static Adafruit_SSD1306 controller(WIDTH, HEIGHT);
    static char *title = "Locked";
    static uint32_t input = 0;
    static uint8_t input_digits = 0;

    void setup(void) {
        if (!controller.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            Serial.println(F("[ERR]: SSD1306 setup failed"));
            for (;;);
        }
        controller.clearDisplay();
        controller.display();
    }

    void show_input(uint32_t input, uint8_t digits) {
        controller.clearDisplay();
        controller.setTextSize(1);
        controller.setTextColor(SSD1306_WHITE);
        controller.setCursor(0, 0);
        controller.cp437(true); // todo: font - try removing
        for (uint8_t i = 0; i < digits; i++) {
            uint8_t digit = input % 10;
            controller.write(digit + '0');
            input /= 10;
        }
        controller.display();
    }

    void update(void) {
        controller.clearDisplay();
        controller.setTextSize(1);
        controller.setTextColor(SSD1306_WHITE);
        controller.setCursor(0, 0);
        controller.println(title);
        uint32_t _input = input;
        char c[2] = {0};
        for (uint8_t digit = 0; digit < input_digits; digit++) {
            c[0] = _input % 10 + '0';
            controller.print(c);
            _input /= 10;
        }
        controller.display();
    }
}
