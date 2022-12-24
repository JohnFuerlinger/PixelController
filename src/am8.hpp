#pragma once

#include "main.hpp"
#include "FastLED.h"

extern CRGBArray<NUM_LEDS> leds;

void am8_startup();
void am8_loop();