/*
 * File:   Main.c
 * Author: marcus
 *
 * Created on October 4, 2012, 8:15 PM
 */


#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "pic_lcd.h"
#include "keypad.h"
#include "clock.h"
#include "eeprom.h"
#include "Main.h"
#include "int.h"

#pragma config FOSC = INTOSC, WDTE = OFF, LVP = OFF



/*
 * For the Main loop or the While loop the routines Get_Analog,
 * Check_Keypad, and Display were used.
 */
int main(void)
{
    Setup_Oscillator(_4MHz);
    Setup_Digital_IO();
    Setup_Timer0();
    setup_i2c_master();
    setup_lcd();
    Setup_Keypad();
    Read_C();
    Read_Setup();
    if(Sec_Setup == 0){
        Write_Data_SRAM(Seconds,(s |= (1<<7)));
    }
    Flag = 0;
    Alarmflag = 0;
    backlight = 0;
    i = 0;
    Write_Data_SRAM(Seconds,0xD6);
    Write_Data_SRAM(Minutes,0x59);
    Write_Data_SRAM(Hours,0x23);
    Write_Data_SRAM(Year,0x12);
    Write_Data_SRAM(Month,0x02);
    Write_Data_SRAM(Date,0x29);
    Write_Data_SRAM(Control,0x10);
    Write_Data_SRAM(Alarm_day,(0xA0));
    First_State();
}

void Setup_Oscillator(const unsigned char Fosc)
{
    OSCCON = Fosc;
    return;
}

void First_State(void){
     while(1){
         Read_C();
         Move_Cursor(1,1);
         Time_Status();
         Move_Cursor(2,2);
         Date_Status();
         Move_Cursor(3,2);
         Change_Backlight_Option();
         Move_Cursor(4,2);
         Alarm_Status();


        if(IOCIF == 1){
            __delay_ms(1);              // Debounce button press
            Key_Value = Scan_Keypad();  // Get the key
        switch (Key_Value){
            case _A_Key:
                Enable_Disable_Alarm();
                break;
            case _B_Key:
                Change_Time_Format();
                break;
            case _C_Key:
                Flag = 1;
                backlight++;
                if(backlight == 4){
                    backlight = 1;
                }
                    TMR0IE = 1; // enable timer 1 interrupt
                break;
            case _Star_Key:             
                Set_Time_Date();
                break;
            case _Pound_Key:
                Set_Alarm();
                break;
            default: ;
            } 
        }
    }
}

void Enable_Disable_Alarm(void){
    Read_Setup();
    Write_Data_SRAM(Control,(c ^= (1<<4)));
    Alarmflag ^= (1<<0);
}

void Alarm_Status(void){
    static unsigned char MI10,MI1,HO10,HO1,r;
    if(Alarmflag & 1){
        Alarm_Min = Read_EEPROM(Alarmmin);
        Write_Data_SRAM(Alarm_minutes,Alarm_Min);
        Alarm_Hour = Read_EEPROM(Alarmhou);
        Write_Data_SRAM(Alarm_hours,Alarm_Hour);
        Alarm_12or24 = Read_EEPROM(Alarm_12_24);
        MI10 = (Alarm_Min >> 4);
        MI1 = (Alarm_Min & 0x0F);
        HO10 = (Alarm_Hour >> 4);
        HO1 = (Alarm_Hour & 0x0F);
        if(_24hourclock & 1){
            printf("Alarm: %d%d:%d%d   ",HO10,HO1,MI10,MI1);
        }
        else{
            r = _24_12[Alarm_Hour];
            if(Alarm_Hour > 0x12){
                printf("Alarm: %2d:%d%d PM",r,MI10,MI1);
            }
            else{
                printf("Alarm: %2d:%d%d AM",r,MI10,MI1);
            }
        }
    }
    else{     //Alarm: 23:43 PM
        printf("               ");
    }

}

void Change_Time_Format(void){
    i++;
}

void Time_Status(void){
    if(i & 1){
        _24hour_Clock();}
    else{
        _12hour_Clock();}
}

void Date_Status(void){
    if(i & 1){
        _24hour_Date();}
    else{
        _12hour_Date();}
}

