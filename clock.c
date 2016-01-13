
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "i2c.h"
#include "pic_lcd.h"
#include "keypad.h"
#include "clock.h"
#include "Main.h"
#include "eeprom.h"


void Setup_Clock(void){
    s = Read_Data_SRAM(Seconds);
    Write_Data_SRAM(Seconds,(s & 0xFF));
    Switch_to_24();
    f = Read_Data_SRAM(OSCON_Day);
    Write_Data_SRAM(OSCON_Day,(f & 0x08 ));
    g = Read_Data_SRAM(Date);
    Write_Data_SRAM(Date,(g & 0x3F));
    j = Read_Data_SRAM(Month);
    Write_Data_SRAM(Month,(j & 0x1F));
    e = Read_Data_SRAM(Year);
    Write_Data_SRAM(Year,(e & 0xFF));
    Write_Data_SRAM(OSCON_Day,0x38);
}

void Switch_to_24(void){
    static unsigned char h;
    h = (Read_Data_SRAM(Hours) & 0x7F);
    Write_Data_SRAM(Hours,( h &= ~(1<<6)));     // sets 12/24 bit
}

void Read_C(void){
    s = (Read_Data_SRAM(Seconds) & 0x7F);
    _10s = (s >> 4);
    _1s = (s & 0x0F);
    m = (Read_Data_SRAM(Minutes) & 0x7F);
    _10m = (m >> 4);
    _1m = (m & 0x0F);
    h = (Read_Data_SRAM(Hours) & 0x3F);
    _10h = (h >> 4);
    _1h = (h & 0x0F);
    day = (Read_Data_SRAM(OSCON_Day) & 0x38);
    date = (Read_Data_SRAM(Date) & 0x3F);
    _10date = (date >> 4);
    _1date = (date & 0x0F);
    month = (Read_Data_SRAM(Month) & 0x1F);
    _10month = (month >> 4);
    _1month = (month & 0x0F);
    year = Read_Data_SRAM(Year);
    _10year = (year >> 4);
    _1year = (year & 0x0F);
}

void Read_Setup(void){
    Sec_Setup = ((Read_Data_SRAM(Seconds) & 0x80) >> 7);
    day = Read_Data_SRAM(OSCON_Day);
    OSCON_Setup = ((day & 0x20) >> 5);
    Vbat_Setup = ((day & 0x10) >> 4);
    VBATEN_Setup = ((day & 0x08) >> 3);
    c = Read_Data_SRAM(Control);
    ALM0_Setup = ((Read_Data_SRAM(Control) & 0x10) >> 4);
}

void _24hour_Clock(void){
    _24hourclock = 1;
    if(s & 1){
        printf("  %d%d:%d%d   ",_10h,_1h,_10m,_1m);}
    else{
        printf("  %d%d %d%d   ",_10h,_1h,_10m,_1m);}
}

void _12hour_Clock(void){
    _24hourclock = 0;
    r = _24_12[h];
    if(s & 1){
        printf("  %2d:%d%d ",r,_10m,_1m);}
    else{
        printf("  %2d %d%d ",r,_10m,_1m);}
    if(h > 0x11){
        printf("PM");}
    else{
        printf("AM");}
}

void _24hour_Date(void){
    printf("%d %s 20%d%d   ",Days_Month[date],Month_Names[month],
            _10year,_1year);
}

void _12hour_Date(void){
    printf("%s %d, 20%d%d ",Month_Names[month],Days_Month[date],
            _10year,_1year);
}

// to get months.
const char Month_Names[19][5] = {
    {""},{"Jan."},{"Feb."},{"Mar."},
    {"Apr."},{"May "},{"Jun."},
    {"Jul."},{"Aug."},{"Sep."},
    {""},{""},{""},{""},{""},{""},
    {"Oct."},{"Nov."},{"Dec."}};

// to switch from 24 to 12 hours.
const unsigned char _24_12[] =
{   12,1,2,3,4,5,6,7,8,9,0,0,0,0,0,
    0,10,11,12,1,2,3,4,5,6,7,0,0,0,
    0,0,0,8,9,10,11};

const unsigned char Days_Month[] =
{   0,1,2,3,4,5,6,7,8,9,0,0,0,0,0,
    0,10,11,12,13,14,15,16,17,18,
    19,0,0,0,0,0,0,20,21,22,23,24,
    25,26,27,28,29,0,0,0,0,0,0,30,31,32};

const unsigned char D_H[] =
{   0x00,0x01,0x02,0x03,0x04,0x05,0x06,
    0x07,0x08,0x09,0,0,0,0,0,0,0x00,0x01};
const unsigned char PLP[] =
{   0x00,0x31,0x29,0x31,0x30,0x31,0x30,0x31,0x31,
    0x30,0,0,0,0,0,0,0x31,0x30,0x31};

const unsigned char NLP[] =
{   0x00,0x31,0x28,0x31,0x30,0x31,0x30,0x31,0x31,
    0x30,0,0,0,0,0,0,0x31,0x30,0x31};