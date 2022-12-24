/* Random animation */

#pragma once

#include "main.hpp"
#include "FastLED.h"
#include <cstdlib>

extern CRGBArray<NUM_LEDS> leds;

void am3_startup();
void am3_loop();