void Change_Backlight_Option(void){
    if(Flag & 1){
        if(backlight == 1){
            printf("Backlight: ON  ");
            A5_Pin = 1;}
        if(backlight == 2){
            printf("Backlight: OFF ");
            A5_Pin = 0;}
        if(backlight == 3){
            printf("Backlight: AUTO");
            if(C1OUT = 0){
                A5_Pin = 1;}
            else{
                A5_Pin = 0;}
        }
    }
    else{     //Backlight: AUTO
        printf("               ");
        if(backlight == 3){
            if(C1OUT = 0){
                A5_Pin = 1;}
            else{
                A5_Pin = 0;}
        }
    }
}

void Set_Time_Date(void){
    static unsigned char r;
    lcd_clear();
    r = _24_12[h];
    Move_Cursor(1,1);
    printf("Enter a new time:");
    if(_24hourclock & 1){
        Move_Cursor(2,4);
        printf("%d%d:%d%d",_10h,_1h,_10m,_1m);
        lcd_command(0x0F);
        Move_Cursor(2,3);
        
    }
    else{
        Move_Cursor(2,4);
        if(h > 0x12){
            printf("%2d:%d%d PM",r,_10m,_1m);}
        else{
            printf("%2d:%d%d AM",r,_10m,_1m);}
        lcd_command(0x0F);
        Move_Cursor(2,3);
    }
    Numerical_Input();
}

