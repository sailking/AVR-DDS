/* Grab arms 2.0
 * fuck_avr.c
 * Hauptprogramm
 * Die Aufgabe dieses Programms ist 4 Motoren zu steuern, um einen Objekt zu greifen und zu stellen. Damit zeigt es die Informationen und Aktionen auf LCD.
 * Eingang: Kein
 * Ausgangг║	M1_Step: PD6  M1_Dir: PD5;
 *				M2_Step: PA1  M2_dir: PA0;			
 *				M3_Step: PB1  M3_Dir: PB0;
 *				M4_Step: PD3  M4_Dir: PD2;
 * Created: 2015/5/27 17:57:31
 *  Author: Lin
 */ 
#ifndef F_CPU
  #define F_CPU 16000000UL
#endif
/******************************************************************************************/
// Hier sind die Headers aus AVRStudio
#include <avr/io.h>						
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
/******************************************************************************************/
// Hier sind die Headers selbst gemacht				
#include "includes/lcd.h"
#include "includes/twi.h"
#include "includes/dataflash.h"
#include "includes/action.h"
/******************************************************************************************/
#define wait_joy_button(){ while(((PINA)&0x08));while(!((PINA)&0x08));_delay_ms(20);while(((PINA)&0x08));}	
// Joystick wird als einen Drucktaste benuzt, die gegen Erschuetterung ist.						 
/******************************************************************************************/
#define A_0 118							// Beispielweise ist A_0 die Anzhaele der Drehung von Motor 1 beim Greifen.
#define A_1 113							// Grad=Grad(Mode)*N  Drehwinkel=0,45 * N
#define A_2 182							// 
#define A_3 69
#define B_0 118
#define B_1 102
#define B_2 178
#define B_3 80
/******************************************************************************************/
#define speed_M0 5						// speed_M0 ist Zeit, die halb Period von Motor 1. D.h. T=10ms und f=100Hz.
#define speed_M1 5						// Deshalb ist Winkelgeschwindigkeit ist 200Pi Rad/s.
#define speed_M2 5
#define speed_M3 5

