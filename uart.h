/*
File:		uart.h

Project: 	Robiarm
Author:		Andy Hempel
Date:		08.06.09
*/


#ifndef _uart_h
#define _uart_h

#ifndef F_CPU
/* In neueren Version der WinAVR/Mfile Makefile-Vorlage kann
   F_CPU im Makefile definiert werden, eine nochmalige Definition
   hier wuerde zu einer Compilerwarnung fuehren. Daher "Schutz" durch
   #ifndef/#endif 
 
   Dieser "Schutz" kann zu Debugsessions führen, wenn AVRStudio 
   verwendet wird und dort eine andere, nicht zur Hardware passende 
   Taktrate eingestellt ist: Dann wird die folgende Definition 
   nicht verwendet, sondern stattdessen der Defaultwert (8 MHz?) 
   von AVRStudio - daher Ausgabe einer Warnung falls F_CPU
   noch nicht definiert: */
#warning "F_CPU war noch nicht definiert, wird nun nachgeholt mit 14745600"
#define F_CPU 16000000UL    // Systemtakt in Hz - Definition als unsigned long beachten >> Ohne ergeben Fehler in der Berechnung
#endif

#define UART	1
 
//#define BAUD 76800UL          // Baudrate
//#define BAUD 38400UL          // Baudrate
#define BAUD 250000UL          // Baudrate
 
// Berechnungen
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)   // clever runden
#define BAUD_REAL (F_CPU/(16*(UBRR_VAL+1)))     // Reale Baudrate
#define BAUD_ERROR ((BAUD_REAL*1000)/BAUD) // Fehler in Promille, 1000 = kein Fehler.
 
#if ((BAUD_ERROR<990) || (BAUD_ERROR>1010))
  #error Systematischer Fehler der Baudrate groesser 1% und damit zu hoch! 
#endif

// Empfangsringpufferlänge 
#define BUFLEN  90

extern volatile char rx_buffer[BUFLEN];           // Empfangspuffer Array
extern volatile uint8_t rx_counter;                  // zähler for stelle im empfangsarray
extern volatile uint8_t line_finish;                 // zeile komplett empfangen

extern volatile uint8_t encoder;

extern void uart_init(void);
extern void uart_puts(char *s);
extern void uart_puts_line(char *s);

extern uint8_t uart_putc(unsigned char c);
extern uint8_t get_uart(uint8_t pointer);

// check received commands
extern void CmdChk(void);

#endif // _uart_h
