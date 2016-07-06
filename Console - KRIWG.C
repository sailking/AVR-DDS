#include <stdio.h>

#define DATA_FRAME_LENGTH 40*4
volatile uint8_t data_frame_in[DATA_FRAME_LENGTH];
int main()
{
	printf("%d",sizeof(data_frame_in[0]));
   return 0;
}