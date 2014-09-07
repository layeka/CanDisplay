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


#ifndef _uart_h
#define _uart_h

#ifndef F_CPU
#warning "F_CPU was not defined, will now be set to 16000000"
#define F_CPU 16000000UL
#endif

#define UART	1
 
//#define BAUD 76800UL 
//#define BAUD 38400UL 
#define BAUD 250000UL  
 
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))  
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD)
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Baudrate error differs more then 1%! 
#endif

// receive buffer length
#define BUFLEN  90

extern volatile char rx_buffer[BUFLEN];
extern volatile uint8_t rx_counter;    
extern volatile uint8_t line_finish;   

extern volatile uint8_t encoder;

extern void uart_init(void);
extern void uart_puts(char *s);
extern void uart_puts_line(char *s);

extern uint8_t uart_putc(unsigned char c);
extern uint8_t get_uart(uint8_t pointer);

// check received commands
extern void CmdChk(void);

#endif // _uart_h
