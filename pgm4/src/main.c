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
// 173us 750us
int main(void) {
    DDRB = 0xFF;
    DDRD = 0xFC;    // PORTD[0:1] -> input, PORTD[2:7] -> output
//    PORTD = 0x03;   // turns on internal pull-ups for PIND1 and PIND2
    LCD_Init();
    LCD_Clear();
    char str[16];
    unsigned char atten = 1;
    sprintf(str, "Atten=%3i", atten);
    LCD_WriteString(str);
    LCD_MoveCursor(1,7);
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
            PORTD |= 0x20;
            LCD_MoveCursor(1, 7);
            PORTD |= 0x10;
//            sprintf(str, "Atten=%3i", atten);
            PrintByte(str, "", atten);
            PORTD &= 0xEF;
            LCD_WriteString(str);
            PORTD &= 0xDF;
        }
        if (pind1_prev && !pind1_curr) {
            ++atten;
            if (atten > 11) atten = 1;
            PORTD |= 0x20;
            LCD_MoveCursor(1, 7);
            PORTD |= 0x10;
//            sprintf(str, "Atten=%3i", atten);
            PrintByte(str, "", atten);
            PORTD &= 0xEF;
            LCD_WriteString(str);
            PORTD &= 0xDF;
        }
        _delay_us(500);
    }
//        PORTD |= 0x10; // begin measure while loop execution time
//        PORTD &= 0xEF; // end measure
    return 0;
}