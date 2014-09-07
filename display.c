/*
 * ea_dog_spi.c
 *
 * Created: 30.05.2012 18:35:09
 *  Author: andy
 */ 
 
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "display.h"
#include "uart.h"

void lcd_init()            //LCD initialisieren
{
   uart_init();
   
   // cursor invisible ESC[?25l
   uart_putc(27); // vt100 esc
   uart_puts("[?25l");
}

void lcd_clr()
{
	// clear screen
   uart_putc(27); // vt100 esc
   uart_puts("[2J");
}

void lcd_home()
{
	// ESC[y,xH
   uart_putc(27); // vt100 esc
   uart_putc('[');
   uart_putc('H');
}

void lcd_putc(unsigned char c)
{
	uart_putc(c);
}

void lcd_puts(const char *s)
{
	register char c;

    while ( (c = *s++) ) {
        lcd_putc(c);
    }
}

void lcd_gotoxy(uint8_t x, uint8_t y)
{
   char charBuff[4];
	// ESC[y,xH
   uart_putc(27); // vt100 esc
   uart_putc('[');
   utoa(y+1,charBuff,10);
   uart_puts(charBuff); 
   uart_putc(';');
   utoa(x+1,charBuff,10);
   uart_puts(charBuff); 
   uart_putc('H');
}
