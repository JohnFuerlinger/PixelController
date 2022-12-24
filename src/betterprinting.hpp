/***********************************************************************
 * betterprinting.h
 *
 * Copyright (c) 2022, John Fuerlinger. All Rights Reserved.
 * Created Aug 2022
 *
 * --desc--
 *
 * This file adds layer on top of the typical Arduino print statements
 * for serial and Telnet to be more "C-like". SPUTS and TPUTS can be
 * used for basic non-formatted prints, while JPRINTF and TPRINTF
 * can be used for formatted text accoring to the sprintf format
 * specifiers. "B" statements print to both serial and telnet.
 ***********************************************************************/

/* TODO: Random color light toggles on the on-board LED! */

#pragma once

#include <Arduino.h>
#include <stdarg.h>
#include <string.h>

/* Over serial */
#define CHAROUTS(c) do {                                                \
    Serial.print(c);                                                    \
    if (c == '\n') { Serial.print('\r'); }                              \
} while(0)

#define SPUTS(txt) do {                                                 \
    for (uint16_t i = 0; i < strlen(txt); i++) { CHAROUTS(txt[i]); }    \
} while(0)

#define JPRINTF(txt, ...) do {                                          \
    char* _pbuf = (char *)malloc((strlen(txt)+100) * sizeof(char));     \
    sprintf(_pbuf, txt, __VA_ARGS__);                                   \
    SPUTS(_pbuf);                                                       \
    free(_pbuf);                                                        \
} while(0)

void cursor_col(uint8_t col);