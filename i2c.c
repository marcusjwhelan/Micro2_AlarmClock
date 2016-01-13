

#include <xc.h>
#include "Main.h"
#include "i2c.h"


// I2C Routines
// By:  E. Nuckolls, P.E.
// Sept. 2012
// with Marcus Whelan Addictions 


// Setup for master mode at 100kbps for a 4MHz oscillator
void setup_i2c_master(void){
    TRISC3 = 1;
    TRISC4 = 1;
    SSPCON1 = 0x28;
    SSPADD = 9;
    SSPIF = 0;
}


unsigned char Read_Data_SRAM(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_send(WRITE_SRAM);
    i2c_send(address);
    i2c_repeated_start();
    i2c_send(READ_SRAM);
    data = i2c_rceive();
    i2c_stop();
    return(data);
}

// write a byte completely to a address with all steps
// d = Read_Data(a);     read the byte d from the address a
unsigned char Read_Data_EEPROM(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_send(WRITE_EEPROM);
    i2c_send(address);
    i2c_repeated_start();
    i2c_send(READ_EEPROM);
    data = i2c_receive(0);
    i2c_stop();
    return(data);
}

//Write_Data_SRAM(a,d);         Write the byte d to the address a
void Write_Data_SRAM(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_send(WRITE_SRAM);
    i2c_send(address);
    i2c_send(data);
    i2c_stop();
}

// Write_Data(a,d);             write the byte d to the address a
void Write_Data_EEPROM(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_send(WRITE_EEPROM);
    i2c_send(address);
    i2c_send(data);
    i2c_stop();
}

void i2c_repeated_start(void){
    RSEN = 1;
    i2c_waitforidle();
}

// Generate a start
void i2c_start(void){
    SEN = 1;
    i2c_waitforidle();
}

// Generate a stop
void i2c_stop(void){
    PEN = 1;
    i2c_waitforidle();
}

// Transmit a byte
unsigned char i2c_send(unsigned char data){
    SSPBUF = data;
    i2c_waitforidle();

   // return ( ! ACKSTAT );      // function returns 1 if transmission is ack
}

// Receive a byte
//int i2c_receive(unsigned char ack){
unsigned char i2c_rceive(void){
    //unsigned char i2creaddata;

    RCEN = 1;

    //i2c_waitforidle();
    //i2creaddata = SSPBUF;
    i2c_waitforidle();
   // if( ack ){
      //  ACKDT = 0;
    //}
    //else{
    //    ACKDT = 1;
    //}
    //ACKEN = 1;                  // send acknowledge sequence
    return(SSPBUF);
    //return( i2creaddata);
}

// Generate an acknowledge
void i2c_ack(void){
    ACKDT = 0;
    ACKEN = 1;
    i2c_waitforidle();
}

void i2c_waitforidle(){
    while(SSPIF == 0);
    SSPIF = 0;
}
