/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include "LCDmodule.h"
#include <avr/interrupt.h>
#include <stdio.h>

#define LCDBufferSize 32

unsigned char LCDBuffer[LCDBufferSize];
unsigned char NByte = 0;

unsigned char atn = 1;
unsigned char LCDISREn = 1;

void EnQueueLCDBuffer(unsigned char ByteIn) {
    static unsigned char *pLCDIn = LCDBuffer;
    static unsigned char BCount = 0;
    if (NByte < LCDBufferSize) {
        *pLCDIn = ByteIn;
        ++BCount;
        if (BCount == LCDBufferSize) {
            pLCDIn = LCDBuffer;
            BCount = 0;
        } else {
            ++pLCDIn;
        }
        ++NByte;
    }
    return;
}

void DeQueueLCDBuffer(void) {
    static unsigned char *pLCDOut = LCDBuffer;
    static unsigned char BCount = LCDBufferSize;
    unsigned char ByteOut;

    if (NByte > 0) {
        ByteOut = *pLCDOut >> 4;
        if ((*pLCDOut & 0x80) == 0) {
            ByteOut |= 0x10;
        }
        _LCDport = ByteOut;
        _LCDport = 0x40 | ByteOut;
        _LCDport = ByteOut;
        ByteOut = (ByteOut & 0xF0) | (*pLCDOut & 0x0F);
        _LCDport = ByteOut;
        _LCDport = 0x40 | ByteOut;
        _LCDport = ByteOut;
        _LCDport = 0x00;
        --BCount;
        if (BCount == 0) {
            pLCDOut = LCDBuffer;
            BCount = LCDBufferSize;
        } else {
            ++pLCDOut;
        }
        --NByte;
    }
    return;
}

ISR(TIMER1_COMPA_vect) {
    PORTD |= _BV(PORTD4);
    static char *pText;
    static char LCDText[16];
    unsigned char result;
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC)) {}
    result = ADCH >> (atn / 2);
    result = atn % 2 ? result : result + (result >> 1);
    if (LCDISREn == 1) {
        PrintByte(LCDText, "", result);
        EnQueueLCDBuffer(0x80);
        pText = LCDText;
        while (*pText != 0x00) {
            EnQueueLCDBuffer(*pText);
            ++pText;
        }
    }
    if (NByte != 0) {
        TIMSK2 |= _BV(OCIE2A);
    }
    OCR0A = result;
    PORTD &= ~_BV(PORTD4);
}

ISR(TIMER2_COMPA_vect) {
    PORTD |= _BV(PORTD5);
    if (LCDISREn == 1) {
        DeQueueLCDBuffer();
        if (NByte == 0) {
            TIMSK2 &= ~_BV(OCIE2A);
        } else {
            TCNT2 = 0x0;
            TIFR2 |= _BV(OCF2A);
        }
    }
    PORTD &= ~_BV(PORTD5);
}

int main(void) {
    cli();
    DDRB = 0xFF;
    DDRC = 0xFC;
    DDRD = 0xFC;

    DIDR0 = 0x03;

    PRR &= ~_BV(PRADC);
    PRR &= ~_BV(PRTIM0);
    PRR &= ~_BV(PRTIM1);
    PRR &= ~_BV(PRTIM2);


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

    TIMSK0 &= ~_BV(OCIE0A);
    OCR0A = 0x00;


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


    TCCR2A &= ~_BV(COM2A0);
    TCCR2A &= ~_BV(COM2A1);
    TCCR2A &= ~_BV(COM2B0);
    TCCR2A &= ~_BV(COM2B1);
    TCCR2A &= ~_BV(WGM20);
    TCCR2A |= _BV(WGM21);

    TCCR2B &= ~_BV(WGM22);
    TCCR2B &= ~_BV(CS20);
    TCCR2B |= _BV(CS21);
    TCCR2B &= ~_BV(CS22);

    TIMSK2 |= _BV(OCIE2A);

    OCR2A = 0x0032;


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

    char *pText;
    char LCDText[16];

    LCD_Init();
    PrintByte(LCDText, "Atten=", atn);
    LCD_MoveCursor(2,1);
    LCD_WriteString(LCDText);

    int PIND0Cur = 0;
    int PIND1Cur = 0;
    int PIND0Pre, PIND1Pre;

    sei();

    while (1) {
        PIND0Pre = PIND0Cur;
        PIND1Pre = PIND1Cur;
        PIND0Cur = PIND & _BV(PIND0);
        PIND1Cur = PIND & _BV(PIND1);
        if (PIND0Pre && !PIND0Cur) {
            atn = atn <= 1 ? 11 : atn - 1;
        }
        if (PIND1Pre && !PIND1Cur) {
            atn = atn >= 11 ? 1 : atn + 1;
        }
        if (PIND0Pre != PIND0Cur || PIND1Pre != PIND1Cur) {
            LCDISREn = 0;
            PrintByte(LCDText, "Atten=", atn);
            EnQueueLCDBuffer(0xC0);
            pText = LCDText;
            while (*pText != 0x00) {
                EnQueueLCDBuffer(*pText);
                ++pText;
            }
            LCDISREn = 1;
        }
        if (NByte != 0) {
            TIMSK2 |= _BV(OCIE2A);
        }
    }
    return 0;
}

