/*
 * Created by gt on 1/2/22 - 6:34 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include "main.h"

void foo2(void) {
    PORTB = 0x80; //shifting 1 through 7 bits
    _delay_ms(500);
}