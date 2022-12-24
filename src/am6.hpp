#pragma once

#include "main.hpp"
#include "FastLED.h"

extern CRGBArray<NUM_LEDS> leds;

void am6_startup();
void am6_loop();