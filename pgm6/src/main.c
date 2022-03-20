/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include <util/delay.h>
#include "LCDmodule.h"
#include <avr/interrupt.h>

void PrintByte(char *OutputString,
               char *PromptString,
               char ByteVar);

ISR(TIMER1_COMPA_vect) {
    static char str[16];
    static unsigned char result;
    static unsigned char atten = 1;

    static int pind0_curr = 0;
    static int pind1_curr = 0;
    static int pind0_prev, pind1_prev;

    PORTD |= _BV(PORTD4);

    pind0_prev = pind0_curr;
    pind1_prev = pind1_curr;
    pind0_curr = PIND & _BV(PIND0);
    pind1_curr = PIND & _BV(PIND1);
    if (pind0_prev && !pind0_curr) {
        atten = atten <= 1 ? 11 : atten - 1;
    }
    if (pind1_prev && !pind1_curr) {
        atten = atten >= 11 ? 1 : atten + 1;
    }
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC)) {}
    result = ADCH >> (atten / 2);
    result = atten % 2 ? result : result + (result >> 1);
    PrintByte(str, "", result);
    LCD_MoveCursor(1, 1);
    LCD_WriteString(str);
    PORTD |= _BV(PORTD5);
    OCR0A = result;
    PORTD &= ~_BV(PORTD5);
    PORTD &= ~_BV(PORTD4);
}

// to obtain 1kSPS rate, main loop should last 1ms
int main(void) {
    DDRB = 0xFF;
    DDRC = 0xFC;
    DDRD = 0xFC;

    PRR &= ~_BV(PRADC);
    PRR &= ~_BV(PRTIM0);
    PRR &= ~_BV(PRTIM1);

    TCCR0A &= ~_BV(COM0A0);
    TCCR0A |= _BV(COM0A1);

    TCCR0A &= ~_BV(COM0B0);
    TCCR0A &= ~_BV(COM0B1);

    TCCR0A |= _BV(WGM00);
    TCCR0A |= _BV(WGM01);
    TCCR0B &= ~_BV(WGM02);

    TCCR0B |= _BV(CS00);
    TCCR0B &= ~_BV(CS01);
    TCCR0B &= ~_BV(CS02);

    TCCR0B &= ~_BV(FOC0A);
    TCCR0B &= ~_BV(FOC0B);

    TCCR1A &= ~_BV(WGM10);
    TCCR1A &= ~_BV(WGM11);
    TCCR1A &= ~_BV(COM1A0);
    TCCR1A &= ~_BV(COM1A1);
    TCCR1A &= ~_BV(COM1B0);
    TCCR1A &= ~_BV(COM1B1);

    TCCR1B |= _BV(WGM12);
    TCCR1B &= ~_BV(WGM13);

    TCCR1B &= ~_BV(CS10);
    TCCR1B |= _BV(CS11);
    TCCR1B &= ~_BV(CS12);

    TCCR1C &= ~_BV(FOC1A);
    TCCR1C &= ~_BV(FOC1B);

    TIMSK1 |= _BV(OCIE1A);

    OCR1A = 0x03E8;

    ADCSRA |= _BV(ADEN);
    ADCSRA &= ~_BV(ADPS0);
    ADCSRA |= _BV(ADPS1);
    ADCSRA |= _BV(ADPS2);

    ADMUX &= ~_BV(REFS0);
    ADMUX &= ~_BV(REFS1);
    ADMUX |= _BV(ADLAR);
    ADMUX |= _BV(MUX0);
    ADMUX &= ~_BV(MUX1);
    ADMUX &= ~_BV(MUX2);
    ADMUX &= ~_BV(MUX3);

    LCD_Init();
    sei();
    while (1) {}
    return 0;
}

