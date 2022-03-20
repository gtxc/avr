/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include <util/delay.h>

// 125ns * 8 * 1000 = 1ms
// ldi  -> 1 cycle  : 125 ns
// ldi  -> 1 cycle  : 125 ns
// nop  -> 1 cycle  : 125 ns
// sbiw -> 2 cycles : 250 ns
// brne -> 1/2 cycle: 62.5 ns
// ret  -> 4 cycles : 500 ns
// 437.5 x + 750 = 1 000 000
// x = 2284
void delay_gt(void) {
    for (int i = 0; i < 1590; ++i)
        __asm__ volatile ("NOP");
}

// in   -> 1
// mov  -> 1
// andi -> 1
// andi -> 1
// sbiw -> 2 if
// brne -> 1/2 if
// or   -> 1 if
// breq -> 1/2 if
// cpse -> 1/2/3 if
// rjmp -> 2 if
// sbiw -> 2 --cnt

int main(void) {
    DDRB = 0xFF;
    DDRD = 0xFC;    // PORTD[0:1] -> input, PORTD[2:7] -> output
    PORTD = 0x03;   // turns on internal pull-ups for PIND1 and PIND2
    PORTB = 0x02;   // turn on LED1 as a starting point
    int cnt250ms = 250;
    int pind0_curr = PIND & _BV(PIND0);
    int pind0_prev;
    while (1) {
        PORTD |= 0x10; // begin measure while loop execution time
        pind0_prev = pind0_curr;
        pind0_curr = PIND & _BV(PIND0);
        if (!cnt250ms) {
            if (!(PIND & _BV(PIND1))) {
                PORTB = PORTB >> 1 | PORTB << 7;
            } else {
                PORTB = PORTB << 1 | PORTB >> 7;
            }
            cnt250ms = 250;
        }
        if (pind0_prev && !pind0_curr) {
            if (!(PIND & _BV(PIND1))) {
                PORTB = PORTB >> 1 | PORTB << 7;
            } else {
                PORTB = PORTB << 1 | PORTB >> 7;
            }
            cnt250ms = 250;
        }
        --cnt250ms;
        delay_gt();
        PORTD &= 0xEF; // end measure
    }
    return 0;
}
