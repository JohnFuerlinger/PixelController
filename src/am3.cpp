/* Random animation */

#include "am3.hpp"

void am3_startup() {
    /* Set all pixels off */
    std::srand(0);


    leds.fill_solid(CRGB::Black);
    FastLED.show();
}

#define DIFFERENTIATOR      30

void am3_loop() {
    static uint32_t cnt = 0;
    if (cnt % 25000 == 0) {
        int numleds = leds.size();
        for (int i = 0; i < numleds; i++) {
            uint8_t r = rand() % 255;
            uint8_t g = rand() % 255;
            uint8_t b = rand() % 255;
            if (r >= g && r >= b) {
                r = max(r, (r += DIFFERENTIATOR));
                g = min(g, (g -= DIFFERENTIATOR));
                b = min(b, (b -= DIFFERENTIATOR));
            } else if (g >= r && g >= b) {
                r = min(r, (r -= DIFFERENTIATOR));
                g = max(g, (g += DIFFERENTIATOR));
                b = min(b, (b -= DIFFERENTIATOR)); 
            } else {
                r = min(r, (r -= DIFFERENTIATOR));
                g = min(g, (g -= DIFFERENTIATOR));
                b = max(b, (b += DIFFERENTIATOR));
            }
            leds[i] = CRGB(r,g,b);
        }
        FastLED.show();
    }
    cnt++;
}