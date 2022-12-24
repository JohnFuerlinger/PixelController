/* Color fading */

#include "am2.hpp"

void am2_startup() {
    /* Set all pixels off */
    leds.fill_solid(CRGB::Black);
    FastLED.show();
}

#define R_INC       1
#define G_INC       2
#define B_INC       3

void am2_loop() {
    static uint8_t r = 0;
    static uint8_t g = 63;
    static uint8_t b = 127;
    static uint64_t cnt = 0;

    static bool r_flip = true;
    static bool g_flip = true;
    static bool b_flip = true;

    if (cnt % 3000 == 0) {
        leds.fill_solid(CRGB(r, g, b));
        FastLED.show();

        if (r >= (255 - R_INC)) {
            r_flip = false;
        } else if (r <= 0 + R_INC) {
            r_flip = true;
        }

        if (g >= (255 - G_INC)) {
            g_flip = false;
        } else if (g <= 0 + G_INC) {
            g_flip = true;
        }

        if (b >= (255 - B_INC)) {
            b_flip = false;
        } else if (b <= 0 + B_INC) {
            b_flip = true;
        }

        if (r_flip) {
            r += R_INC;
        } else {
            r -= R_INC;
        }

        if (g_flip) {
            g += G_INC;
        } else {
            g -= G_INC;
        }

        if (b_flip) {
            b += B_INC;
        } else {
            b -= B_INC;
        }
    }
    cnt++;
}