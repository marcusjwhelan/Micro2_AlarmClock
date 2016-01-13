
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "eeprom.h"

//Main time saves are sec,min,hour,month,date,year,backlight
// and 12 or 24 hour option
// Alarm saves are in Alarm sec, alarm min, alarm hour,
// and alarm 12 or 24 hour option
__EEPROM_DATA(sec,minu,hou,mon,dat,yea,Backlight,_12or24);
__EEPROM_DATA(Alarmsec,Alarmmin,Alarmhou,Alarm_12_24,0,0,0,0);

//Write_EEPROM(sec,s);
void Write_EEPROM(unsigned char address,unsigned char Byte){
    eeprom_write(address,Byte);
}

//x =Read_EEPROM(hou);
unsigned char Read_EEPROM(char val){
    unsigned char byte;
    byte = eeprom_read(val);
    return(byte);
}

