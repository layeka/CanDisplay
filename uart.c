/*
File:		uart.c

Project: 	Robiarm
Author:		Andy Hempel
Date:		08.06.09
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
   void (*bootloader)( void ) = 0x0C00;  // Achtung Falle: Hier Word-Adresse
   
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

// initialization function
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
         
#ifdef __UARTDEBUG__
		case 1:  // Debug informations
			// OilTemp debug
			if(rx_buffer[1] & 0x01) 
			{
				vdoOilTemp.sim = 1;
			}
			else
			{
				vdoOilTemp.sim = 0;
			}				
			
			// WatTemp debug
			if(rx_buffer[1] & 0x02) 
			{
				vdoWaterTemp.sim = 1;
			}
			else
			{
				vdoWaterTemp.sim = 0;
			}
			
			// Boost debug
			if(rx_buffer[1] & 0x04) 
			{
				boost.sim = 1;
			}
			else
			{
				boost.sim = 0;
			}
			
			// Oil pressure debug
			if(rx_buffer[1] & 0x08) 
			{
				vdoOilPress.sim = 1;
			}
			else
			{
				vdoOilPress.sim = 0;
			}
			
			// RPM Speed debug
			if(rx_buffer[1] & 0x10)
			{
				counter_speed_rpm_stop();
			}
			else
			{
				counter_speed_rpm_init();
			}
			
			// LED flasher debug
			if(rx_buffer[1] & 0x20)
			{
				WARNING_LED_PORT &= ~(1<<WARNING_LED_PIN);
				leddebug = 1;
			}
			else
			{
				WARNING_LED_DDR |= (1<<WARNING_LED_PIN);
				leddebug = 0;
			}
			
			break;
		
		case 2: // change screen
			menu_cnt = rx_buffer[1];
			break;
		
		case 3: // request for settings
			// 8bit values
			// Byte 0-8
			for (uint8_t i=0;i<9;i++)
			{
				uart_putc(eeprom_read_byte(&eeprom_8bit_array[i]));
			}
			// 16bit values
			// Byte 9-14
			for (uint8_t i=0;i<3;i++)
			{
				uart_putc((eeprom_read_word(&eeprom_16bit_array[i])&0x00FF));
				uart_putc((eeprom_read_word(&eeprom_16bit_array[i]))>>8);
			}
			// oiltemp values
			// Byte 15-18
			for (uint8_t i=0;i<4;i++)
			{
				uart_putc(vdoOilTemp.label[i]);
			}
			// oiltemp phys. uint
			// Byte 19-18
			for (uint8_t i=0;i<4;i++)
			{
				uart_putc(vdoOilTemp.label[i]);
			}
			break;
			
		case 4:
			// sensor curve length values
			uart_putc((vdoOilTemp.length));
			uart_putc((vdoWaterTemp.length));
			uart_putc((vdoOilPress.length));
			uart_putc((boost.length));
			break;
			
		case 5:
			// send values for oiltemp
			for (uint8_t i=0;i<((vdoOilTemp.length));i++)
			{
				uart_putc(vdoOilTemp.curve[i]&0x00FF);
				uart_putc(vdoOilTemp.curve[i]>>8);
			}
			// send values for watertemp
			for (uint8_t i=0;i<((vdoWaterTemp.length));i++)
			{
				uart_putc(vdoWaterTemp.curve[i]&0x00FF);
				uart_putc(vdoWaterTemp.curve[i]>>8);
			}
			// send values for oiltemp
			for (uint8_t i=0;i<(vdoOilPress.length);i++)
			{
				uart_putc(vdoOilPress.curve[i]&0x00FF);
				uart_putc(vdoOilPress.curve[i]>>8);
			}
			// send values for boost
			for (uint8_t i=0;i<(boost.length);i++)
			{
				uart_putc(boost.curve[i]&0x00FF);
				uart_putc(boost.curve[i]>>8);
			}
			break;
			
		case 10: // Oiltemp overwrite
			vdoOilTemp.currentRaw  = (uint16_t)rx_buffer[2]<<8;
			vdoOilTemp.currentRaw  |= rx_buffer[1];
			break;
			
		case 11: // Watertemp overwrite
			vdoWaterTemp.currentRaw = (uint16_t)rx_buffer[2]<<8;
			vdoWaterTemp.currentRaw |= rx_buffer[1];
			break;
			
		case 12: // Boost pressure overwrite
			
			boost.currentRaw = (uint16_t)rx_buffer[2]<<8;
			boost.currentRaw |= rx_buffer[1];
			break;	
			
		case 13: // Oil pressure overwrite
			vdoOilPress.currentRaw = (uint16_t)rx_buffer[2]<<8;
			vdoOilPress.currentRaw |= rx_buffer[1];
			break;

		case 14: // RPM overwrite
			rpm_raw_value  = (uint16_t)rx_buffer[2]<<8;
			rpm_raw_value |= rx_buffer[1];
			break;
			
		case 15: // RPM overwrite
			speed_raw_value  = (uint16_t)rx_buffer[2]<<8;
			speed_raw_value |= rx_buffer[1];
			break;
		
		case 16: // Day light 
			lcd_bright_day = rx_buffer[1];
			break;
		
		case 17: // Night light 
			lcd_bright_day = rx_buffer[1];
			break;
		
		case 18: // LCD Contrast
			lcd_contrast = rx_buffer[1];
			lcd_set_contrast(lcd_contrast);
			break;
		
		case 19: // flasher PWM
			flasher_pwm_state = rx_buffer[1];
			break;
#endif			

		/* default is for errors or unknown commands */
		default:
			uart_puts_line("X");
			break;	
		}
			
	}
}