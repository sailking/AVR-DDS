/*
 * Pumpensteuerung.c
 *
 * Created: 06.07.2015 20:38:53
 *  Author: soeren
 */ 
#define F_CPU 16000000UL

#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "uart.h"
#include <avr/wdt.h>
#include <util/atomic.h>


// *****************MAIN*****************

int main(void)
{				   
	DDRB = 0b00000000;  
	PORTB = 0b00000000;
					  
	DDRC = 0b00000000;  
	PORTC = 0b00000000;

	DDRD = 0b00000000; 
   	PORTD = 0b00000000;	   


// *****************Timer 0*****************
/*
	TCCR0A |= (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);

	TCCR0B |= (0<<WGM02) | (0<<CS02) | (1<<CS01) | (1<<CS00);

	OCR0A = 156;
	
	TIMSK0  |= (1<<OCIE0A) | (0<<OCIE0B) | (0<<TOIE0);

// *****************Timer 2*****************

	TCCR2 |= (0<<CS22) | (0<<CS21) | (0<<CS20); //1 0 1
	TCCR2 |= (0<<WGM21) | (0<<WGM20) | (0<<COM21) | (0<<COM20) | (0<<FOC2);
	ASSR |= (0<<TCN2UB) | (0<<TCR2UB) | (1<<AS2) | (0<<OCR2UB);
	TIMSK |= (0<<TOIE2);
	//OCR2 = 127;

// *****************Timer 1*****************

	TCCR1B |= (0<<CS12) | (0<<CS11) | (0<<CS10); 
	TCCR1B |= (0<<WGM13) | (1<<WGM12);
	TCCR1A |= (0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<FOC1A) | (0<<FOC1B) | (0<<WGM11) | (0<<WGM10);
	OCR1A = 400;
	OCR1B = 0;
//	ICR1 = 255;
	TIMSK  |= (0<<OCIE1A);
	TIMSK  |= (0<<OCIE1B);
*/

uart_init1();
uart_init0();

sei();

while(1)
{	
	if(data_ok == 1) // Empfangene Daten in Ordnung -> antworten!
	{
		uart1_tx_frame();
		//uart0_tx_frame();
		data_ok = 0;
	}
}
}
