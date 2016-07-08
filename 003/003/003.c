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
#include <avr/sfr_defs.h>

#include "includes/uart.h"
#include "includes/lcd.h"
#include "includes/twi.h"
#include "includes/dataflash.h"
#include "includes/oszilloskope.h"

//????AD9850???
#define DATA PINC5
#define WCLK PINC4
#define FQUP PINC3
#define RESET PINC2
#define ClOCK 125000000UL

uint32_t freq = 0;
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

void fillDataLcdBuffer (uint8_t address, uint8_t data);
void adc_init();
void signal_display();


int main(void)
{
	// set PA3-PA7 as input and activated internal Pull-Up
	DDRA &= ~((1<<PINA3)|(1<<PINA4)|(1<<PINA5)|(1<<PINA6)|(1<<PINA7));		// Required for DMM Board 2013
	PORTA |= ((1<<PINA3)|(1<<PINA4)|(1<<PINA5)|(1<<PINA6)|(1<<PINA7));		// Required for DMM Board 2013
	
	// set PB0-PB3 as output
	DDRB = 0x0F;
	// set PB0-PB3 on high-level
	PORTB |= 0x0F;		// Required for DMM Board DMM Board 2013
	
	//void(*get_frequence_p)(void) = get_frequence;			//define a function pointer, point to function get_frequence

	LCD_Init();
    uart_init1();
	start ();
	
	AD9850_setup();
	AD9850_reset();

	//get_frequence();
	AD_freq=10000000;

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

		//if (~PINA&(1<<PINA3))
			//(*get_frequence_p)();		//when Joystick Button is pressed, get the frequence from serial port again
		
		AD9850_Setfrequency(AD_freq);
		adc_init();
		signal_display();
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
	LCD_PutString_P(PSTR("waiting for \r\n"));
	LCD_PutString_P(PSTR("inputting the \r\n"));
	LCD_PutString_P(PSTR("frequence \r\n "));
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

			uart1_tx_frame();
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

void adc_init ()
{
	ADMUX = 0b01100001;    // PA1 -> ADC1, ADLAR=1 (8-bit)
	ADCSRA |= ((1<<ADEN) | (1<<ADSC) | (1<<ADPS1)); // ADC prescaler at 4
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
	while(1);
}

void create_wave()
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		LCD_DrawPixel(i,dataLcdBuffer[i],1);
	}
}

void signal_display()
{
	//-----------------read signal from ADC-----------------------------------
	uint16_t i,k;
	//uint32_t endOfPeriod=0;
	uint8_t freqComplete=0;

	//create_raster();
	create_wave();

	for(;;)
	{
		if((~PINA&(1<<PINA7))&& (timeDiv <= 120))

		if((~PINA&(1<<PINA6)) && (timeDiv >= 1))

		if((~PINA&(1<<PINA5)) && (Ypos2 <= 60))

		if((~PINA&(1<<PINA4)) && (Ypos2 >= -60))

		if(~PINA&(1<<PINA3))
			while(~PINA&(1<<PINA3));

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




/*
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
}

void adc_init()
{
	// AREF = AVcc
	ADMUX = (1<<REFS0);
	
	// ADC Enable and prescaler of 128
	// 16000000/128 = 125000
	ADCSRA = ((1<<ADEN)|(0<<ADPS2)|(1<<ADPS1));
}



uint16_t adc_read(uint8_t ch)
{
	// select the corresponding channel 0~7
	// ANDing with '7' will always keep the value
	// of 'ch' between 0 and 7
	ch &= 0b00000111;  // AND operation with 7
	ADMUX = (ADMUX & 0xF8)|ch;     // clears the bottom 3 bits before ORing
	
	// start single conversion
	// write '1' to ADSC
	ADCSRA |= (1<<ADSC);
	
	// wait for conversion to complete
	// ADSC becomes '0' again
	// till then, run loop continuously
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}
*/



