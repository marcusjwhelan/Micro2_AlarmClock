/* 
 * File:   i2c.h
 * Author: marcus
 *
 * Created on October 8, 2012, 3:12 PM
 */

#ifndef I2C_H
#define	I2C_H

#define	SDA		RC4
#define	SCL		RC3
#define	TRUE	1
#define	FALSE	0
#define WRITE_EEPROM            0xAE
#define READ_EEPROM             0xAF
#define	WRITE_SRAM      	0xDE
#define	READ_SRAM       	0xDF

#define Seconds                 0x00
#define Minutes                 0x01
#define Hours                   0x02
#define OSCON_Day               0x03
#define Date                    0x04
#define Month                   0x05
#define Year                    0x06

#define Alarm_seconds           0x0A
#define Alarm_minutes           0x0B
#define Alarm_hours             0x0C
#define Alarm_day               0x0D
#define Alarm_date              0x0E
#define Alarm_month             0x0F

#define Timestamp_PD_minutes    0x18
#define Timestamp_PD_hours      0x19
#define Timestamp_PD_date       0x1A
#define Timestamp_PD_month      0x1B

#define Timestamp_PU_minutes    0x1C
#define Timestamp_PU_hours      0x1D
#define Timestamp_PU_date       0x1E
#define Timestamp_PU_month      0x1F
#define Access_Config           0x07

#define SRAM                    0x20
#define Calibration             0x08
#define Unlock_ID               0x09
#define Control                 0x07

void setup_i2c_master(void);
unsigned char Read_Data_EEPROM(unsigned char address);
unsigned char Read_Data_SRAM(unsigned char address);
void Write_Data_SRAM(unsigned char data, unsigned char address);
void Write_Data_EEPROM(unsigned char data, unsigned char address);
void i2c_start(void);
void i2c_stop(void);
unsigned char i2c_send(unsigned char data);
int i2c_receive(unsigned char ack);
void i2c_ack(void);
void i2c_waitforidle();
void i2c_repeated_start(void);


void i2c_write(unsigned char data);
unsigned char i2c_read(void);
void write_data_sram(unsigned char data, unsigned char address);
unsigned char read_data_sram(unsigned char address);
unsigned char i2c_rceive(void);

#endif	/* I2C_H */

