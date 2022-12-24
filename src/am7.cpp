#include "am7.hpp"

void am7_startup() {

}

void am7_loop() {
    leds.fill_solid(CRGB::MintCream);
    FastLED.show();
}