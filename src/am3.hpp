#pragma once

#include "main.hpp"
#include "FastLED.h"

extern CRGBArray<NUM_LEDS> leds;

void am3_startup();
void am3_loop();