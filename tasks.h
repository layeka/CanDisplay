/*
File:		

Project: 	
Author:		
Date:		
*/


#ifndef _tasks_h
#define _tasks_h

#define F_CTC        100
#define CTC_PRESCALER   1024
#define CTC_TOP (uint8_t)(((uint32_t)F_CPU/((uint16_t)CTC_PRESCALER * (uint16_t)F_CTC))-1)

#define startTask10ms()    		if(!(flgTimer & 0x01)) return
#define startTask20ms()    		if(!(flgTimer & 0x02)) return
#define startTask50ms()    		if(!(flgTimer & 0x08)) return
#define startTask100ms()    	if(!(flgTimer & 0x10)) return
#define startTask250ms()    	if(!(flgTimer & 0x20)) return
#define startTask500ms()    	if(!(flgTimer & 0x40)) return
#define startTask1000ms()    	if(!(flgTimer & 0x80)) return

#define endTask10ms()      		(flgTimer &= ~(0x01))
#define endTask20ms()      		(flgTimer &= ~(0x02))
#define endTask50ms()      		(flgTimer &= ~(0x08))
#define endTask100ms()      	(flgTimer &= ~(0x10))
#define endTask250ms()      	(flgTimer &= ~(0x20))
#define endTask500ms()      	(flgTimer &= ~(0x40))
#define endTask1000ms()      	(flgTimer &= ~(0x80))

extern void task10ms(void);
extern void task20ms(void);
extern void task50ms(void);
extern void task100ms(void);
extern void task250ms(void);
extern void task500ms(void);
extern void task1000ms(void);

extern void taskTimer_init(void);

extern volatile uint8_t flgTimer;
extern uint8_t cntMenu,cntScreen;
extern int8_t swtTempLow,swtTempHigh;
#endif // _tasks_h