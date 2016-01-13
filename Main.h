/* 
 * File:   Main.h
 * Author: marcus
 *
 * Created on October 12, 2012, 11:28 AM
 */

#ifndef MAIN_H
#define	MAIN_H

#define _XTAL_FREQ      4000000     // Oscillator frequency
#define _4MHz           0xEA        // Oscillator config value

#define A6_Pin          RA5
#define A6_TRIS         TRISA5
#define A6_Select       ANSA5
#define A5_Pin          RA4
#define A5_TRIS         TRISA4
#define A5_Select       ANSA4
#define A4_Pin          RA3
#define A4_TRIS         TRISA3
#define A4_Select       ANSA3
#define A1_Pin          RA0
#define A1_TRIS         TRISA0
#define A1_Select       ANSA0

unsigned char Key_Value,Hold_Flag,key,yearkey,datekey,monthkey;
unsigned char flag,monthflag,yearflag,dateflag;
unsigned char Alarm_Sec,Alarm_Min,Alarm_Hour;
unsigned char Alarm_12or24,Alarmflag,Value,AlarmON;
unsigned char almm,almh,alm10h,alm1h,alm10m,alm1m;
unsigned char magic,talent,int_,skip,count2;

void Setup_Timer0(void);
void Setup_Digital_IO(void);
void Setup_Oscillator(const unsigned char);
void Enable_Disable_Alarm(void);
void Change_Time_Format(void);
void Change_Backlight_Option(void);
void Set_Time_Date(void);
void Set_Alarm(void);
void Alarm_Status(void);
void Time_Status(void);
void Date_Status(void);
void First_State(void);
void Setup_Comparator(void);
void Numerical_Input(void);
void Alarm_Numerical_Input(void);
void Key_C(void);
void Alarm_flash_display(void);

#endif	/* MAIN_H */

