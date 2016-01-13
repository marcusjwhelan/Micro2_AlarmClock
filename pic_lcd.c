/*
 * File:   pic_lcd.c
 * Author: Marcus Whelan
 *
 * Created on September 10, 2012, 10:33 PM
 *
 * This is for the 4x20 character LCD display used in my MicroII class
 * It has a HD44870 chip as a driver.
 */

#include <xc.h>
#include "Main.h"
#include "pic_lcd.h"


// Moves cursor to home without clearing the display
void Cursor_Home(void)
{
    lcd_command(2);
    __delay_ms(3);
    return;
}
// Turns on cursor without blinking
void Cursor_On(void)
{
    lcd_command(0b00001110);
    return;
}

void Move_Cursor(unsigned char x, unsigned char y)
{
    unsigned char CursorPosition = 0b10000000;
    switch(x) {
        case 1:
            CursorPosition |= Line_1;
            break;
        case 2:
            CursorPosition |= Line_2;
            break;
        case 3:
            CursorPosition |= Line_3;
            break;
        case 4:
            CursorPosition |= Line_4;
            break;
        default: ;
    } // End of switch statement

    // LCD is 20 characters wide. Col numbers start at 1
    if(y < 1 || y > 20)
        return;
    CursorPosition += --y;

    lcd_command(CursorPosition);

    return;
}

void LCD_String(const unsigned char *charArray)
{
    while(*charArray)
        putch(*charArray++);
}

#define LCD_bit_bang
#ifdef  LCD_bit_bang
// I/O Pin definitions for SPI bit banging -- Change if needed.
#define RS              RC2	    // Register select output
#define LCD_Latch       RC1	    // Latch output
#define SPI_Clk         RC0         // Serial clock
#define SPI_Data        RC5	    // Serial data

#define	RS_TRIS         TRISC2      // TRIS bits for I/O pins
#define	LCD_Latch_TRIS  TRISC1
#define	SPI_Clk_TRIS    TRISC0
#define	SPI_Data_TRIS   TRISC5

#else
// I/O Pin definitions for SSP module -- Change if needed.
#define	RS              RC2	    // Register select output
#define LCD_Latch       RC4	    // Latch output
#define	SPI_Clk         RC3	    // Serial clock
#define	SPI_Data        RC5	    // Serial data

#define	RS_TRIS         TRISC2      // TRIS bits for I/O pins
#define	LCD_Latch_TRIS  TRISC4
#define	SPI_Clk_TRIS    TRISC3
#define	SPI_Data_TRIS   TRISC5

#endif


// ===== LCD COMMAND ============================================
// Sends a command to the LCD using SPI
// Use either big banging or SSP module
void lcd_command(unsigned char data) {
    RS = 0;

#ifdef  LCD_bit_bang
    unsigned char i;
    for ( i = 8 ; i != 0 ; --i ) {
        if (data & 0x80)  SPI_Data = 1;
        else              SPI_Data = 0;
        SPI_Clk = 0;
        SPI_Clk = 1;
        data <<= 1;
        }

#else
    SSPBUF = data;
    while(SSPIF == 0);
    SSPIF = 0;

#endif

    LCD_Latch = 1;
    LCD_Latch = 0;
    __delay_us(60);
}

// ===== PUT CHAR ===============================================
// Sends a character to the LCD using SPI
void putch(char data) {
    RS = 1;

#ifdef  LCD_bit_bang
    unsigned char i;
    for ( i = 8 ; i != 0 ; --i ) {
        if (data & 0x80)  SPI_Data = 1;
        else              SPI_Data = 0;
        SPI_Clk = 0;
        SPI_Clk = 1;
        data <<= 1;
        }

#else
    SSPBUF = data;
    while(SSPIF == 0);
    SSPIF = 0;

 #endif

    LCD_Latch = 1;
    LCD_Latch = 0;
    __delay_us(60);
}

// ===== PRINT NUM ==============================================
// Print one digit to the LCD

const unsigned char num_to_ascii[] = {"0123456789ABCDEF"};

void lcd_print_num(unsigned char number) {
    unsigned char num_char;
    num_char = num_to_ascii[number & 0x0F];
    putch(num_char);
}

// ===== PRINT NUM2 =============================================
// Print two digits to the LCD
void lcd_print_num2(unsigned char number) {
    unsigned char num_char;
    num_char = num_to_ascii[(number >> 4) & 0x0F];
    putch(num_char);
    num_char = num_to_ascii[number & 0x0F];
    putch(num_char);
}

// ===== SETUP LCD ==============================================
// Setup LCD serial port and initialize the LCD
void setup_lcd(void) {
// Configure I/O pins
    SPI_Clk = 1;                // Clock idle position
    LCD_Latch = 0;              // negate latch
    RS_TRIS = 0;                // set pins as outputs
    LCD_Latch_TRIS = 0;
    SPI_Clk_TRIS = 0;
    SPI_Data_TRIS = 0;

#ifndef LCD_bit_bang
// Setup SPI for serial connection
// Master mode, Fosc/4, Idle high clock
    SMP = 0;
    CKE = 0;
    SSPCON1 = 0x30;
    SSPIF = 0;
#endif

// 200ms Startup Delay
    __delay_ms(200);

// Send commands to initialize LCD
    lcd_command(0x38);          // 8-bit data, 4 lines
    lcd_command(0x0C);          // display on, cursor off
    lcd_command(1);	            // clear display
    __delay_ms(3);              // delay for clear command

}

// ===== CURSOR ON ================================================
// LCD cursor on solid
#define lcd_cursor_on()         lcd_command(0x0E)


// ===== CURSOR BLINK =============================================
// LCD cursor on blinking
#define lcd_cursor_blink()      lcd_command(0x0F)


// ===== CURSOR OFF ===============================================
// LCD cursor off
#define lcd_cursor_off()        lcd_command(0x0C)


// ===== CLEAR DISPLAY ============================================
// Clear LCD display
void lcd_clear(void) {
    lcd_command(1);
    __delay_ms(3);
}

// ===== LCD OFF ==================================================
// Turn LCD off
#define lcd_off()               lcd_command(0x08)


// ===== LCD ON ===================================================
// LCD on with no cursor
#define lcd_on                  lcd_command(0x0C)


// ===== CURSOR LEFT ==============================================
// Move LCD cursor left
#define lcd_cursor_left()       lcd_command(0x10)


// ===== CURSOR RIGHT =============================================
// Move LCD cursor right
#define lcd_cursor_right()      lcd_command(0x14)


// ===== CURSOR POSITION ==========================================
// Move cursor to specified position
#define lcd_cursor_position(position)       lcd_command(position)


