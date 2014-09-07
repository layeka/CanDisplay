#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <avr/pgmspace.h>

#include <util/delay.h>
#include <avr/interrupt.h>

#include "main.h"
#include "can.h"
#include "uart.h"

#include "display.h"
//#include "ea_dog_spi.h"
//#include "ws0010.h"

//######################################################################
/*
1. INTRODUCTION
Haltech ECUs broadcast on the CAN bus a number of engine parameters/sensor readings. 
Third party devices can read these CAN packets and use the values for data logging, 
displaying on a dash, etc. This document describes the CAN packets and the data they 
contain.
There are two versions of this protocol. The V1 protocol is present in all Haltech Platinum 
ECUs to date but will be deprecated in the future. Starting from firmware versions 1.11 all 
Haltech Platinum ECUs will support V2, and all products that wish to interoperate with 
Haltech ECUs should support this version.

2. ENCODING
The Haltech CAN bus operates at 1MBit and uses 11-bit IDs. In this document, the first byte 
in the packet is considered byte zero, and the 8th byte is byte 7. Data is encoded as 'Big 
Endian'. IDs are expressed in Hexadecimal.
To address individual bits within a byte, the following notation is used X:Y. The X is the byte 
number, and the Y is the bit number. Consistent with the byte numbering, the left most bit is 
the most significant, and the right most bit is the least significant. Where ranges of bytes/bits 
is specified, the addresses are inclusive. All 16-bit values are signed two's complement.

Examples
Bytes Description
0 - 1 A 16-bit value formed from the first two bytes
2:3 A single bit value from bit 3 in byte 2
0:4 -
0:7 A 4-bit value encoded in the lower nibble of the first byte

3. UNITS
Converting the raw values to units is done by multiplying the value by the value in the Units
column. For example, a road speed raw value of 1000 corresponds to a speed of 100km/h. If 
other units are required, it is the responsibility of the device reading these values to perform 
these conversions. All pressures are absolute, and it is necessary to subtract 101.3 kPa from 
the final result if gauge pressure is desired.
*/
//######################################################################
char * fixPointToAscii(int16_t num, char *str, uint16_t div);
#define KTOC_OFS 273
int16_t kelvToInt(uint16_t kelv);

volatile char rx_buffer[BUFLEN];           // Empfangspuffer Array
volatile uint8_t rx_counter = 0;                  // zähler for stelle im empfangsarray
volatile uint8_t line_finish = 0;                 // zeile komplett empfangen

volatile uint8_t encoder = 0;

uint16_t rawRpmValue = 24;
uint16_t rawThrottleValue = 0;
uint16_t rawMafPresValue = 0;
uint16_t rawCoolPresValue = 0;

uint16_t rawCoolTempValue = -400;
uint16_t rawAirTempValue = -10;
uint16_t rawFuelTempValue = 0;
uint16_t rawOilTempValue = 0;