void Numerical_Input(void)
{
    // AKA the DNA Strand

    static unsigned char key,m10,m1,h10,h1,hm,mm,r,flag;
    static unsigned char ampm,pmh,yearflag,monthflag,dateflag;
    static unsigned char yearkey,monthkey,datekey,y1,y2,_1m,_2m,d1,d2;
    static unsigned char yr,mh,de,lp,mcheck,x;
    key = 0;
    flag = 1;
    while(flag == 1)
    {
        while(key < 10)
        {

            if(IOCIF == 1)
            {
                __delay_ms(2);              // Debounce button press
                Key_Value = Scan_Keypad();  // Get the key
                key++;
                timekey = 1;
                Key_C();
                if(key == 2)
                {
                    h10 = value;
                    Move_Cursor(2,4);
                    printf("%d",value);
                }
                if(key == 4)
                {
                    h1 = value;
                    Move_Cursor(2,5);
                    printf("%d:",value);
                }
                if(key == 6)
                {
                    m10 = value;
                    Move_Cursor(2,7);
                    printf("%d",value);
                }
                if(key == 8)
                {
                    m1 = value;
                    Move_Cursor(2,8);
                    printf("%d",value);
                }
            }
            if(key == 8)
            {
                hm = (((h10 & 3) << 4) | h1);
                mm = ((m10 << 4) | m1);
                if(_24hourclock & 1)
                {
                    if((((h10 > 0x02) && (h1 > 0x03)) || ((m10 > 0x06)
                            || (m10 == 0x06))) || (h10 > 0x02))
                    {
                        Move_Cursor(2,4);
                        printf("%d%d:%d%d   ",h10,h1,m10,m1);
                        Move_Cursor(2,3);
                        key = 0;
                        flag = 1;
                    }
                    else
                    {
                        key = 11;
                        flag = 0;
                        Write_EEPROM(minu,mm);
                        Write_Data_SRAM(Minutes,mm);
                        Write_EEPROM(hou,hm);
                    }
                }
                else
                {
                    if(hm == 0)
                    {
                        Move_Cursor(2,4);
                        printf("%d%d:%d%d",h10,h1,m10,m1);
                        Move_Cursor(2,3);
                        key = 0;
                        flag = 1;
                    }
                    if((((h10 > 0x01) && (h1 > 0x02))  || ((m10 > 0x06)
                            || (m10 == 0x06))) || (h10 > 0x01))
                    {
                        Move_Cursor(2,4);
                        printf("%d%d:%d%d",h10,h1,m10,m1);
                        Move_Cursor(2,3);
                        key = 0;
                        flag = 1;
                    }
                    else
                    {
                        key = 11;
                        flag = 0;
                        Write_EEPROM(minu,mm);
                        Write_EEPROM(hou,hm);
                        Write_Data_SRAM(Minutes,mm);
                    }
                }
            }
        }
    }
    //if(key == 5)
    //{
        //key = 0;
        if(_24hourclock == 0)
        {
            Move_Cursor(3,1);
            printf("1 for AM / 2 for PM");
            ampm = 1;
            while(ampm == 1)
            {
                if(IOCIF == 1)
                {
                    _delay(1000);
                    Key_Value = Scan_Keypad();// Debounce button press
                    if(Key_Value == _Star_Key)
                    {
                        ampm = 1;
                    }
                    if(Key_Value == _1_Key)
                    {
                        Write_Data_SRAM(Hours,hm);
                        ampm = 0;
                    }
                    if(Key_Value == _2_Key)
                    {
                        x = D_H[(h1 + 0x02)];
                        pmh = (((h10 + 0x01) << 4) | x);
                        Write_EEPROM(hou,pmh);
                        Write_Data_SRAM(Hours,pmh);
                        ampm = 0;
                    }
                }
            }
        //}
    }
    __delay_ms(1000);
    lcd_clear();
    Move_Cursor(1,1);
    printf("Enter new year:");
    Move_Cursor(2,4);
    printf("20%d%d",_10year,_1year);
    Move_Cursor(2,6);
    yearkey = 0;
    yearflag = 1;
    while(yearflag == 1)
    {
        while(yearkey < 6)
        {
            if(IOCIF == 1)
            {
                _delay(1000);              // Debounce button press
                Key_Value = Scan_Keypad();  // Get the key
                yearkey++;
                Key_C();
                if(yearkey == 2)
                {
                    Move_Cursor(2,6);
                    y1 = value;
                    printf("%d",value);}
                if(yearkey == 4)
                {
                    Move_Cursor(2,7);
                    y2 = value;
                    printf("%d",value);
                }
            }
            if(yearkey == 4)
            {
                yr = ((y1 << 4) | y2);
                if(yr > 0x99)
                {
                    yearflag = 1;
                    yearkey = 0;
                    Move_Cursor(2,4);
                    printf("%d%d",y1,y2);
                    Move_Cursor(2,6);
                }
                else
                {
                    Write_EEPROM(yea,yr);
                    Write_Data_SRAM(Year,yr);
                    yearkey = 7;
                    yearflag = 0;
                }
            }
        }
    }
    __delay_ms(1000);
    lcd_clear();
    Move_Cursor(1,1);
    printf("Enter new month:");
    Move_Cursor(2,4);
    printf("%d%d",_10month,_1month);
    Move_Cursor(2,3);
    monthkey = 0;
    monthflag = 1;
    while(monthflag == 1)
    {
        while(monthkey < 6)
        {
            if(IOCIF == 1)
            {
                _delay(1000);              // Debounce button press
                Key_Value = Scan_Keypad();  // Get the key
                monthkey++;
                Key_C();
                if(monthkey == 2)
                {
                    Move_Cursor(2,4);
                    _1m = value;
                    printf("%d",value);
                }
                if(monthkey == 4)
                {
                    Move_Cursor(2,5);
                    _2m = value;
                    printf("%d",value);
                }
            }
            if(monthkey == 4)
            {
                mh = ((_1m << 4) | _2m);
                if(mh == 0)
                {
                    monthflag = 1;
                    monthkey = 0;
                    Move_Cursor(2,4);
                    printf("%d%d",_1m,_2m);
                    Move_Cursor(2,3);
                }
                else
                {
                    if(mh > 0x12)
                    {
                        monthflag = 1;
                        monthkey = 0;
                        Move_Cursor(2,4);
                        printf("%d%d",_1m,_2m);
                        Move_Cursor(2,3);
                    }
                    else
                    {
                        Write_EEPROM(mon,mh);
                        Write_Data_SRAM(Month,mh);
                        monthkey = 7;
                        monthflag = 0;
                    }
                }
            }
        }
    }
    __delay_ms(1000);
    lcd_clear();
    Move_Cursor(1,1);
    printf("Enter new date:");
    Move_Cursor(2,4);
    printf("%d",Days_Month[date]);
    Move_Cursor(2,3);
    mcheck = Read_Data_SRAM(Month);
    lp = (mcheck & 0x20);
    datekey = 0;
    dateflag = 1;
    while(dateflag == 1)
    {
        while(datekey < 6)
        {
             if(IOCIF == 1)
             {
                _delay(1000);              // Debounce button press
                Key_Value = Scan_Keypad();  // Get the key
                datekey++;
                Key_C();
                if(datekey == 2)
                {
                    Move_Cursor(2,4);
                    d1 = value;
                    printf("%d",value);}
                if(datekey == 4)
                {
                    Move_Cursor(2,5);
                    d2 = value;
                    printf("%d",value);
                }
            }
            if(datekey == 4)
            {
                de = ((d1 << 4) | d2);
                if(de == 0)
                {
                    dateflag = 1;
                    datekey = 0;
                    Move_Cursor(2,4);
                    printf("%d%d",d1,d2);
                    Move_Cursor(2,3);
                }
                else
                {
                    if(lp)
                    {
                        x = PLP[mh];
                        if(de > x)
                        {
                            dateflag = 1;
                            datekey = 0;
                            Move_Cursor(2,4);
                            printf("%d%d",d1,d2);
                            Move_Cursor(2,3);
                        }
                        else
                        {
                            Write_EEPROM(dat,de);
                            Write_Data_SRAM(Date,de);
                            datekey = 7;
                            dateflag = 0;
                        }
                    }
                    else
                    {
                        x = NLP[mh];
                        if(de > x)
                        {
                            dateflag = 1;
                            datekey = 0;
                            Move_Cursor(2,4);
                            printf("%d%d",d1,d2);
                            Move_Cursor(2,3);
                        }
                        else
                        {
                            Write_EEPROM(dat,de);
                            Write_Data_SRAM(Date,de);
                            datekey = 7;
                            dateflag = 0;
                        }
                    }
                }
            }
        }
    }
    __delay_ms(1000);
    lcd_clear();
}


