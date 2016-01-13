/* 
 * File:   clock.h
 * Author: marcus
 *
 * Created on October 10, 2012, 2:21 PM
 */

#ifndef CLOCK_H
#define	CLOCK_H

#define _31     31
#define Feb     28
#define FebL    29
#define _30     30

unsigned char h,m,s,ampm;
unsigned char _10h,_1h,_10m,_1m,_10s,_1s;
unsigned char Ah,Am,As,_10year,_1year,_10date,_1date;
unsigned char date,day,month,year,LP;
unsigned char a,c,d,e,f,g,i,j,k,r;
unsigned char Sec_Setup,OSCON_Setup,ALM0_Setup;
unsigned char Vbat_Setup,VBATEN_Setup;
unsigned char Exact,_24hourclock,_10month,_1month;

const char Month_Names[19][5];
const unsigned char _24_12[];
const unsigned char Names[];
const unsigned char Days_Month[];
const unsigned char D_H[];
const unsigned char PLP[];
const unsigned char NLP[];

void Read_ALM0(void);
void Read_Setup(void);
void Setup_Clock(void);
void Switch_to_24(void);
void _24hour_Clock(void);
void _12hour_Clock(void);
void _24hour_Date(void);
void _12hour_Date(void);
void Read_C(void);
void Month_Print(void);
void Days_of_Month(void);


#endif	/* CLOCK_H */

