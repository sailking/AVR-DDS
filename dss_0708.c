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

#include "includes/uart.h"
#include "includes/lcd.h"
#include "includes/twi.h"
#include "includes/dataflash.h"
#include "includes/oszilloskope.h"

//????AD9850???
#define DATA PINB1
#define WCLK PINB2
#define FQUP PIND6
#define RESET PIND7
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

const uint8_t PROGMEM raster[1024] =
{
	/*--  ������һ��ͼ����F:\DMM_Projekt\LCDͼ������\raster_screen.bmp  --*/
	/*--  ����x�߶�=128x64  --*/
	0xFF,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0xAB,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,
	0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x03,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x01,0x01,0x01,0xFF,0x00,0x00,0xF0,0xF8,0x8C,0x8C,0xF8,0xF0,0x00,0x00,0xFC,
	0xFC,0x00,0x00,0xFC,0xFC,0x00,0x00,0x04,0xFC,0xFC,0x44,0xC4,0xFC,0x38,0x00,0x00,
	0xFF,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0xAA,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x08,0xFF,0x00,0x00,0xC7,0x27,0x20,0xC0,0x07,0x07,0x00,0x00,0x01,
	0x03,0x06,0x06,0x03,0x01,0x00,0x00,0x04,0x07,0x07,0x00,0x00,0x07,0x07,0x00,0x00,
	0xFF,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x20,0xAA,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x20,0xFF,0x00,0x00,0x19,0x22,0x22,0x1C,0x00,0x1E,0x21,0x21,0x21,
	0x00,0x1E,0x21,0x21,0x1E,0x00,0xFF,0x21,0x21,0x1E,0x00,0x1E,0x29,0x29,0x2E,0x00,
	0xFF,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xC0,0x00,0x80,0x00,0x80,0x00,0x80,
	0x00,0x80,0x00,0xC0,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xC0,0x00,0x80,
	0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xC0,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,
	0x00,0xAA,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xC0,0x00,0x80,0x00,0x80,
	0x00,0x80,0x00,0x80,0x00,0xC0,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xC0,
	0x00,0x80,0x00,0x80,0x00,0x80,0x00,0x80,0x00,0xC0,0x00,0x80,0x00,0x80,0x00,0x80,
	0x00,0x80,0x00,0x80,0xFF,0x00,0x00,0x00,0xF0,0x50,0x10,0x00,0xF0,0x50,0xA0,0x00,
	0xF0,0x50,0x10,0x00,0xE0,0x10,0x10,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0xAA,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x01,0x00,
	0x01,0x01,0x01,0x00,0x00,0x01,0x01,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xFF,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x02,0xAA,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x02,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xFF,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x08,0xAA,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x08,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0xF8,0x20,0xD8,0x00,0xF8,0x20,0xF8,0x00,0x90,0xD0,0xB0,0x00,
	0xFF,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xA0,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0xA0,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xA0,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xA0,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0xAA,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xA0,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0x80,0xA0,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xA0,
	0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0xA0,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x80,0x80,0x80,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,

};

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
	adc_init();
	
	AD9850_setup();
	AD9850_reset();
	
	start ();
	
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
			_delay_ms(100);

		if (~PINA&(1<<PINA4))
			AD_freq +=100000;
			_delay_ms(100);

		if (~PINA&(1<<PINA3))
			(*get_frequence_p)();		//when Joystick Button is pressed, get the frequence from serial port again
		
		AD9850_Setfrequency(AD_freq);
		create_raster();
		frequence_display();
		//signal_display();
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

void adc_init ()
{
	ADMUX = 0b01100001;    // PA1 -> ADC1, ADLAR=1 (8-bit)
	ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1)); // ADC prescaler at 4
}

void AD9850_setup()
{
	DDRB |= ((1<<DATA)|(1<<WCLK));		//???AD9850?????????
	PORTB &= ~((1<<DATA)|(1<<WCLK));
	DDRD |=((1<<FQUP)|(1<<RESET));
	PORTD &= ~((1<<FQUP)|(1<<RESET));
}

