/*
 * Created by gt on 1/2/22 - 6:28 AM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include "main.h"

int main(void) {
    DDRB |= 0xFF;
    PORTB = 0x0F; //shifting 1 through 7 bits
    PORTB = 0x0F; //shifting 1 through 7 bits
    PORTB = 0x0F; //shifting 1 through 7 bits
    _delay_ms(500);
    foo();
    foo2();
    return 0;
}