#include <Arduino.h>
#include "betterprinting.hpp"
#include "repl.h"
#include "FastLED.h"

#include "am1.hpp"
#include "am2.hpp"
#include "am3.hpp"
#include "am4.hpp"
#include "am5.hpp"
#include "am6.hpp"
#include "am7.hpp"
#include "am8.hpp"


#define LED_TYPE   WS2812
#define COLOR_ORDER   GRB
#define DATA_PIN        4
#define VOLTS          5
#define MAX_MA       3000

CRGBArray<NUM_LEDS> leds;

uint8_t switchpos = 0;

void setup() {
    Serial.begin(115200);
    SPUTS("\n--------------------------------------------------------------------------------\nMerry Christmas Daniel and Matthew!\n\n");

    /* Rotary switch pins */
    pinMode(1, INPUT_PULLDOWN);
    pinMode(2, INPUT_PULLDOWN);
    pinMode(42, INPUT_PULLDOWN);
    pinMode(41, INPUT_PULLDOWN);
    pinMode(40, INPUT_PULLDOWN);
    pinMode(39, INPUT_PULLDOWN);
    pinMode(38, INPUT_PULLDOWN);
    pinMode(37, INPUT_PULLDOWN);

    FastLED.setMaxPowerInVoltsAndMilliamps(VOLTS, MAX_MA);
    FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
}

void loop() {
    repl_loop();

    if (digitalRead(1)) {
        if (switchpos != 1) {
            am1_startup();
            switchpos = 1;
        }
        am1_loop();
    } else if (digitalRead(2)) {
        if (switchpos != 2) {
            am1_startup();
            switchpos = 2;
        }        
        am2_loop();
    } else if (digitalRead(42)) {
        if (switchpos != 3) {
            am1_startup();
            switchpos = 3;
        }        
        am3_loop();
    } else if (digitalRead(41)) {
        if (switchpos != 4) {
            am1_startup();
            switchpos = 4;
        }        
        am4_loop();
    } else if (digitalRead(40)) {
        if (switchpos != 5) {
            am1_startup();
            switchpos = 5;
        }        
        am5_loop();
    } else if (digitalRead(39)) {
        if (switchpos != 6) {
            am1_startup();
            switchpos = 6;
        }        
        am6_loop();
    } else if (digitalRead(38)) {
        if (switchpos != 7) {
            am1_startup();
            switchpos = 7;
        }        
        am7_loop();
    } else if (digitalRead(37)) {
        if (switchpos != 8) {
            am1_startup();
            switchpos = 8;
        }        
        am8_loop();
    } else {
        SPUTS("Rotary switch broken.\n");
    }
}
