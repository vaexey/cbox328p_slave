#pragma once

#include "CONF.h"

namespace timer
{

    void setup()
    {
        pinMode(PIN_O1, OUTPUT);
        pinMode(PIN_O2, OUTPUT);
        pinMode(PIN_O3, OUTPUT);

        // TIMER 1 8-bit / prescaler 1
        TCCR1A = _BV(COM1A1) | _BV(COM1B1) | _BV(WGM10);
        TCCR1B = _BV(WGM12) | _BV(CS10);

        OCR1A = 0;
        OCR1B = 0;

        // TIMER 2 prescaler 1 only A
        TCCR2A = _BV(COM2A1) | _BV(WGM21) | _BV(WGM20);
        TCCR2B = _BV(CS20);

        OCR2A = 0;
    }

    inline void set_unsafe(const unsigned char o1, const unsigned  char o2, const unsigned  char o3)
    {
        OCR1A = o1;
        OCR1B = o2;
        OCR2A = o3;
    }

    // Force timer off when PWM==0
    void set(const unsigned char o1, const unsigned  char o2, const unsigned  char o3)
    {
        OCR1A = o1;
        OCR1B = o2;
        OCR2A = o3;

        // TIMER 1 8-bit / prescaler 1
        TCCR1A = ((o1 != 0) << COM1A1) | ((o2 != 0) << COM1B1) | _BV(WGM10);
        // TCCR1B = _BV(WGM12) | _BV(CS10);

        // TIMER 2 prescaler 1 only A
        TCCR2A = ((o3 != 0) << COM2A1) | _BV(WGM21) | _BV(WGM20);
        // TCCR2B = _BV(CS20);
    }

    // Reset timer counting
    inline void reset()
    {
        TCNT1L = 0;
        TCNT2 = 0;
    }
}