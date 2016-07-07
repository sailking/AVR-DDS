#include <oszilloskope.h>

void fillDataLcdBuffer (unsigned char address, unsigned char data)
{
	dataLcdBuffer[address] = data;
}

int main (void)
{

	unsigned int  i,k;
	unsigned long endOfPeriod=0;
	unsigned char freqComplete=0;
	unsigned int  maxVoltage=0;

    adc_init();
	_delay_ms(4000);
	createWave();

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

		restoreRaster();
		createWave();

		line=3;                    //Show the DC voltage
		column=109;
		gLCDgotoXY(line,column);
		hex2Ascii(maxVoltage,hex2asciiBuffer);
		GLCD_WriteChar(hex2asciiBuffer[2]);
		sendDataOnLCD(0b01000000); //Print one dot character on LCD (only 1 column length).
		sendDataOnLCD(0);
		GLCD_WriteChar(hex2asciiBuffer[1]);
		GLCD_WriteChar(hex2asciiBuffer[0]);
		sendDataOnLCD(0x00);
		GLCD_WriteChar('V');

		line=4;                    //Show the Vpp (Volts peak-to-peak).
		column=109;
		gLCDgotoXY(line,column);
		hex2Ascii(voltage,hex2asciiBuffer);
		GLCD_WriteChar(hex2asciiBuffer[2]);
		sendDataOnLCD(0b01000000); //Print one dot character on LCD (only 1 column length).
		sendDataOnLCD(0);
		GLCD_WriteChar(hex2asciiBuffer[1]);
		GLCD_WriteChar(hex2asciiBuffer[0]);
		sendDataOnLCD(0x00);
		GLCD_WriteChar('V');

		line=6;                   // Go to 6th line on LCD.
		column=122; 
		gLCDgotoXY(line,column);

		if(timeDiv == 0)          // If 'timeDiv' = 0 then the frequency in to the 'endOfPeriod' variable is real.
			GLCD_WriteChar(0x5c); // So, print on LCD the "Play" character. '0x5c' is the '\' character ("Play" symbol).
		else
			GLCD_WriteChar(']');  // If 'timeDiv' is not zero that means we have shrink the waveform. So, print the "Pause" symbol on LCD. 
		
		if(timeDiv == 0)		  // If 'timeDiv' = 0 then the frequency in to the 'endOfPeriod' variable is real.
		{
			line=7;               // Go to 7th line and print on LCD the waveform's frequency
			column=102;
			gLCDgotoXY(line,column);
			if(endOfPeriod <10000) // The maximum frequency that can be displayed on LCD is 9999 Hz.
			{
				gLCDgotoXY(line,column);
				hex2Ascii(endOfPeriod,hex2asciiBuffer);
				GLCD_WriteChar(hex2asciiBuffer[3]);
				GLCD_WriteChar(hex2asciiBuffer[2]);
				GLCD_WriteChar(hex2asciiBuffer[1]);
				GLCD_WriteChar(hex2asciiBuffer[0]);	
			}
			
		}                    // if 'timeDiv' > 0 do not update the frequency. Keep the previous frequency value instead. 

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