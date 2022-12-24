#include "am3.hpp"

void am3_startup() {

}

void am3_loop() {
    leds.fill_solid(CRGB::Beige);
    FastLED.show();
}