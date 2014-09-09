/*
Copyright (c) 2014, Andy Hempel
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the {organization} nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>

#include "display.h"
#include "uart.h"

void lcd_init()
{
   uart_init();
   
   // cursor invisible ESC[?25l
   uart_putc(27); // vt100 esc
   uart_puts("[?25l");
   lcd_clr();
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
