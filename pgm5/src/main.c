/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include "LCDmodule.h"

void PrintByte(char *OutputString,
               char *PromptString,
               char ByteVar);

// to obtain 1kSPS rate, main loop should last 1ms
int main(void) {
    DDRB = 0xFF;
    DDRC = 0xFC;
    DDRD = 0xFC;    // PORTD[0:1] -> input, PORTD[2:7] -> output

    DIDR0 = 0x3F;
    PRR &= ~_BV(PRADC);
    // ADMUX : 0000 0000; AND
    ADMUX &= ~_BV(REFS0);
    ADMUX &= ~_BV(REFS1);
    ADMUX |= _BV(ADLAR);
    ADMUX |= _BV(MUX0);
    ADMUX &= ~_BV(MUX1);
    ADMUX &= ~_BV(MUX2);
    ADMUX &= ~_BV(MUX3);

    // ADCSRA : 0000 0000 OR
    ADCSRA |= _BV(ADEN);
    ADCSRA &= ~_BV(ADPS0);
//    ADCSRA &= ~_BV(ADPS1);
//    ADCSRA &= ~_BV(ADPS2);
    ADCSRA |= _BV(ADPS1);
    ADCSRA |= _BV(ADPS2);

    LCD_Init();
    PORTD |= _BV(PORTD2);
    char str[16];
    unsigned char result;
//    sprintf(str, "%i", ~_BV(REFS1));
    unsigned char atten = 1;
    unsigned char adch_tmp;
//    sprintf(str, "Atten=%3i", atten);
//    LCD_WriteString(str);
//    LCD_MoveCursor(1,7);

    int pind0_curr = PIND & _BV(PIND0);
    int pind1_curr = PIND & _BV(PIND1);
    int pind0_prev, pind1_prev;

    while (1) {

        pind0_prev = pind0_curr;
        pind1_prev = pind1_curr;
        pind0_curr = PIND & _BV(PIND0);
        pind1_curr = PIND & _BV(PIND1);
        if (pind0_prev && !pind0_curr) {
            --atten;
            if (atten < 1) atten = 11;
//            PORTD |= 0x20;
//            PORTD |= 0x10;
//            sprintf(str, "Atten=%3i", atten);
//            PrintByte(str, "", atten);
//            PORTD &= 0xEF;
//            PORTD &= 0xDF;

        }
        if (pind1_prev && !pind1_curr) {
            ++atten;
            if (atten > 11) atten = 1;
//            PORTD |= 0x20;
//            LCD_MoveCursor(1, 7);
//            PORTD |= 0x10;
//            sprintf(str, "Atten=%3i", atten);
//            PrintByte(str, "", atten);
//            PORTD &= 0xEF;
//            LCD_WriteString(str);
//            PORTD &= 0xDF;

        }
        if (PORTD | _BV(PIND2)) {
            PORTD &= ~_BV(PORTD3);
            ADCSRA |= _BV(ADSC);
            while (ADCSRA & _BV(ADSC)) {}
            result = ADCH;
//            _delay_us(50);
            PrintByte(str, "", result);
            LCD_MoveCursor(1, 1);
            LCD_WriteString(str);
//            _delay_us(50);
            PORTD &= ~_BV(PORTD2);
            PORTD |= _BV(PORTD3);
        }
        if (PORTD | _BV(PORTD3)) {
            PORTD &= ~_BV(PORTD2);
            ADCSRA |= _BV(ADSC);
            while (ADCSRA & _BV(ADSC)) {}
            adch_tmp = ADCH >> (atten / 2);
            PORTB = atten % 2 ? adch_tmp : adch_tmp + (adch_tmp >> 1);
//            _delay_us(50);
            PORTD &= ~_BV(PORTD3);
            PORTD |= _BV(PORTD2);
        }
        _delay_us(550);

//        _delay_us(990);
//        _delay_ms(1);
    }
    return 0;
}