int main(void)
{
   char charBuff[8];
      
   lcd_init();
            
   // init CAN controller
   // Haltech CAN Baud rate is 1000kBits/sec --> 1MBit/sec
   can_init(BITRATE_1_MBPS);
	
   // passive mode 
   can_set_mode(LISTEN_ONLY_MODE);
   
   // filter for all messages
   can_filter_t fltMsg;
   fltMsg.id = 0x360;
   
   // set can filter
   can_set_filter(0, &fltMsg);
   
   // message structure
   can_t msg;
	// start main loop ********************
	while(1)
	{
      CmdChk(); // check received uart commands
      
      if(can_check_message())
      {
         can_get_message(&msg);
        
         if(msg.id==0x360)
         {
               rawRpmValue = ((uint16_t)msg.data[0]<<8); // high nibble
               rawRpmValue = rawRpmValue | (0xFF & msg.data[1]); // high nibble
               
               rawThrottleValue = ((uint16_t)msg.data[2]<<8); // high nibble
               rawThrottleValue = rawThrottleValue | (0xFF & msg.data[3]); // high nibble
               
               rawMafPresValue = ((uint16_t)msg.data[4]<<8); // high nibble
               rawMafPresValue = rawMafPresValue | (0xFF & msg.data[5]); // high nibble
               
               rawCoolPresValue = ((uint16_t)msg.data[6]<<8); // high nibble
               rawCoolPresValue = rawCoolPresValue | (0xFF & msg.data[7]); // high nibble
         }
         if(msg.id==0x3E0)
         {
               rawCoolTempValue = ((uint16_t)msg.data[0]<<8); // high nibble
               rawCoolTempValue = rawCoolTempValue | (0xFF & msg.data[1]); // high nibble
               
               rawAirTempValue = ((uint16_t)msg.data[2]<<8); // high nibble
               rawAirTempValue = rawAirTempValue | (0xFF & msg.data[3]); // high nibble
               
               rawFuelTempValue = ((uint16_t)msg.data[4]<<8); // high nibble
               rawFuelTempValue = rawFuelTempValue | (0xFF & msg.data[5]); // high nibble
               
               rawOilTempValue = ((uint16_t)msg.data[6]<<8); // high nibble
               rawOilTempValue = rawOilTempValue | (0xFF & msg.data[7]); // high nibble
         }
         } 
         
           
         //   
            // cursor home 
            lcd_home();           
            lcd_puts("Wat:");
            lcd_puts("      ");
            lcd_gotoxy(4,0);
            //utoa(rawCoolTempValue,charBuff,10);
            fixPointToAscii(rawCoolTempValue,charBuff,100);
            lcd_puts(charBuff);
            
            
            lcd_gotoxy(0,1);
            lcd_puts("AiT:");
            lcd_puts("      ");
            lcd_gotoxy(4,1);
            //utoa(rawAirTempValue,charBuff,10);
            fixPointToAscii(rawAirTempValue,charBuff,10);
            lcd_puts(charBuff);
            
            lcd_gotoxy(0,2);
            lcd_puts("FuT:");
            lcd_puts("      ");  
            lcd_gotoxy(4,2);      
            //utoa(rawFuelTempValue,charBuff,10);
            fixPointToAscii(rawFuelTempValue,charBuff,0);
            lcd_puts(charBuff);
            
            lcd_gotoxy(0,3);
            lcd_puts("OiT:");
            lcd_puts("      "); 
            lcd_gotoxy(4,3);
            //utoa(rawOilTempValue,charBuff,10);
            fixPointToAscii(rawOilTempValue,charBuff,1000);
            lcd_puts(charBuff);
            
         if(encoder & 0x01)
         {
            rawFuelTempValue++;
            encoder &= ~(1<<0);            
         }
         if(encoder & 0x02)
         {
            rawFuelTempValue--;
            encoder &= ~(1<<1);            
         }
         
      rawOilTempValue++;
      _delay_ms(100);
   }
return 0;
}

char * fixPointToAscii(int16_t num, char *str, uint16_t div)
{
   char * result = str;
   char str2[5];
   uint8_t i=(div/10);
   
   if(num < 0)
   {
      num = -num;
      *str++ = '-';
   }
   else
   {
      *str++ = ' ';
   }
   
   if(div!=0)
   {
      itoa(num/div,str,10);
      strcat(str,".");
      
      // adding leading zero after decimal point only if divisor is greater than 10
      if(!((div/10) == 1))
      {
         while(i>=10)
         {
            if(i>(num-((num/div)*div))) strcat(str,"0");
            i/=10;
         }
      }
      utoa((num-((num/div)*div)),str2,10);
      strcat(str,str2);
   }
   else
   {
      utoa(num,str,10);
   }
   return result;
}

int16_t kelvToInt(uint16_t kelv)
{
   return (int16_t)((int16_t)kelv - KTOC_OFS);
}
