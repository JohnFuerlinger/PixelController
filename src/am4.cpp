#include "am4.hpp"

void am4_startup() {

}

void am4_loop() {
    leds.fill_solid(CRGB::DarkGreen);
    FastLED.show();
}