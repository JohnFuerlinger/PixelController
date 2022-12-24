#pragma once

#include "main.hpp"
#include "FastLED.h"

extern CRGBArray<NUM_LEDS> leds;

void am7_startup();
void am7_loop();