#define _grab_init_1(){ PORTD |= (1<<PIND5);PORTA |= (1<<PINA0);PORTB &= ~(1<<PINB0);PORTD |= (1<<PIND2);}	// Motoren 1,2 und 4 drehen gegen Uhrzeitrichtung, und Motror 3 dreht mit Uhrzeitrichtung.
#define _grab_init_2(){ PORTD &= ~(1<<PIND5);PORTA &= ~(1<<PINA0);PORTB |= (1<<PINB0);PORTD&= ~(1<<PIND2);}	// Motoren 1,2 und 4 drehen mit Uhrzeitrichtung, und Motror 3 dreht gegen Uhrzeitrichtung.
#define _put_init_1(){ PORTD &= ~(1<<PIND5);PORTA |= (1<<PINA0);PORTB &= ~(1<<PINB0);PORTD |= (1<<PIND2);}	// Motoren 1 und 4 drehen gegen Uhrzeitrichtung, und Motroren 1 und 3 drehen mit Uhrzeitrichtung.
#define _put_init_2(){ PORTD |= (1<<PIND5);PORTA &= ~(1<<PINA0);PORTB |= (1<<PINB0);PORTD &= ~(1<<PIND2);}	// Motoren 1 und 4 drehen mit Uhrzeitrichtung, und Motroren 1 und 3 drehen gegen Uhrzeitrichtung.

	
/******************************************************************************************/								
void _start (void);					// Anfangsbild, das die Informationen von Projekt zeigt.
void _status (void);				// Status von Motoren
void _finish (void);				// Ende
/******************************************************************************************/
void _execute(void);					// Durchfuehrung von Manipolator
/******************************************************************************************/
void _grab_action_1(void);				// Glenk 1 dreht gegen Uhrzeitrichtung 53.1бу
void _grab_action_2(void);				// Glenk 3 dreht mit Uhrzeitrichtung 81.9бу
void _grab_action_3(void);				// Glenk 4 dreht gegen Uhrzeitrichtung 31.05бу
void _grab_action_4(void);				// Glenk 2 dreht gegen Uhrzeitrichtung 50.85бу
void _grab_action_5(void);				// Glenk 2 dreht mit Uhrzeitrichtung 50.85бу
void _grab_action_6(void);				// Glenk 3 dreht gegen Uhrzeitrichtung 81.9бу
void _grab_action_7(void);				// Glenk 4 dreht mit Uhrzeitrichtung 31.05бу
void _grab_action_8(void);				// Glenk 1 dreht mit Uhrzeitrichtung 53.1бу
/******************************************************************************************/
/*	Nach Greifen bewegt Manipolator zurueck zur Anfangstelle, dann fuehrt er naechste Aufgabe, danach bewegt noch zurueck.      */
/******************************************************************************************/
void _put_action_1(void);				// Glenk 1 dreht mit Uhrzeitrichtung 53.1бу
void _put_action_2(void);				// Glenk 3 dreht mit Uhrzeitrichtung 80.1бу
void _put_action_3(void);				// Glenk 4 dreht gegen Uhrzeitrichtung 36бу
void _put_action_4(void);				// Glenk 2 dreht gegen Uhrzeitrichtung 45.9бу
void _put_action_5(void);				// Glenk 2 dreht mit Uhrzeitrichtung 45.9бу
void _put_action_6(void);				// Glenk 3 dreht gegen Uhrzeitrichtung 80.1бу
void _put_action_7(void);				// Glenk 4 dreht mit Uhrzeitrichtung 65бу
void _put_action_8(void);				// Glenk 1 dreht gegen Uhrzeitrichtung 53.1бу
/******************************************************************************************/
void _show_action_1 (void);			// 19 Teilen der Aktionen stellen ganze Ausfuehrung dar.
void _show_action_2 (void);
void _show_action_3 (void);
void _show_action_4 (void);
void _show_action_5 (void);
void _show_action_6 (void);
void _show_action_7 (void);
void _show_action_8 (void);
void _show_action_9 (void);
void _show_action_10(void);
void _show_action_11(void);
void _show_action_12(void);
void _show_action_13(void);
void _show_action_14(void);
void _show_action_15(void);
void _show_action_16(void);
void _show_action_17(void);
void _show_action_18(void);
void _show_action_19(void);
/******************************************************************************************/
/*									Hauptprogramm										  */
/******************************************************************************************/
int main(void)
{
    // set PA3-PA7 as input and activated internal Pull-Up	
  DDRA &= ~((1<<PINA3)|(1<<PINA4)|(1<<PINA5)|(1<<PINA6)|(1<<PINA7));		// Required for DMM Board 2013
  PORTA |= ((1<<PINA3)|(1<<PINA4)|(1<<PINA5)|(1<<PINA6)|(1<<PINA7));		// Required for DMM Board 2013
  DDRD = 0xFF;			//PortB, PortD, PortA0 und PortA1 als Ausgangen sein.
  DDRA |=(1<<PINA0);
  DDRA |=(1<<PINA1);
  DDRB = 0xFF;
 
/******************************************************************************************/
/*						Durchfuehrung des Hauptprogramms     							  */
/******************************************************************************************/
  LCD_Init();													
  _start();							
  _status();						
  _execute();						
  _finish();
  Backlight_Off();						// Backgrundlicht schliessen
  LCD_Clear();							// LCD clean
  // loop forever
  while(1);
  return 0;
}
/******************************************************************************************/
/*										Definition der Funktionen						  */
/******************************************************************************************/
void _start (void)
{ 
  Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);// Backgrundlicht aufmachen
