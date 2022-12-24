#include "am2.hpp"

void am2_startup() {
    
}

void am2_loop() {
    leds.fill_solid(CRGB::Blue);
    FastLED.show();
}