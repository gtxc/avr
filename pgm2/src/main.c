/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include <util/delay.h>

int main(void) {
    DDRB = 0xFF;
    DDRD = 0xFC;    // PORTD[0:1] -> input, PORTD[2:7] -> output
    PORTD = 0x03;   // turns on internal pull-ups for PIND1 and PIND2
    PORTB = 0x02;   // turn on LED1 as a starting point
    while (1) {
        if (!(PIND & _BV(PIND0))) {
            if (!(PIND & _BV(PIND1))) {
                PORTB = PORTB >> 1 | PORTB << 7;
            } else {
                __asm__ volatile ("NOP");
                PORTB = PORTB << 1 | PORTB >> 7;
            }
        }
    }
    return 0;
}