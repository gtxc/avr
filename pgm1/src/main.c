/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <setjmp.h>

int main(void) {
//    jmp_buf buf;
//    setjmp(buf);
    DDRB |= 0xFF;
    for (;;) {
        PORTB = 0x0F;
//        _delay_us(0.25);
//        _delay_ms(500);
        PORTB = 0x80;
    }
//    longjmp(buf, 42);
//        _delay_ms(500);

    return 0;
}

