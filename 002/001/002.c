/*/*
 * getfrequency.c
 *
 * Created: 2016/6/7 ??? ?? 4:55:18
 *  Author: Administrator
 */ 

#ifndef F_CPU
  #define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>

#include "includes/uart.h"
#include "includes/lcd.h"
#include "includes/twi.h"
#include "includes/dataflash.h"

//????AD9850???
#define DATA PIND2
#define WCLK PIND3
#define FQUP PIND4
#define RESET PIND6
#define ClOCK 125000000UL

signed int freq = 0;
uint32_t AD_freq = 10000;   //

#define wait_joy_button()       {LCD_GotoXY(20,7);  \
	LCD_PutChar(0x10); \
	LCD_Update();      \
while(((PINA)&0x08));while(!((PINA)&0x08));_delay_ms(20);while(((PINA)&0x08)); }	// Required for DMM Board 2013
//while(PINA&0xf8);while(!(PINA&0x08));_delay_ms(20);while(PINA&0x08);}			// Required for DMM Board 2012 and before

void start ();

void AD9850_setup();
void AD9850_reset();
void AD9850_Setfrequency(double freq);

void frequence_display();


int main(void)
{
	// set PA3-PA7 as input and activated internal Pull-Up
	DDRA &= ~((1<<PINA3)|(1<<PINA4)|(1<<PINA5)|(1<<PINA6)|(1<<PINA7));		// Required for DMM Board 2013
	PORTA |= ((1<<PINA3)|(1<<PINA4)|(1<<PINA5)|(1<<PINA6)|(1<<PINA7));		// Required for DMM Board 2013
	
	// set PB0-PB3 as output
	DDRB = 0x0F;
	// set PB0-PB3 on high-level
	PORTB |= 0x0F;		// Required for DMM Board DMM Board 2013
	
	LCD_Init();
    UART_Init();
	start ();
	
	AD9850_setup();
	AD9850_reset();
	for(;;)
	{
		if(~PINA&(1<<PINA7))
			AD_freq +=1000;
			_delay_ms(100);
		
		if (~PINA&(1<<PINA6))
			AD_freq -=1000;
			_delay_ms(100);
		
		if (~PINA&(1<<PINA5))
			AD_freq -=100;
			_delay_ms(20);
			
		if (~PINA&(1<<PINA4))
			AD_freq +=100;
			_delay_ms(100);
		
		AD9850_Setfrequency(AD_freq);
		frequence_display();
	}
}

void start ()
{
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_PutString_P(PSTR("Project 04:\r\n"));
	LCD_PutString_P(PSTR("DDS-Signalgenerator\r\n"));
	LCD_PutString_P(PSTR("Members:\r\n"));
	LCD_PutString_P(PSTR("Li Xuejing\r\n"));
	LCD_PutString_P(PSTR("Zhang Yue\r\n"));
	LCD_PutString_P(PSTR("Wang Shihang\r\n"));
	LCD_Update();
	wait_joy_button();
}

void AD9850_setup()
{
	DDRD |= ((1<<DATA)|(1<<WCLK)|(1<<FQUP)|(1<<RESET));		//???AD9850?????????
	PORTD &= ~((1<<DATA)|(1<<WCLK)|(1<<FQUP)|(1<<RESET));	//????????????
}

void AD9850_reset()
{
	PORTD &= ~((1<<WCLK)|(1<<FQUP));
	PORTD &= ~(1<<RESET);									//pulse RESET
	_delay_us(5);
	PORTD |= (1<<RESET);
	_delay_us(5);
	PORTD &= ~(1<<RESET);
	_delay_us(5);
	
	PORTD &= ~(1<<WCLK);									//pulse WCLK
	_delay_us(5);
	PORTD |= (1<<WCLK);
	_delay_us(5);
	PORTD &= ~(1<<WCLK);
	_delay_us(5);
	
	PORTD &= ~(1<<DATA);									//make sure DATA pin is LOW
	
	PORTD &= ~(1<<FQUP);									//pulse FQUP
	_delay_us(5);
	PORTD |= (1<<FQUP);
	_delay_us(5);
	PORTD &= ~(1<<FQUP);
	_delay_us(5);
	// chip ist reset now
}

void AD9850_Setfrequency(double freq)
{
	int i;
	double x;
	x =  pow(2,32)/125;
	freq = freq/1000;
	uint32_t tuning_word = freq * x;
	PORTD &= ~(1<<FQUP);
	
	//write W0
	uint8_t W0 = tuning_word;
	for(i=0; i<8; i++ )
	{
		if(W0&0x01)
		{
			PORTD |= (1<<DATA);
		}
		else
		{
			PORTD &= ~(1<<DATA);
		}
		PORTD |= (1<<WCLK);
		PORTD &= ~(1<<WCLK);
		W0 = W0>>1;
	}
	
	//write W1
	uint8_t W1 = tuning_word>>8;
	for(i=0; i<8; i++ )
	{
		if(W1&0x01)
		{
			PORTD |= (1<<DATA);
		}
		else
		{
			PORTD &= ~(1<<DATA);
		}
		PORTD |= (1<<WCLK);
		PORTD &= ~(1<<WCLK);
		W1 = W1>>1;
	}
	
	//write W2
	uint8_t W2 = tuning_word>>16;
	for(i=0; i<8; i++ )
	{
		if(W2&0x01)
		{
			PORTD |= (1<<DATA);
		}
		else
		{
			PORTD &= ~(1<<DATA);
		}
		PORTD |= (1<<WCLK);
		PORTD &= ~(1<<WCLK);
		W2 = W2>>2;
	}
	
	//write W3
	uint8_t W3 = tuning_word>>24;
	for(i=0; i<8; i++ )
	{
		if(W3&0x01)
		{
			PORTD |= (1<<DATA);
		}
		else
		{
			PORTD &= ~(1<<DATA);
		}
		PORTD |= (1<<WCLK);
		PORTD &= ~(1<<WCLK);
		W3 = W3>>1;
	}
	
	//write W4
	uint8_t W4 = 0x00;
	for(i=0; i<8; i++ )
	{
		PORTD &= ~(1<<DATA);
		PORTD |= (1<<WCLK);
		PORTD &= ~(1<<WCLK);
		W4 = W4>>1;
	}
	
	PORTD |= (1<<FQUP);
	_delay_us(5);
	PORTD &= ~(1<<FQUP);
}

void frequence_display()
{
	char getfrequency_buffer[32];
	LCD_Clear();
	Backlight_LED(BL_BLUE_ON);
	// initialize adc and lcd
	
	LCD_GotoXY(0,0);
	// display the labels on LCD
	LCD_PutString_P(PSTR("The Frequence ist   \r\n\n"));
	itoa(AD_freq, getfrequency_buffer, 10);
	LCD_PutString(getfrequency_buffer);
	LCD_PutString_P(PSTR("      KHz"));
	LCD_PutChar(0x10);
	LCD_Update();
	_delay_ms(10);
}




