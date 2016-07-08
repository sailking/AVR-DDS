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
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <math.h>
//#include <string_utils.h>

#include "includes/uart.h"
#include "includes/lcd.h"
#include "includes/twi.h"
#include "includes/dataflash.h"

//????AD9850???
#define DATA PINC5
#define WCLK PINC4
#define FQUP PINC3
#define RESET PINC2
#define ClOCK 125000000UL


signed int freq = 0;

uint32_t AD_freq = 0;
uint16_t data1 = 0;
uint16_t data2 = 0;

#define wait_joy_button()       {LCD_GotoXY(20,7);  \
	LCD_PutChar(0x10); \
	LCD_Update();      \
while(((PINA)&0x08));while(!((PINA)&0x08));_delay_ms(20);while(((PINA)&0x08)); }	// Required for DMM Board 2013
//while(PINA&0xf8);while(!(PINA&0x08));_delay_ms(20);while(PINA&0x08);}			// Required for DMM Board 2012 and before

void start ();

void AD9850_setup();
void AD9850_reset();
void get_frequence();
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

	void(*get_frequence_p)(void) = get_frequence;			//define a function pointer, point to function get_frequence

	LCD_Init();
    uart_init1();
	start ();

	AD9850_setup();
	AD9850_reset();

	get_frequence();

	for(;;)
	{
		if(~PINA&(1<<PINA7))
			AD_freq +=1000000;
			_delay_ms(100);

		if (~PINA&(1<<PINA6))
			AD_freq -=1000000;
			_delay_ms(100);

		if (~PINA&(1<<PINA5))
			AD_freq -=100000;
			_delay_ms(20);

		if (~PINA&(1<<PINA4))
			AD_freq +=100000;
			_delay_ms(100);

		if (~PINA&(1<<PINA3))
			(*get_frequence_p)();		//when Joystick Button is pressed, get the frequence from serial port again
		
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
	LCD_PutString_P(PSTR("Wang Shihang\r\n"));
	LCD_PutString_P(PSTR("Li Xuejing\r\n"));

	LCD_Update();
	wait_joy_button();
}

void AD9850_setup()
{
	DDRC |= ((1<<DATA)|(1<<WCLK)|(1<<FQUP)|(1<<RESET));		//???AD9850?????????
	PORTC &= ~((1<<DATA)|(1<<WCLK)|(1<<FQUP)|(1<<RESET));	//????????????
}

void AD9850_reset()
{
	PORTC &= ~((1<<WCLK)|(1<<FQUP));
	PORTC &= ~(1<<RESET);									//pulse RESET
	_delay_us(5);
	PORTC |= (1<<RESET);
	_delay_us(5);
	PORTC &= ~(1<<RESET);
	_delay_us(5);

	PORTC &= ~(1<<WCLK);									//pulse WCLK
	_delay_us(5);
	PORTC |= (1<<WCLK);
	_delay_us(5);
	PORTC &= ~(1<<WCLK);
	_delay_us(5);

	PORTC &= ~(1<<DATA);									//make sure DATA pin is LOW

	PORTC &= ~(1<<FQUP);									//pulse FQUP
	_delay_us(5);
	PORTC |= (1<<FQUP);
	_delay_us(5);
	PORTC &= ~(1<<FQUP);
	_delay_us(5);
	// chip ist reset now
}

void get_frequence()
{
   // uart_init1();
    //uart_init0();
	
	LCD_Clear();
	LCD_GotoXY(0,0);
	LCD_PutString_P(PSTR("waiting for\r\n "));
	LCD_PutString_P(PSTR("the frequence\r\n"));
	LCD_GotoXY(20,7);
    LCD_PutChar(0x10);
	LCD_Update();

    sei();
    while(1)
    {
		//uart0_tx_frame();
        if(data_ok == 1)
        {
			
			
			data1 = (uint16_t)(data_frame_in[0] << 8);
			data1 += (uint16_t)(data_frame_in[1]);
			data2 = (uint16_t)(data_frame_in[2] << 8);
			data2 += (uint16_t) data_frame_in[3]; // 32bit Variable mit daten drin

			AD_freq = data1;
			AD_freq <<= 16;
			AD_freq += data2;
			
			
			uart1_tx_frame(3);
			data_ok = 0;
			break;
        }
    }
}

void AD9850_Setfrequency(double freq)
{
	int i;
	double x;
	x =  pow(2,32)/125000;
	freq = freq/1000;
	uint32_t tuning_word = freq * x;
	PORTC &= ~(1<<FQUP);

	//write W0
	uint8_t W0 = tuning_word;
	for(i=0; i<8; i++ )
	{
		if(W0&0x01)
		{
			PORTC |= (1<<DATA);
		}
		else
		{
			PORTC &= ~(1<<DATA);
		}
		PORTC |= (1<<WCLK);
		PORTC &= ~(1<<WCLK);
		W0 = W0>>1;
	}

	//write W1
	uint8_t W1 = tuning_word>>8;
	for(i=0; i<8; i++ )
	{
		if(W1&0x01)
		{
			PORTC |= (1<<DATA);
		}
		else
		{
			PORTC &= ~(1<<DATA);
		}
		PORTC |= (1<<WCLK);
		PORTC &= ~(1<<WCLK);
		W1 = W1>>1;
	}

	//write W2
	uint8_t W2 = tuning_word>>16;
	for(i=0; i<8; i++ )
	{
		if(W2&0x01)
		{
			PORTC |= (1<<DATA);
		}
		else
		{
			PORTC &= ~(1<<DATA);
		}
		PORTC |= (1<<WCLK);
		PORTC &= ~(1<<WCLK);
		W2 = W2>>2;
	}

	//write W3
	uint8_t W3 = tuning_word>>24;
	for(i=0; i<8; i++ )
	{
		if(W3&0x01)
		{
			PORTC |= (1<<DATA);
		}
		else
		{
			PORTC &= ~(1<<DATA);
		}
		PORTC |= (1<<WCLK);
		PORTC &= ~(1<<WCLK);
		W3 = W3>>1;
	}

	//write W4
	uint8_t W4 = 0x00;
	for(i=0; i<8; i++ )
	{
		PORTC &= ~(1<<DATA);
		PORTC |= (1<<WCLK);
		PORTC &= ~(1<<WCLK);
		W4 = W4>>1;
	}

	PORTC |= (1<<FQUP);
	_delay_us(5);
	PORTC &= ~(1<<FQUP);
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
	itoa(((uint32_t)AD_freq/1000), getfrequency_buffer, 10);
	//sprintf(getfrequency_buffer,"%0i; %0i; %0i; %0i",data_frame_in[0],data_frame_in[1],data_frame_in[2],data_frame_in[3]);
	LCD_PutString(getfrequency_buffer);
	LCD_PutString_P(PSTR("      KHz"));
	LCD_PutChar(0x10);
	LCD_Update();
	_delay_ms(10);
}