void Key_C(void){
    switch (Key_Value){
        case _0_Key:
            value = 0x00;
            break;
        case _1_Key:
            value = 0x01;
            break;
        case _2_Key:
            value = 0x02;
            break;
        case _3_Key:
            value = 0x03;
            break;
        case _4_Key:
            value = 0x04;
            break;
        case _5_Key:
            value = 0x05;
            break;
        case _6_Key:
            value = 0x06;
            break;
        case _7_Key:
            value = 0x07;
            break;
        case _8_Key:
            value = 0x08;
            break;
        case _9_Key:
            value = 0x09;
            break;
        case _A_Key:
            Key_Value = 0;
            break;
        case _B_Key:
            timekey = 0;
            break;
        case _C_Key:
            timekey = 0;
            break;
        case _D_Key:
            timekey = 0;
            break;
        case _Pound_Key:
            timekey = 0;
            break;
        case _Star_Key:
            timekey = 0;
            break;
            default: ;
        }
}

void Set_Alarm(void){
    
}

void Setup_Digital_IO(void){
    A1_TRIS = 1;            //Pin A0 is a input.
    A4_TRIS = 1;            //Pin A3 is a input.
    A5_TRIS = 0;            //Pin A4 is an output.
    A5_Select = 0;          //Pin A4 is Digital.
    A1_Select = 1;          //Pin A0 is Analog.
    A4_Select = 1;          //Pin A3 is Analog.
    A4_Pin = 1;
    A5_Pin = 0;             //off to start
    A1_Pin = 1;
}

void Setup_Timer0(void){
    OPTION_REGbits.TMR0CS = 0;      // Fosc/4
    OPTION_REGbits.PSA = 0;         // Turn on Prescaler
    OPTION_REGbits.PS = 7;          // Prescaler == 256
    TMR0IF = 0;                     // enable global intterupts
    ei();
}

void Setup_Comparator(void){
    //CM1CON1 = 0b00100000;
    //CM1CON0 = 0b10000000;
    //FVRCON = 0b1100010;
}