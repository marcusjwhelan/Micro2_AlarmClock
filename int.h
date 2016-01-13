/* 
 * File:   int.h
 * Author: marcus
 *
 * Created on October 12, 2012, 1:39 PM
 */

#ifndef INT_H
#define	INT_H

unsigned char Flag,backlight,clock;
void interrupt Backlight_Int(void); // Timer 0 overflow interrupt
void interrupt Key_Hold(void);

#endif	/* INT_H */

