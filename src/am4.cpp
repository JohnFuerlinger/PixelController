/* Christmas dancers */

#include "am4.hpp"
#include "betterprinting.hpp"

void am4_startup() {
    leds.fill_solid(CRGB::Black);
    FastLED.show();
}

void am4_loop() {
    static uint32_t cnt = 0;
    static bool flip = false;
    if (cnt % 20000 == 0) {
        int numleds = leds.size();
        for (int i = 0; i < numleds; i++) {
            if (flip) {
                /* Evens green, odds red */
                for (int i = 0; i < numleds; i++) {
                    if (i % 2 == 0) {
                        leds[i] = CRGB(0, 255, 0);
                    } else {
                        leds[i] = CRGB(255, 0, 0);
                    }
                }
                if (i == (numleds - 1)) { flip = false; }
            } else {
                /* Evens red, odds green */
                for (int i = 0; i < numleds; i++) {
                    if (i % 2 == 0) {
                        leds[i] = CRGB(255, 0, 0);
                    } else {
                        leds[i] = CRGB(0, 255, 0);
                    }
                }
                if (i == (numleds - 1)) { flip = true; }
            }
        }
        FastLED.show();
    }
    cnt++;
}