void AD9850_reset()
{
	PORTB &= ~(1<<WCLK);
	PORTD &= ~(1<<FQUP);
	PORTD &= ~(1<<RESET);									//pulse RESET
	_delay_us(5);
	PORTD |= (1<<RESET);
	_delay_us(5);
	PORTD &= ~(1<<RESET);
	_delay_us(5);
	
	PORTB &= ~(1<<WCLK);									//pulse WCLK
	_delay_us(5);
	PORTB |= (1<<WCLK);
	_delay_us(5);
	PORTB &= ~(1<<WCLK);
	_delay_us(5);
	
	PORTB &= ~(1<<DATA);									//make sure DATA pin is LOW
	
	PORTD &= ~(1<<FQUP);									//pulse FQUP
	_delay_us(5);
	PORTD |= (1<<FQUP);
	_delay_us(5);
	PORTD &= ~(1<<FQUP);
	_delay_us(5);
	// chip ist reset now
}

void get_frequence()
{
    _delay_ms(200);
	LCD_Clear();
	LCD_GotoXY(0,2);
	LCD_PutString_P(PSTR("    waiting for "));
	LCD_GotoXY(0,4);
	LCD_PutString_P(PSTR("   the frequence..."));
	LCD_GotoXY(20,7);
    LCD_PutChar(0x10);
	LCD_Update();

    sei();
    while(1)
    {
		_delay_ms(200);
		if (~PINA&(1<<PINA3))
			break;
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
	x =  pow(2,32)/125;
	freq = freq/1000000;
	uint32_t tuning_word = freq * x;
	PORTD &= ~(1<<FQUP);
	
	//write W0
	uint8_t W0 = tuning_word;
	for(i=0; i<8; i++ )
	{
		if(W0&0x01)
		{
			PORTB |= (1<<DATA);
		}
		else
		{
			PORTB &= ~(1<<DATA);
		}
		PORTB |= (1<<WCLK);
		PORTB &= ~(1<<WCLK);
		W0 = W0>>1;
	}
	
	//write W1
	uint8_t W1 = tuning_word>>8;
	for(i=0; i<8; i++ )
	{
		if(W1&0x01)
		{
			PORTB |= (1<<DATA);
		}
		else
		{
			PORTB &= ~(1<<DATA);
		}
		PORTB |= (1<<WCLK);
		PORTB &= ~(1<<WCLK);
		W1 = W1>>1;
	}
	
	//write W2
	uint8_t W2 = tuning_word>>16;
	for(i=0; i<8; i++ )
	{
		if(W2&0x01)
		{
			PORTB |= (1<<DATA);
		}
		else
		{
			PORTB &= ~(1<<DATA);
		}
		PORTB |= (1<<WCLK);
		PORTB &= ~(1<<WCLK);
		W2 = W2>>2;
	}
	
	//write W3
	uint8_t W3 = tuning_word>>24;
	for(i=0; i<8; i++ )
	{
		if(W3&0x01)
		{
			PORTB |= (1<<DATA);
		}
		else
		{
			PORTB &= ~(1<<DATA);
		}
		PORTB |= (1<<WCLK);
		PORTB &= ~(1<<WCLK);
		W3 = W3>>1;
	}
	
	//write W4
	uint8_t W4 = 0x00;
	for(i=0; i<8; i++ )
	{
		PORTB &= ~(1<<DATA);
		PORTB |= (1<<WCLK);
		PORTB &= ~(1<<WCLK);
		W4 = W4>>1;
	}
	
	PORTD |= (1<<FQUP);
	_delay_us(5);
	PORTD &= ~(1<<FQUP);
}

void fillDataLcdBuffer (uint8_t address, uint8_t data)
{
	dataLcdBuffer[address] = data;
}

void create_raster()
{
	uint16_t raster_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
		for (fb_x = 0; fb_x < 128; fb_x++)
			lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&raster[raster_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
}

void frequence_display()
{
	char getfrequency_buffer[32];
	LCD_GotoXY(17,5);
	// display the labels on LCD
	itoa(((uint32_t)AD_freq/1000), getfrequency_buffer, 10);
	//sprintf(getfrequency_buffer,"%0i; %0i; %0i; %0i",data_frame_in[0],data_frame_in[1],data_frame_in[2],data_frame_in[3]);
	LCD_PutString(getfrequency_buffer);
	LCD_GotoXY(20,7);
	LCD_PutChar(0x10);
	LCD_Update();
	_delay_ms(10);
}

void create_wave()
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		LCD_DrawPixel(i,dataLcdBuffer[i],1);
		LCD_Update();
	}
}

