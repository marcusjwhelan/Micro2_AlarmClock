/*
 * File:   pic_lcd.h
 * Author: marcus
 *
 * Created on October 4, 2012, 8:06 PM
 */

#ifndef PIC_LCD_H
#define	PIC_LCD_H

#define Line_1      0x00
#define Line_2      0x40
#define Line_3      0x14
#define Line_4      0x54

#define _XTAL_FREQ      4000000     // Oscillator frequency


void putch(char data);
void lcd_command(unsigned char data);
void setup_lcd(void);
void lcd_clear(void);
void lcd_print_num2(unsigned char number);
void lcd_print_num(unsigned char number);
void Cursor_Home(void);
void Cursor_On(void);
void Move_Cursor(unsigned char x, unsigned char y);
void LCD_String(const unsigned char *charArray);

#endif	/* PIC_LCD_H */

