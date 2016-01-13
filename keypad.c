/*
 * File:   pic_keypad.c
 * Author: Marcus Whelan
 *
 * Created on September 13, 2012, 4:39 AM
 *
 * This library is for a 16 button keypad used in my micro2 class.
 * The keypad is connected to portB of the PIC. RB7:RB4 are inputs,
 * RB3:RB0 are outputs.
 *                       RB4  RB5  RB6  RB7
 *                        |    |    |    |
 * Keypad Layout:   RB0 - 1    2    3    A
 *                  RB1 - 4    5    6    B
 *                  RB2 - 7    8    9    C
 *                  RB3 - #    0    *    D
 */                    // E         F

#include <xc.h>
#include "keypad.h"

// Patterns for scanning the keypad
const unsigned char Key_Pattern[] =
{ 0b11010111,   // 0
  0b11101110,   // 1
  0b11011110,   // 2
  0b10111110,   // 3
  0b11101101,   // 4
  0b11011101,   // 5
  0b10111101,   // 6
  0b11101011,   // 7
  0b11011011,   // 8
  0b10111011,   // 9
  0b01111110,   // A
  0b01111101,   // B
  0b01111011,   // C
  0b01110111,   // D
  0b11100111,   // E
  0b10110111 }; // F


void Setup_Keypad(void)
{
    PORTB = 0;      // Clear port
    TRISB = 0xF0;   // RB7:RB4 input, RB3:RB0 output
    nWPUEN = 0;     // Enable pull-up resistors
    ANSELB = 0;     // Set all digital
    IOCBN = 0xF0;   // Interrupt on change RB7:RB4
    //IOCBP = 0x40;   // Interrupt on change RB6 for * on raising
    IOCBF = 0;      // Clear interrupt flags
}

// Scan routine to find key
unsigned char Scan_Keypad(void)
{
    unsigned char Key, Pattern;
    // Check patterns to find key
    for(Key = 0; Key != 16; Key++)
    {
        Pattern = Key_Pattern[Key];
        PORTB = Pattern;
        if(PORTB == Pattern) break;
    }
    PORTB = 0;
    IOCBF = 0; // Reset the intterupt on change flags
    return(Key);
}



