/* 
 * File:   eeprom.h
 * Author: marcus
 *
 * Created on October 13, 2012, 3:18 PM
 */

#ifndef EEPROM_H
#define	EEPROM_H

#define sec         0x00
#define minu        0x01
#define hou         0x02
#define mon         0x03
#define dat         0x04
#define yea         0x05
#define Backlight   0x06
#define _12or24     0x07
#define Alarmsec    0x08
#define Alarmmin    0x09
#define Alarmhou    0x0A
#define Alarm_12_24 0x0B
#define setyear     0x0C
#define setmonth    0x0D
#define setdate     0x0E

unsigned char value,byte,val;

void Write_EEPROM(unsigned char address,unsigned char Byte);
unsigned char Read_EEPROM(char value);

#endif	/* EEPROM_H */

