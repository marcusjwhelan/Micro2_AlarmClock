
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "Main.h"
#include "keypad.h"
#include "tmr0.h"
#include "int.h"
#include "pic_lcd.h"

//The function prototype needs the keyword "interrupt"
//The function accepts and returns nothing (void)



void interrupt Backlight_Int(void){
    static unsigned char count;
    if(TMR0IE && TMR0IF){
        TMR0IF = 0; // clear flag
        //5 sec = 1us*256*256*76 = about 4.98 seconds
        if(int_ == 2){
            count = 0;
            Flag = 0;
            count2++;

            if(count2 == 31){
                TMR0IE = 0;
                IOCBF = 0;
            }
        }
        if(int_ == 1){
            count++;
            count2 = 0;
        if(count == 76){
            Flag = 0;
            count = 0; // reset count
            TMR0IE = 0; // turn off interrupt
        }
        if(count < 76){
            if(_C_Key ==  Key_Value){
                count = 0;
            }
            if((Key_Value == 1) | (Key_Value == 2) | (Key_Value == 3) |
            (Key_Value == 4) | (Key_Value == 5) | (Key_Value == 6) |
            (Key_Value == 7) | (Key_Value == 8) | (Key_Value == 9) |
            (Key_Value == 0xA) | (Key_Value == 0xB) | (Key_Value == 0xD) |
            (Key_Value == 0xE) | (Key_Value == 0xF) | (Key_Value == 0)){
                TMR0IE = 0;         // turn off interrupt
                count = 0;
                ei();               // enable global interrupts
                Flag = 0;
            }
        }
        }
    }
    if(C1IE && C1IF){
        C1IF = 0;
        if(C1OUT == 1){
            A5_Pin = 1;

        }
        else{
            A5_Pin = 0;
        }
    }
}

