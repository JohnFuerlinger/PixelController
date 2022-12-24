#pragma once

#include "main.hpp"
#include "FastLED.h"

extern CRGBArray<NUM_LEDS> leds;

void am5_startup();
void am5_loop();