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

/* TODO: Need to re-write the above message, since this is going to be
 * used primarily for the magic ANSI escape chars
 */

#include "betterprinting.hpp"

/* CHA command */
void cursor_col(uint8_t col) {
    JPRINTF("\x1b[%uG", col);
}