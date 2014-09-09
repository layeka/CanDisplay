/*
File:		

Project: 
Author:	
Date:		
*/
#include <avr/io.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "main.h"
#include "tasks.h"
//#include "keys.h"
#include "uart.h"
#include "can.h"
#include "display.h"

volatile uint8_t flgTimer = 0;
uint8_t cntMenu = 0, cntScreen = 0;
int8_t swtTempLow = 0, swtTempHigh = -2;



void task10ms(void)
{
   startTask10ms();
   /* put code or functions below */
  
  
  
  
   /* put code or functions above */
   endTask10ms();
}

void task20ms(void)
{
   startTask20ms();
   /* put code or functions below */
   
   
  
   
   /* put code or functions above */
   endTask20ms();
}

void task50ms(void)
{
   startTask50ms();
   /* put code or functions below */
   
   char charBuff[8];
   //cursor home 
   lcd_home();           
   lcd_puts("Wat:");
   lcd_puts("      ");
   lcd_gotoxy(4,0);
   fixPointToAscii(rawCoolTempValue,charBuff,10);
   lcd_puts(charBuff);
   
   
   lcd_gotoxy(0,1);
   lcd_puts("AiT:");
   lcd_puts("      ");
   lcd_gotoxy(4,1);
   fixPointToAscii(rawAirTempValue,charBuff,10);
   lcd_puts(charBuff);
   
   lcd_gotoxy(0,2);
   lcd_puts("FuT:");
   lcd_puts("      ");  
   lcd_gotoxy(4,2);      
   fixPointToAscii(rawFuelTempValue,charBuff,10);
   lcd_puts(charBuff);
   
   lcd_gotoxy(0,3);
   lcd_puts("OiT:");
   lcd_puts("      "); 
   lcd_gotoxy(4,3);
   fixPointToAscii(rawOilTempValue,charBuff,10);
   lcd_puts(charBuff);
   
   lcd_gotoxy(10,0);
   lcd_puts("RPM:");
   lcd_puts("      "); 
   lcd_gotoxy(14,0);
   fixPointToAscii(rawRpmValue,charBuff,1);
   lcd_puts(charBuff);
   
   /* put code or functions above */
   endTask50ms();
}

void task100ms(void)
{
   startTask100ms();
   /* put code or functions below */
   
   /* put code or functions above */
   endTask100ms();
}

void task250ms(void)
{
   startTask250ms();
   /* put code or functions below */

   /* put code or functions above */
   endTask250ms();
}

void task500ms(void)
{
   startTask500ms();
   /* put code or functions below */
   
   /* put code or functions above */
   endTask500ms();
}

void task1000ms(void)
{
   startTask1000ms();
   /* put code or functions below */

   /* put code or functions above */
   endTask1000ms();
}

void taskTimer_init(void)
{
#ifndef __AVR_AT90CAN128__
   TCCR0A |= (1<<WGM01);
   TCCR0B |= (1<<CS02)|(1<<CS00);
#else
   TCCR0A |= (1<<WGM01)|(1<<CS02)|(1<<CS00);
#endif   
   OCR0A = CTC_TOP;
   TIMSK0 |= (1<<OCIE0A);
   sei();
}

// systimer ISR handles timer 0 compare match
// fires every 10ms
#ifndef __AVR_AT90CAN128__
ISR(TIMER0_COMPA_vect)
#else
ISR(TIMER0_COMP_vect)
#endif
{
   static uint8_t cntTimer=0;
   // increment timer counter every 10ms
   cntTimer++;
   if(!(cntTimer%1)) flgTimer|=0x01;  // 10ms
   if(!(cntTimer%2)) flgTimer|=0x02;  // 20ms
   if(!(cntTimer%5)) flgTimer|=0x08;  // 50ms
   if(!(cntTimer%10)) flgTimer|=0x10;  // 100ms
   if(!(cntTimer%25)) flgTimer|=0x20;  // 250ms
   if(!(cntTimer%50)) flgTimer|=0x40;  // 500ms
   if(!(cntTimer%100)) 
   {
      flgTimer|=0x80; // 1000ms
      cntTimer = 0;
   }
}
