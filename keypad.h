/*
 * File:   keypad.h
 * Author: marcus
 *
 * Created on October 5, 2012, 1:11 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

/*
 * File:   pic_keypad.h
 * Author: Marcus Whelan
 *
 * Created on September 11, 2012, 9:24 AM
 *
 * This library is for a 16 button keypad used in my micro2 class.
 * The keypad is connected to portB of the PIC. RB7:RB4 are inputs,
 * RB3:RB0 are outputs.
 *                       RB4  RB5  RB6  RB7
 *                        |    |    |    |
 * Keypad Layout:   RB0 - 1    2    3    A
 *                  RB1 - 4    5    6    B
 *                  RB2 - 7    8    9    C
 *                  RB3 - E    0    F    D
 */

#define _0_Key      0
#define _1_Key      1
#define _2_Key      2
#define _3_Key      3
#define _4_Key      4
#define _5_Key      5
#define _6_Key      6
#define _7_Key      7
#define _8_Key      8
#define _9_Key      9
#define _A_Key      0xA
#define _B_Key      0xB
#define _C_Key      0xC
#define _D_Key      0xD
#define _Star_Key   0xE
#define _Pound_Key  0xF

// Global variable
const unsigned char Key_Pattern[];

// Function Prototypes
void Setup_Keypad(void);
unsigned char Scan_Keypad(void);


#endif	/* KEYPAD_H */

