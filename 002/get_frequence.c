
#include "uart.c"
#include "uart.h"
void get_frequence()
{
    uart_init1();
    uart_init0();

    sei();

    while(1)
    {
        if(data_ok == 1)
        {
            AD_freq = get_num_from_ascii(data_frame_in);
            data_ok = 0;
        }
    }
}

