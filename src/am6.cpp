#include "am6.hpp"

void am6_startup() {

}

void am6_loop() {
    leds.fill_solid(CRGB::Fuchsia);
    FastLED.show();
}