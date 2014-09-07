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

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include "uart.h"
#include "main.h"


// ignor unused interrupt
ISR(BADISR_vect)
{
}

// ISR for received char over UART
#ifndef __AVR_AT90CAN128__
ISR(USART_RX_vect)
{
//	rx_buffer[rx_counter] = UDR0;
//
//	// use CR+LF for command-complete, (0x0D 0x0A)
//	if((rx_buffer[rx_counter-1] == 0x0D) && (rx_buffer[rx_counter] == 0x0A))
//	{
//		// terminate received string with \0
//		rx_buffer[rx_counter] = 0;
//		rx_buffer[rx_counter-1] = 0;
//		
//		// new command available
//		line_finish = 1;
//		
//		// reset rx counter
//		rx_counter = 0;
//	}
//	else if(   (rx_buffer[rx_counter-3] == 'B') && (rx_buffer[rx_counter-2] == 'O')
//			&& (rx_buffer[rx_counter-1] == 'O') && (rx_buffer[rx_counter] == 'T'))
//	{//BOOTLOADER
//		 wdt_reset();
//    	_WD_CONTROL_REG = (1 << WDE) | (1 << _WD_CHANGE_BIT);
//    	_WD_CONTROL_REG = (1 << WDE);
//    	for (;;);
//	}
//	else rx_counter++;
}
#else
ISR(USART0_RX_vect)
{
//	rx_buffer[rx_counter] = UDR0;
//
//	// use CR+LF for command-complete, (0x0D 0x0A)
//	if((rx_buffer[rx_counter-1] == 0x0D) && (rx_buffer[rx_counter] == 0x0A))
//	{
//		// terminate received string with \0
//		rx_buffer[rx_counter] = 0;
//		rx_buffer[rx_counter-1] = 0;
//		
//		// new command available
//		line_finish = 1;
//		
//		// reset rx counter
//		rx_counter = 0;
//	}
//	else if(   (rx_buffer[rx_counter-3] == 'B') && (rx_buffer[rx_counter-2] == 'O')
//			&& (rx_buffer[rx_counter-1] == 'O') && (rx_buffer[rx_counter] == 'T'))
//	{//BOOTLOADER
//		 wdt_reset();
//    	_WD_CONTROL_REG = (1 << WDE) | (1 << _WD_CHANGE_BIT);
//    	_WD_CONTROL_REG = (1 << WDE);
//    	for (;;);
//	}
//	else rx_counter++;
}

ISR(USART1_RX_vect)
{
   void (*bootloader)( void ) = 0x0C00; 
   
   rx_buffer[rx_counter] = UDR1;
   
   // use CR+LF for command-complete, (0x0D 0x0A)
   if((rx_buffer[rx_counter-1] == 0x0D) && (rx_buffer[rx_counter] == 0x0A))
   {
      // terminate received string with \0
      rx_buffer[rx_counter] = 0;
      rx_buffer[rx_counter-1] = 0;
      
      // new command available
      line_finish = 1;
      
      // reset rx counter
      rx_counter = 0;
   }
   else if(   (rx_buffer[rx_counter-3] == 'B') && (rx_buffer[rx_counter-2] == 'O')
         && (rx_buffer[rx_counter-1] == 'O') && (rx_buffer[rx_counter] == 'T'))
   {
      bootloader(); // jump to bootloader
         for (;;);
   }
   else if(rx_buffer[rx_counter] == '+')
   {
      encoder |= (1<<0);
      // reset rx counter
      rx_counter = 0;
   }
   else if(rx_buffer[rx_counter] == '-')
   {
      encoder |= (1<<1);
      // reset rx counter
      rx_counter = 0;
   }
   else if(rx_buffer[rx_counter] == '*')
   {
      encoder |= (1<<2);
      // reset rx counter
      rx_counter = 0;
   }
   else rx_counter++;
}
#endif


uint8_t get_uart(uint8_t pointer)
{
	return rx_buffer[pointer];
}

void uart_init(void)
{

	// for atmega8
/*	UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);
	UCSRC |= (1<<UMSEL)| (1<<UCSZ1)| (1<<UCSZ0); // Asynchron 8N1 

	UBRRH = UBRR_VAL >> 8;
    UBRRL = UBRR_VAL & 0xFF;
*/

#if UART == 0
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;
   
   // for atmega44,88,168
	//UCSR0C |= (1<<UMSEL01)| (1<<UCSZ01); // Asynchron 8N1
   //UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
   // for at90can128
	UCSR0C |= (1<<UCSZ01)| (1<<UCSZ00); // Asynchron 8N1
   UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
#elif UART == 1
   UBRR1H = UBRR_VAL >> 8;
	UBRR1L = UBRR_VAL & 0xFF;
   
   // for at90can128
	UCSR1C |= (1<<UCSZ11)| (1<<UCSZ10); // Asynchron 8N1
   UCSR1B |= (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
#endif	
	sei(); // Interrupts global enable
}


// send a char
uint8_t uart_putc(unsigned char c)
{
#if UART == 0
      // wait till transmit possible
      while (!(UCSR0A & (1<<UDRE0)))  
      {
      }                             
      
      // send Char
      UDR0 = c;
#elif UART == 1
         // wait till transmit possible
      while (!(UCSR1A & (1<<UDRE1)))  
      {
      }                             
      
      // send Char
      UDR1 = c;
#endif
    return 0;
}
 
// send a sting 
void uart_puts (char *s)
{
	// till *s != '\0' send chars from "string"
    while (*s)
    {   
        uart_putc(*s);
        s++;
    }
}


// send a sting an line-end commands
void uart_puts_line(char *s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
	uart_putc(13);
	uart_putc(10);

}

// check received commands
void CmdChk(void)
{
	
	// only execute if a complete command is received	
	if(line_finish==1)
	{
		// reset received line
		line_finish=0;
		
		// interpret commands
		switch (rx_buffer[0])
		{
      case 'W': // change screen
			rawCoolTempValue += 10;
			break;
         
		/* default is for errors or unknown commands */
		default:
			uart_puts_line("X");
			break;	
		}
			
	}
}