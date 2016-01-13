

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "tmr0.h"
// Routines for Timer 0


void Setup_Timer0(void){
    OPTION_REGbits.TMR0CS = 0;      // Fosc/4
    OPTION_REGbits.PSA = 0;         // Turn on Prescaler
    OPTION_REGbits.PS = 7;          // Prescaler == 256
    TMR0IF = 0;                     // enable global intterupts
    ei();
}

