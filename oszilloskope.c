#include <oszilloskope.h>

void fillDataLcdBuffer (uint8_t address, uint8_t data)
{
	dataLcdBuffer[address] = data;
}

void create_raster()
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
	LCD_Update();	
}

void create_wave()
{
	uint8_t i;
	for(i=0;i<100;i++)
	{
		LCD_DrawPixel(i,dataLcdBuffer[i],1);
	}
}

int signal_display (void)
{

	uint16_t  i,k;
	uint32_t endOfPeriod=0;
	uint8_t freqComplete=0;
	uint16_t  maxVoltage=0;

    adc_init();
	_delay_ms(4000);
	create_raster();
	create_wave();

	for(;;)
	{

		if (~PINA&(1<<PINA7)) && (timeDiv <= 120))
	    	timeDiv += 1;

		if (~PINA&(1<<PINA6)) && (timeDiv >= 1))
			timeDiv -= 1;


		if (~PINA&(1<<PINA5)) && (Ypos2 <= 60))
	    	Ypos2++;

		if (~PINA&(1<<PINA4)) && (Ypos2 >= -60))
			Ypos2--;

		if (~PINA&(1<<PINA4)& (1<<freeze))
			while (~PINA&(1<<PINA4) & (1<<freeze)); // It freezes the display to watch the wave.
		

			
//--------v
// Read 100 samples from analog input.
// From these 100 samples we can find the middle of the signal to be used as trigger...
// ... the Volts peak-to-peak and the frequency of the measured signal. 

		findZero = 0;
		upLimit = 0;
		lowLimit = 255;
		endOfPeriod = 0;
		freqComplete = 0;
		complete = FALSE;

		for (i=2; i<15000; i++)
		{
			ADCSRA |= (1 << ADSC); // Enable ADC
			loop_until_bit_is_set(ADCSRA, ADIF); // wait until conversion complete.
			ADCvalue = ADCH;

			//------------------------------------------------------v
			//  Find the end of the first period that is appeared on LCD.
			//  The beginning of the period is always the first pixel on the left of LCD.

            //Find the start of the period of the measured waveform. 
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 0))
				freqComplete = 1;
			
			//If you have found the start of the period, find the rise of the waveform.
			if((ADCvalue < trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 1))
				freqComplete = 2;

			//The next step is to find the start of the next period...
			if((ADCvalue > trigger) && (prevADCvalue < ADCvalue) && (freqComplete == 2))
			{
				freqComplete = 3; //Wow! we found the end of the first period. 
				//endOfPeriod = ((23000)/(i/2)); //Calculate the frequency that will be displayed on LCD.	
			}
			//------------------------------------------------------^

			prevADCvalue = ADCvalue; // Get a backup of the current ADC value.

			for(k=timeDiv;k>0;k--)   // Make a delay to see on LCD low-frequency waveforms. Normally, the minimum waveform that can be entirely displayed on LCD is 310 Hz. 
			{
				ADCSRA |= (1 << ADSC);   // Enable ADC
				loop_until_bit_is_set(ADCSRA, ADIF); // wait until conversion complete.
				ADCvalue = ADCH;
			}

			if (upLimit < ADCvalue)  // Find the higher voltage level of the input waveform.
				upLimit = ADCvalue;

			if (lowLimit > ADCvalue) // Find the lower voltage level of the input waveform.
				lowLimit = ADCvalue;


			maxVoltage = (upLimit * 2);  // Maximum voltage on ADC pin is calculated from upLimit register (255*2=510 = 5.10V).

			if (ADCvalue > 0)
			{
				voltage = ((upLimit-lowLimit)*2); //Get the Vpp and store it to "voltage" (Volts Peak-to-peak of inputed waveform).
				ADCvalue += 5;
				ADCvalue /= 5;
				ADCvalue += 2;
			}else
				ADCvalue = 2;
			
			position = ADCvalue + Ypos2 +5; 
			if ((position <= 63) && (position >= 0) && (i<100))  // Adjust Up-Down the wave on LCD.
				fillDataLcdBuffer(i,position);
			else
			if(i<100)
				fillDataLcdBuffer(i,0);

			if((i>100)&&(freqComplete==3))   //If i>100 and freqComplete=3 that means that our waveform is outside of LCD displaying area.
				break;  //Do not wait until i=15000. Terminate this loop ("for (i=0; i<15000; i++)")

		}

		if(upLimit != lowLimit)
			trigger = (((upLimit - lowLimit)/2)+ lowLimit); // Find the middle of the wave to be used as trigger.
		else
			trigger = upLimit;

//--------^

//--------- Print Volts peak-to-peak and frequency on display -------------

		create_wave();

//-------------------------------------------------------

		dataCounter = 0;
		complete = FALSE;
		freqComplete = 0;

		do  //Wait in this loop until you find again the start of the measured waveform.
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