/*							Inhalt in Buffer setzen            							  */
  LCD_PutString_P(PSTR("Project 04:\r\n"));
  LCD_PutString_P(PSTR("   3D Grabber arms\r\n"));
  LCD_PutString_P(PSTR("Members:\r\n"));
  LCD_PutString_P(PSTR("-Wang,Xueyuan\r\n"));
  LCD_PutString_P(PSTR("-Lin ,Xiangluan\r\n"));
  LCD_PutString_P(PSTR("-Jin ,Hao\r\n"));
  LCD_PutString_P(PSTR("-Shen,Bin\r\n"));
  LCD_PutString_P(PSTR("     Have Fun :)     "));
  LCD_Update();								// Inhalt in Buffer auf LCD zeigen.
  wait_joy_button();						// Auf Button warten
}
/******************************************************************************************/
/*				Dieser Teil geht nur um Greifen, d.h. zeigt LCD die Informationen nur ueber Status des Greifens von Motoren, weil Aktionen es gibt. */
/******************************************************************************************/
void _status (void)
{
  LCD_Clear();
  Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
  LCD_GotoXY(0,0);						// Position des Cursors(0,0)
  LCD_PutString_P(PSTR("-------Status:-------\r\n\n"));
  LCD_PutString_P(PSTR(" Motor Speed   Dir.\r\n"));
  LCD_PutString_P(PSTR(" M1 200pi rad/s Right\r\n"));
  LCD_PutString_P(PSTR(" M1 200pi rad/s Right\r\n"));
  LCD_PutString_P(PSTR(" M1 200pi rad/s Left\r\n"));
  LCD_PutString_P(PSTR(" M1 200pi rad/s Right\r\n"));
  LCD_PutString_P(PSTR(">>>>>>Show time>>>>>>"));
  LCD_Update();
  wait_joy_button();
}
/******************************************************************************************/
void _finish()
{
  LCD_Clear();
  Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
  LCD_GotoXY(0,4);
  LCD_PutString_P(PSTR("-------Finish--------"));
  LCD_Update();
  wait_joy_button();
}
/******************************************************************************************/
void _execute()
{
	//_execute_init();
//	Manipolator fuhrt die Aufgabe von Greifen durch, gleichzeitig zeigt Mikrokontroller Aktionen auf LCD.
	_grab_init_1();
		
	_show_action_1();
	_show_action_2();
		_grab_action_1();
	_show_action_3();
		_grab_action_2();
	_show_action_4();
		_grab_action_3();
	_show_action_5();
		_grab_action_4();
	_show_action_6();
//	Manipolator bewegt zurueck nach Originalsstelle
	_grab_init_2();
	
	_show_action_7();
		_grab_action_5();
	_show_action_8();
		_grab_action_6();
	_show_action_9();
		_grab_action_7();
	_show_action_10();
		_grab_action_8();
//	Manipolator geht nach Stelle B, um Objekt zu stellen.
	_put_init_1();
	
	_show_action_11();
		_put_action_1();
	_show_action_12();
		_put_action_2();
	_show_action_13();
		_put_action_3();
	_show_action_14();
		_put_action_4();
	_show_action_15();
//	Zurueck
	_put_init_2();

	_show_action_16();
		_put_action_5();
	_show_action_17();
		_put_action_6();
	_show_action_18();
		_put_action_7();
	_show_action_19();
		_put_action_8();
}
/******************************************************************************************/	
//	Definitonen jedes Schritts von Ausfuehrungsfunktionen.
void _grab_action_1()
{
	char Count;
	for (Count=0;Count<A_0;Count++)// Wenn MCU gibt ein "rise-edge" Signal Step von Motor aus, dreht der ein Zahn.
	{
		PORTD&=~(1<<PD6);		   // Deshalb gibt aus Mikrokontroller ein Rechtecksignal, desen Zaehle und Frequenz steuerbar sind	
		_delay_ms(speed_M0);	
		PORTD|=(1<<PD6);
		_delay_ms(speed_M0);
	}
}

void _grab_action_2()
{
	char Count;
	for (Count=0;Count<A_2;Count++)//
	{
		PORTB&=~(1<<PB1);
		_delay_ms(speed_M1);	
		PORTB|=(1<<PB1);
		_delay_ms(speed_M1);
	}
}

void _grab_action_3()
{
	char Count;
	for (Count=0;Count<A_1;Count++)//
	{
		PORTD&=~(1<<PD3);
	_delay_ms(speed_M2);	
		PORTD|=(1<<PD3);
	_delay_ms(speed_M2);
	}
}

void _grab_action_4()
{
	char Count;
	for (Count=0;Count<A_3;Count++)//
	{
		PORTA&=~(1<<PA1);
	_delay_ms(speed_M3);
		PORTA|=(1<<PA1);
	_delay_ms(speed_M3);
	}
}

void _grab_action_5()
{
	char Count;
	for (Count=0;Count<A_1;Count++)//
	{
		PORTA&=~(1<<PA1);
	_delay_ms(speed_M1);
		PORTA|=(1<<PA1);
	_delay_ms(speed_M1);
	}
}

void _grab_action_6()
{
	char Count;
	for (Count=0;Count<A_2;Count++)//
	{
		PORTB&=~(1<<PB1);
	_delay_ms(speed_M2);
		PORTB|=(1<<PB1);
	_delay_ms(speed_M2);
	}
}

