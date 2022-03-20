/*
 * Created by gt on 1/2/22 - 9:26 PM.
 * Copyright (c) 2022 GTXC. All rights reserved.
 */

#include <avr/io.h>
#include "LCDmodule.h"
#include <avr/interrupt.h>
#include <stdio.h>

#define LCDBufferSize 32
#define USARTBufferSize 32

static unsigned char USARTBuffer[USARTBufferSize];
static unsigned char NUSARTByte = 0;
static char USARTText[16];
static char *pUSARTText;

static unsigned char LCDBuffer[LCDBufferSize];
static unsigned char NLCDByte = 0;
static char LCDText[16];
static char *pLCDText;

static unsigned char atn = 1;

void EnQueueLCDBuffer(unsigned char ByteIn) {
    static unsigned char *pLCDIn = LCDBuffer;
    static unsigned char BCount = 0;
    if (NLCDByte < LCDBufferSize) {
        *pLCDIn = ByteIn;
        ++BCount;
        if (BCount == LCDBufferSize) {
            pLCDIn = LCDBuffer;
            BCount = 0;
        } else {
            ++pLCDIn;
        }
        ++NLCDByte;
        TIMSK2 |= 0x02;
    }
    return;
}

void DeQueueLCDBuffer(void) {
    static unsigned char *pLCDOut = LCDBuffer;
    static unsigned char BCount = LCDBufferSize;
    unsigned char ByteOut;

    if (NLCDByte > 0) {
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
        --NLCDByte;
    } else {
        TIMSK2 &= 0x00;
    }
    return;
}

void EnQueueUSARTBuffer(unsigned char ByteIn) {
    static unsigned char *pUSARTIn = USARTBuffer;
    static unsigned char BCount = 0;
    if (NUSARTByte < USARTBufferSize) {
        *pUSARTIn = ByteIn;
        ++BCount;
        if (BCount == USARTBufferSize) {
            pUSARTIn = USARTBuffer;
            BCount = 0;
        } else {
            ++pUSARTIn;
        }
        ++NUSARTByte;
        UCSR0B |= 0xBB;
    }
    return;
}

void DeQueueUSARTBuffer(void) {
    static unsigned char *pUSARTOut = USARTBuffer;
    static unsigned char BCount = USARTBufferSize;

    if (NUSARTByte > 0) {
        UDR0 = *pUSARTOut;
        --BCount;
        if (BCount == 0) {
            pUSARTOut = USARTBuffer;
            BCount = USARTBufferSize;
        } else {
            ++pUSARTOut;
        }
        --NUSARTByte;
    } else {
        UCSR0B=0x93;//Set RX, Clear UDCE
    }
    return;
}

ISR(TIMER1_COMPA_vect) {
    static unsigned char sampleCount = 0;
    static unsigned char ADCOut;
    ADCSRA |= _BV(ADSC);
    while (ADCSRA & _BV(ADSC)) {}
    ++sampleCount;
    ADCOut = ADCH >> (atn / 2);
    ADCOut = atn % 2 ? ADCOut : ADCOut + (ADCOut >> 1);
    OCR0A = ADCOut;
        PrintByte(LCDText, "", ADCOut);
        EnQueueLCDBuffer(0x80);
        pLCDText = LCDText;
        while (*pLCDText != 0x00) {
            EnQueueLCDBuffer(*pLCDText);
            ++pLCDText;
        }
//    if (NLCDByte != 0) {
//        TIMSK2 |= _BV(OCIE2A);
//    }
    if (sampleCount == 10) {
        PrintByte(USARTText, "ADC = ", ADCOut);
        pUSARTText = USARTText;
        EnQueueUSARTBuffer(0x0D);
        while (*pUSARTText != 0x00) {
            EnQueueUSARTBuffer(*pUSARTText);
            ++pUSARTText;
        }
        sampleCount = 0;
    }
}

ISR(TIMER2_COMPA_vect) {
        DeQueueLCDBuffer();
//        if (NLCDByte == 0) {
//            TIMSK2 &= ~_BV(OCIE2A);
//        } else {
//            TCNT2 = 0x0;
//            TIFR2 |= _BV(OCF2A);
//        }
}

ISR(USART_RX_vect) {
    static unsigned char charIn;

    while((UCSR0A &(1<<RXC0)) == 0) {};
    charIn = UDR0;
    charIn &= 0xDF;
    if (charIn == 'D') {
        atn = atn <= 1 ? 11 : atn - 1;
    }
    if (charIn == 'U') {
        atn = atn >= 11 ? 1 : atn + 1;
    }
//    if (charIn == 'D' || charIn == 'U') {
        TIMSK2 &= 0x00; //Timer-1-2 off
        TIMSK1 &= 0x00;
        PrintByte(LCDText, "Atten=", atn);
        PrintByte(USARTText, "Atten=", atn);
        EnQueueUSARTBuffer(0x0D);
        EnQueueLCDBuffer(0xC0);
        pLCDText = LCDText;
        pUSARTText = USARTText;
        while (*pLCDText != 0x00) {
            EnQueueLCDBuffer(*pLCDText);
            ++pLCDText;
        }
        while (*pUSARTText != 0x00) {
            EnQueueUSARTBuffer(*pUSARTText);
            ++pUSARTText;
        }
        TIMSK2 |= 0x02;
        TIMSK1 |= 0x02;
//    }
//    if (NLCDByte != 0) {
//        TIMSK2 |= _BV(OCIE2A);
//    }
}

ISR(USART_UDRE_vect) {
    while( ( UCSR0A & ( 1 << UDRE0 ) ) == 0 ){};
    DeQueueUSARTBuffer();
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

    OCR1A = 0x2710;


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


    UBRR0L = 0x33;
    UBRR0H = 0x00;

    UCSR0A |= _BV(RXC0);
    UCSR0A |= _BV(UDRE0);

    UCSR0B &= ~_BV(UCSZ02);
    UCSR0B |= _BV(TXEN0);
    UCSR0B |= _BV(RXEN0);
    UCSR0B |= _BV(UDRIE0);
    UCSR0B |= _BV(TXCIE0);
    UCSR0B |= _BV(RXCIE0);

    UCSR0C |= _BV(UCSZ00);
    UCSR0C |= _BV(UCSZ01);
    UCSR0C  &= ~_BV(UMSEL00);
    UCSR0C  &= ~_BV(UMSEL01);
    UCSR0C  &= ~_BV(UPM00);
    UCSR0C  &= ~_BV(UPM01);
    UCSR0C  &= ~_BV(USBS0);

    LCD_Init();
    PrintByte(LCDText, "Atten=", atn);
    LCD_MoveCursor(2,1);
    LCD_WriteString(LCDText);

    sei();

    while (1) {}
    return 0;
}

