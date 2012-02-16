#ifndef GLCD_H
#define GLCD_H

#define CS1	PORTDbits.RD11
#define CS2 PORTDbits.RD10
#define RESET PORTDbits.RD3
#define RW PORTDbits.RD5
#define DI PORTBbits.RB15
#define ENABLE PORTDbits.RD4
#define LCD_TRIS TRISE
#define LCD_DATA PORTE
#define BACKLIGHT PORTGbits.RG8

void initLCD(void);

#define LEFT 0b01
#define RIGHT 0b10
#define BOTH 0b11
#define NONE 0b00

void setBackLightOn();
void setBackLightOff();
void delayus(unsigned char us);
void msDelay(unsigned int ms);

// internal function prototypes
// you would not normally call these directly. but you can
// if you like.
void _lcd_enable(void);
unsigned char _lcd_status(void);
void _lcd_reset(void);
void _lcd_waitbusy(void);

// public function prototypes
// call these all you want !
void lcd_screenon(unsigned char on);
void lcd_cls(void);
void lcd_setpage(unsigned char page);
void lcd_setyaddr(unsigned char y);
void lcd_selectside(unsigned char sides);
void lcd_write (unsigned char data);
unsigned char lcd_read (void);
void lcd_plotpixel(unsigned char rx, unsigned char ry);
void lcd_putchar(char c);
void lcd_puts(char *string);
//void lcd_putrs(const rom char *string);

#endif
