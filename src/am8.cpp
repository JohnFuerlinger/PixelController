#include "am8.hpp"

void am8_startup() {

}

void am8_loop() {
    leds.fill_solid(CRGB::FireBrick);
    FastLED.show();
}