void signal_display()
{
	//-----------------read signal from ADC-----------------------------------
	uint16_t i,k;
	//uint32_t endOfPeriod=0;
	uint8_t freqComplete=0;

	//LCD_Clear();
	//create_wave();

	for(;;)
	{
		if((~PINA&(1<<PINA7))&& (timeDiv <= 120))

		if((~PINA&(1<<PINA6)) && (timeDiv >= 1))

		if((~PINA&(1<<PINA5)) && (Ypos2 <= 60))

		if((~PINA&(1<<PINA4)) && (Ypos2 >= -60))

		if(~PINA&(1<<PINA3))
			break;

		findZero = 0;
		upLimit = 0;
		lowLimit = 255;
		//endOfPeriod = 0;
		freqComplete = 0;
		complete = FALSE;

		for (i=2; i<15000; i++)
		{
			ADCSRA |= (1 << ADSC); // Enable ADC
			loop_until_bit_is_set(ADCSRA, ADIF); // wait until conversion complete.
			ADCvalue = ADCH;

			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 0))
				freqComplete = 1;
			
			//If you have found the start of the period, find the rise of the waveform.
			if((ADCvalue < trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 1))
				freqComplete = 2;

			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 2))
			{
				freqComplete = 3; //we found the end of the first period.
			}

			prevADCvalue = ADCvalue; // Get a backup of the current ADC value.

			for(k=timeDiv;k>0;k--) 
			{
				ADCSRA |= (1 << ADSC);   // Enable ADC
				loop_until_bit_is_set(ADCSRA, ADIF); // wait until conversion complete.
				ADCvalue = ADCH;
			}

			if (upLimit < ADCvalue)  // Find the higher voltage level of the input waveform.
				upLimit = ADCvalue;

			if (lowLimit > ADCvalue) // Find the lower voltage level of the input waveform.
				lowLimit = ADCvalue;
		
			if (ADCvalue > 0)
			{
				voltage = ((upLimit-lowLimit)*2); //Get the Vpp and store it to "voltage" (Volts Peak-to-peak of inputed waveform).
				ADCvalue += 5;
				ADCvalue /= 5;
				ADCvalue += 2;
			}
			else
				ADCvalue = 2;
		
			position = ADCvalue + Ypos2 +5; 
			if ((position <= 63) && (position >= 0) && (i<100))
				fillDataLcdBuffer(i,position);
			else
			{
				if(i<100)
					fillDataLcdBuffer(i,0);

				if((i>100)&&(freqComplete==3))
					break;
			}
		}
		if(upLimit != lowLimit)
			trigger = (((upLimit - lowLimit)/2)+ lowLimit);
		else
			trigger = upLimit;

//--------------------display the signal----------------------------
		create_wave();
//-------------------------------------------------------------------

		dataCounter = 0;
		complete = FALSE;
		freqComplete = 0;

		do
		{
			prevADCvalue = ADCvalue;
			
			ADCSRA |= (1 << ADSC);    // Enable ADC
			loop_until_bit_is_set(ADCSRA, ADIF);
			ADCvalue = ADCH;

			//Find the start of the period of the measured waveform. 
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 0))
				freqComplete = 1;
	
			//If you have found the start of the period, find the rise of the waveform.
			if((ADCvalue < trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 1))
				freqComplete = 2;

			//The next step is to find the start of the next period...
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 2))
			{
				freqComplete = 3; 
				complete = TRUE;
			}
			if(dataCounter > 3000)
				complete = TRUE;
			dataCounter++;
		}while(complete == FALSE);
	}
}






