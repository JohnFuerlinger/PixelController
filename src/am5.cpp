#include "am5.hpp"

void am5_startup() {

}

void am5_loop() {
    leds.fill_solid(CRGB::DarkSalmon);
    FastLED.show();
}