void _grab_action_7()
{
	char Count;
	for (Count=0;Count<A_3;Count++)//
	{
		PORTD&=~(1<<PD3);
	_delay_ms(speed_M3);
		PORTD|=(1<<PD3);
	_delay_ms(speed_M3);
	}
}

void _grab_action_8()
{
	char Count;
	for (Count=0;Count<A_0;Count++)//
	{
		PORTD&=~(1<<PD6);
	_delay_ms(speed_M0);
		PORTD|=(1<<PD6);
	_delay_ms(speed_M0);
	}
}

void _put_action_1()
{
	char Count;
	for (Count=0;Count<B_0;Count++)//
	{
		PORTD&=~(1<<PD6);
	_delay_ms(speed_M0);	
		PORTD|=(1<<PD6);
	_delay_ms(speed_M0);
	}
}

void _put_action_2()
{
	char Count;
	for (Count=0;Count<B_2;Count++)//
	{
		PORTB&=~(1<<PB1);
	_delay_ms(speed_M1);
		PORTB|=(1<<PB1);
	_delay_ms(speed_M1);
	}
}

void _put_action_3()
{
	char Count;
	for (Count=0;Count<B_1;Count++)//
	{
		PORTD&=~(1<<PD3);
	_delay_ms(speed_M2);
		PORTD|=(1<<PD3);
	_delay_ms(speed_M2);
	}
}

void _put_action_4()
{
	char Count;
	for (Count=0;Count<B_3;Count++)//
	{
		PORTA&=~(1<<PA1);
	_delay_ms(speed_M3);
		PORTA|=(1<<PA1);
	_delay_ms(speed_M3);
	}
}

void _put_action_5()
{
	char Count;
	for (Count=0;Count<B_1;Count++)//
	{
		PORTA&=~(1<<PA1);
	_delay_ms(speed_M1);	
		PORTA|=(1<<PA1);
	_delay_ms(speed_M1);
	}
}

void _put_action_6()
{
	char Count;
	for (Count=0;Count<B_2;Count++)//
	{
		PORTB&=~(1<<PB1);
	_delay_ms(speed_M2);	
		PORTB|=(1<<PB1);
	_delay_ms(speed_M2);
	}
}

void _put_action_7()
{
	char Count;
	for (Count=0;Count<B_3;Count++)//
	{
		PORTD&=~(1<<PD3);
	_delay_ms(speed_M3);
		PORTD|=(1<<PD3);
	_delay_ms(speed_M3);
	}
}

void _put_action_8()
{
	char Count;
	for (Count=0;Count<B_0;Count++)//
	{
		PORTD&=~(1<<PD6);
	_delay_ms(speed_M0);
		PORTD|=(1<<PD6);
	_delay_ms(speed_M0);
	}
}
/******************************************************************************************/
//	
void _show_action_1 ()
{
	uint16_t emg_offset = 0;				
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);	
	LCD_Clear();							
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_1[emg_offset++]);
	lcd_frameupdate = 0xff;					
	LCD_GotoXY(1,1);
	LCD_PutString_P(PSTR("Start"));
	LCD_Update();							
	_delay_ms(20);
	wait_joy_button();
}

void _show_action_2 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_2[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_3 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_3[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_4 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_4[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_5 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_5[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_6 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_6[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(500);
}

void _show_action_7 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_7[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_8 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_8[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_9 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_9[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_10 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_10[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_11 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_11[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_12 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_12[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_13 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_13[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_14 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_14[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_15 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_15[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(500);
}

void _show_action_16 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_16[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_17 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_17[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_18 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_18[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}

void _show_action_19 ()
{
	uint16_t emg_offset = 0;
	uint8_t fb_x = 0;
	uint8_t fb_y = 0;
	Backlight_LED(BL_RED_ON | BL_GREEN_ON | BL_BLUE_ON);
	LCD_Clear();
	for (fb_y = 0; fb_y < 8; fb_y++)
	for (fb_x = 0; fb_x < 128; fb_x++)
	lcd_framebuffer[fb_y][fb_x] = pgm_read_byte(&_action_19[emg_offset++]);
	lcd_frameupdate = 0xff;
	LCD_Update();
	_delay_ms(20);
}
/******************************************************************************************/