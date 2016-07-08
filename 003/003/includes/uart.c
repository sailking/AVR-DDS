#include "uart.h"

volatile uint8_t data_frame_out[DATA_FRAME_LENGTH] = {"Die Daten werden als serieller digitaler Datenstrom mit einem fixen Rahmen übertragen, der aus einem Start-Bit, fünf bis maximal neun Datenbits."}; //Dummydaten
volatile uint8_t data_frame_in[DATA_FRAME_LENGTH];
const uint8_t device_address = 3;
volatile uint8_t data_ok = 0;

ISR(USART0_RX_vect)
{
	char udr_buffer = UDR0;
	static uint8_t uart_rx_cnt = 0;
	static uint8_t address = 0;
	static uint8_t checksum = 0;
	static uint8_t checksum_in = 0;

	if(udr_buffer != 13) //CR
	{
		if(uart_rx_cnt == 1)#include "uart.h"

volatile uint8_t data_frame_out[DATA_FRAME_LENGTH] = {"Die Daten werden als serieller digitaler Datenstrom mit einem fixen Rahmen übertragen, der aus einem Start-Bit, fünf bis maximal neun Datenbits."}; //Dummydaten
volatile uint8_t data_frame_in[DATA_FRAME_LENGTH];
const uint8_t device_address = 3;
volatile uint8_t data_ok = 0;

ISR(USART0_RX_vect)
{
	char udr_buffer = UDR0;
	static uint8_t uart_rx_cnt = 0;
	static uint8_t address = 0;
	static uint8_t checksum = 0;
	static uint8_t checksum_in = 0;

	if(udr_buffer != 13) //CR
	{
		if(uart_rx_cnt == 1)
		{
			address = udr_buffer;
		}
		else if(uart_rx_cnt >= 2 && uart_rx_cnt < DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES && address == device_address)
		{
			data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES] = udr_buffer;
			checksum ^= data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES];
		}
		else if(uart_rx_cnt == DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES)
		{
			checksum_in = udr_buffer;
		}
		uart_rx_cnt++;
	}
	else //CR received
	{
		if(checksum == checksum_in)
		{
			data_ok = 1;
		}
		uart_rx_cnt = 0;
		address = 0;
		checksum_in = 0xBB;
		checksum = 0;
	}
}


ISR(USART0_TX_vect)
{
	static uint8_t checksum = 0;
	static uint8_t byte_cnt = 0;
	if(byte_cnt < DATA_FRAME_LENGTH)
	{
		UDR0 = (data_frame_out[byte_cnt]);
		checksum ^= data_frame_out[byte_cnt];
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH)
	{
		UDR0 = checksum;
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH + 1)
	{
		UDR0 = 13;
		byte_cnt++;
	}
	else
	{
		checksum = 0;
		byte_cnt = 0;
		data_ok = 0;
	}
}

ISR(USART1_RX_vect)
{
char udr_buffer = UDR1;
static uint8_t uart_rx_cnt = 0;
static uint8_t address = 0;
static uint8_t checksum = 0;
static uint8_t checksum_in = 0;
if(udr_buffer != 13) //CR
{
	if(uart_rx_cnt == 1)
	{
		address = udr_buffer;
	}
	else if(uart_rx_cnt >= 2 && uart_rx_cnt < DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES && address == device_address)
	{
		data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES] = udr_buffer;
		checksum ^= data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES];
	}
	else if(uart_rx_cnt == DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES)
	{
		checksum_in = udr_buffer;
	}
	uart_rx_cnt++;
}
else //CR received
{
	if(checksum == checksum_in && address == device_address)
	{
		data_ok = 1;
	}
	uart_rx_cnt = 0;
	address = 0;
	checksum_in = 0xBB;
	checksum = 0;
}
}


ISR(USART1_TX_vect)
{
	static uint8_t checksum = 0;
	static uint8_t byte_cnt = 0;
	if(byte_cnt < DATA_FRAME_LENGTH)
	{
		UDR1 = (data_frame_out[byte_cnt]);
		checksum ^= data_frame_out[byte_cnt];
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH)
	{
		UDR1 = checksum;
		byte_cnt++;#include "uart.h"

volatile uint8_t data_frame_out[DATA_FRAME_LENGTH] = {"Die Daten werden als serieller digitaler Datenstrom mit einem fixen Rahmen übertragen, der aus einem Start-Bit, fünf bis maximal neun Datenbits."}; //Dummydaten
volatile uint8_t data_frame_in[DATA_FRAME_LENGTH];
const uint8_t device_address = 3;
volatile uint8_t data_ok = 0;

ISR(USART0_RX_vect)
{
	char udr_buffer = UDR0;
	static uint8_t uart_rx_cnt = 0;
	static uint8_t address = 0;
	static uint8_t checksum = 0;
	static uint8_t checksum_in = 0;

	if(udr_buffer != 13) //CR
	{
		if(uart_rx_cnt == 1)
		{
			address = udr_buffer;
		}
		else if(uart_rx_cnt >= 2 && uart_rx_cnt < DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES && address == device_address)
		{
			data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES] = udr_buffer;
			checksum ^= data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES];
		}
		else if(uart_rx_cnt == DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES)
		{
			checksum_in = udr_buffer;
		}
		uart_rx_cnt++;
	}
	else //CR received
	{
		if(checksum == checksum_in)
		{
			data_ok = 1;
		}
		uart_rx_cnt = 0;
		address = 0;
		checksum_in = 0xBB;
		checksum = 0;
	}
}


ISR(USART0_TX_vect)
{
	static uint8_t checksum = 0;
	static uint8_t byte_cnt = 0;
	if(byte_cnt < DATA_FRAME_LENGTH)
	{
		UDR0 = (data_frame_out[byte_cnt]);
		checksum ^= data_frame_out[byte_cnt];
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH)
	{
		UDR0 = checksum;
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH + 1)
	{
		UDR0 = 13;
		byte_cnt++;
	}
	else
	{
		checksum = 0;
		byte_cnt = 0;
		data_ok = 0;
	}
}

ISR(USART1_RX_vect)
{
char udr_buffer = UDR1;
static uint8_t uart_rx_cnt = 0;
static uint8_t address = 0;
static uint8_t checksum = 0;
static uint8_t checksum_in = 0;
if(udr_buffer != 13) //CR
{
	if(uart_rx_cnt == 1)
	{
		address = udr_buffer;
	}
	else if(uart_rx_cnt >= 2 && uart_rx_cnt < DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES && address == device_address)
	{
		data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES] = udr_buffer;
		checksum ^= data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES];
	}
	else if(uart_rx_cnt == DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES)
	{
		checksum_in = udr_buffer;
	}
	uart_rx_cnt++;
}
else //CR received
{
	if(checksum == checksum_in && address == device_address)
	{
		data_ok = 1;
	}
	uart_rx_cnt = 0;
	address = 0;
	checksum_in = 0xBB;
	checksum = 0;
}
}


ISR(USART1_TX_vect)
{
	static uint8_t checksum = 0;
	static uint8_t byte_cnt = 0;
	if(byte_cnt < DATA_FRAME_LENGTH)
	{
		UDR1 = (data_frame_out[byte_cnt]);
		checksum ^= data_frame_out[byte_cnt];
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH)
	{
		UDR1 = checksum;
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH + 1)
	{
		UDR1 = 13;
		byte_cnt++;
	}
	else
	{
		checksum = 0;
		byte_cnt = 0;
		data_ok = 0;
	}
}

uint8_t uart0_tx_frame()
{
	uart_tx_byte(PREAMBLE);
	uart_tx_byte(device_address);
	return 1;
}


uint8_t uart1_tx_frame()
{
	uart1_tx_byte(PREAMBLE);
	uart1_tx_byte(device_address);
	return 1;
}

void uart_init0 ()
{
	UCSR0A |= (0<<U2X0) | (0<<MPCM0);
	UCSR0B = (1<<RXCIE0) | (1<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
	UCSR0C = (0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<< UCSZ01) | (1<< UCSZ00) | (0<< UCPOL0);
	UBRR0H = 0;
	UBRR0L = BAUD_VALUE; //9600 = 103 16 MHz
	
}

void uart_init1 ()
{
	UCSR1A |= (0<<U2X1) | (0<<MPCM1);
	UCSR1B = (1<<RXCIE1) | (1<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
	UCSR1C = (0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<< UCSZ11) | (1<< UCSZ10) | (0<< UCPOL1);
	UBRR1H = 0;
	UBRR1L = BAUD_VALUE; //9600 = 103 16 MHz
	
}

uint16_t get_num_from_ascii(char* in)
{
	uint8_t digcnt = 0;
	for(uint8_t i=0; i<6;i++)
	{
		if(in[i] != 0)
		{
			digcnt++;
		}
	}
	uint16_t num = 0;
	switch (digcnt)
	{
		case 1: num = in[0] - 48;
		break;
		case 2: num = in[1] - 48;
				num = num+((in[0]-48)*10);
		break;
		case 3: num = in[2] - 48;
				num = num+((in[1]-48)*10);
				num = num+((in[0]-48)*100);
		break;
		case 4: num = in[3] - 48;
				num = num+((in[2]-48)*10);
				num = num+((in[1]-48)*100);
				num = num+((in[0]-48)*1000);
		
		break;
		case 5: num = in[4] - 48;
				num = num+((in[3]-48)*10);
				num = num+((in[2]-48)*100);
				num = num+((in[1]-48)*1000);
				num = num+((in[0]-48)*10000);
	}		
	return num;
}

uint8_t get_h_from_ascii(char* in)
{
	uint8_t h = in[1] - 48;
	h = h+((in[0]-48)*10);
	return h;
}

uint8_t get_min_from_ascii(char* in)
{
	uint8_t min = in[3] - 48;
	min = min+((in[2]-48)*10);
	return min;
}


void uart_tx_byte(unsigned char c)
{
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich */
	{
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UDR0 = c;
	}
}

void uart1_tx_byte(unsigned char c)
{
	while (!(UCSR1A & (1<<UDRE1)))  /* warten bis Senden moeglich */
	{
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UDR1 = c;
	}
}

uint8_t uart_tx_string(char* string) 
{
	uint8_t cnt = 0;
	while(*string) 
	{
		uart_tx_byte(*string);
		string++;
		cnt++;
	}
	return cnt;
}

void uart_tx_command(uint16_t com)
{
	uart_tx_byte(com);
	com >>= 8;
	uart_tx_byte(com);
}


void uart_tx_hex8(uint8_t data)
{	
	uint8_t buf = data;
	uart_tx_byte('0');
	uart_tx_byte('x');
	buf = data >> 4;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
	buf = data & 0b00001111;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
}

void uart_tx_hex16(uint16_t data)
{	
	uint16_t buf = data;
	uart_tx_byte('0');
	uart_tx_byte('x');
	buf = (uint16_t)data >> 12;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data >> 8;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data >> 4;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
}


void uart_tx_num16(uint16_t number)
{
	uint8_t digit1 = 0;
	uint8_t digit2 = 0;
	uint8_t digit3 = 0;
	uint8_t digit4 = 0;
    while (number >= 10000) 
	{
        digit1++;
        number -= 10000;
    }
    while (number >= 1000) 
	{
        digit2++;
        number -= 1000;
    }
	while (number >= 100) 
	{
        digit3++;
        number -= 100;
    }
    while (number >= 10) 
	{
        digit4++;
        number -= 10;
    }
	uart_tx_byte(48+digit1);
	uart_tx_byte(48+digit2);
	uart_tx_byte(48+digit3);
	uart_tx_byte(48+digit4);
	uart_tx_byte(48+number);
}

void uart_tx_number2(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero,uint8_t decpoint) //
{
	uint8_t digits[5] = {0,0,0,0,0};
	uint8_t var_dig = 0;
	uint8_t digit_cnt_buf = digit_cnt;
	while (number >= 10000)
	{
		digits[4]++;
		number -= 10000;
		var_dig |= (1<<5);
	}
	while (number >= 1000)
	{
		digits[3]++;
		number -= 1000;
		var_dig |= (1<<4);
	}
	while (number >= 100)
	{
		digits[2]++;
		number -= 100;
		var_dig |= (1<<3);
	}
	while (number >= 10)
	{
		digits[1]++;
		number -= 10;
		var_dig |= (1<<2);
	}
	digits[0] = number;
	if(leading_zero == 0 || leading_zero == 1)
	{
		if(var_dig & (1<<5))
		{
			digit_cnt = 5;
		}
		else if(var_dig & (1<<4))
		{
			digit_cnt = 4;
		}
		else if(var_dig & (1<<3))
		{
			digit_cnt = 3;
		}
		else if(var_dig & (1<<2))
		{
			digit_cnt = 2;
		}
		else
		{
			digit_cnt = 1;
		}
		if(leading_zero == 1)
		{
			for(uint8_t i = digit_cnt_buf-digit_cnt; i>0;i--)
			{
				uart_tx_string(" ");
			}
		}
	}
	while(digit_cnt > 0)
	{
		uart_tx_byte(48+digits[digit_cnt-1]);
		if(digit_cnt == (5-decpoint))
		{
			uart_tx_string(".");
		}
		digit_cnt--;
	}
}

void uart_tx_number(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero) //
{
	uint8_t digits[5] = {0,0,0,0,0};
	uint8_t var_dig = 0;
	uint8_t digit_cnt_buf = digit_cnt;
    while (number >= 10000) 
	{
        digits[4]++;
        number -= 10000;
		var_dig |= (1<<5);
    }
    while (number >= 1000) 
	{
        digits[3]++;
        number -= 1000;
		var_dig |= (1<<4);
    }
	while (number >= 100) 
	{
        digits[2]++;
        number -= 100;
		var_dig |= (1<<3);
    }
    while (number >= 10) 
	{
        digits[1]++;
        number -= 10;
		var_dig |= (1<<2);
    }
	digits[0] = number;
	if(leading_zero == 0 || leading_zero == 1)
	{
		if(var_dig & (1<<5))
		{
			digit_cnt = 5;
		}
		else if(var_dig & (1<<4))
		{
			digit_cnt = 4;
		}
		else if(var_dig & (1<<3))
		{
			digit_cnt = 3;
		}
		else if(var_dig & (1<<2))
		{
			digit_cnt = 2;
		}
		else
		{
			digit_cnt = 1;
		}
		if(leading_zero == 1)
		{
			for(uint8_t i = digit_cnt_buf-digit_cnt; i>0;i--)
			{
				uart_tx_string(" ");
			}
		}
	}
	while(digit_cnt > 0)
	{
		uart_tx_byte(48+digits[digit_cnt-1]);
		digit_cnt--;
	}
}



void uart_tx_time(uint8_t s, uint8_t m, uint8_t h,uint16_t d)
{
	if(d)
	{
		uart_tx_number(d,5,0);
		uart_tx_string(".");
	}
	uart_tx_number(h,2,2);
	uart_tx_string(":");
	uart_tx_number(m,2,2);
	uart_tx_string(":");
	uart_tx_number(s,2,2);
}

void uart_tx_temperature(int16_t temp,uint8_t unit)
{
	if(temp >= 0)
	{
		uint8_t digit = 0;
		while (temp >= 100) 
		{
	        digit += 10;
	        temp -= 100;
	    }
	    while (temp >= 10) 
		{
	        digit++;
	        temp -= 10;
	    }
		uart_tx_string("+");
		uart_tx_number(digit,2,2);
		uart_tx_string(".");
		uart_tx_byte(48+temp);
		if(unit)
		uart_tx_string("'C");
	}
	else
	{
		uint8_t digit_ = 0;
		while (temp <= -100) 
		{
	        digit_ += 10;
	        temp += 100;
	    }
	    while (temp <= -10) 
		{
	        digit_++;
	        temp += 10;
	    }
		uart_tx_string("-");
		uart_tx_number(digit_,2,2);
		uart_tx_string(".");
		uart_tx_byte(48 + ~temp);
		if(unit)
		uart_tx_string("'C");
	}
}

void uart_tx_line_break()
{
	uart_tx_byte('\r');
	uart_tx_byte('\n');
}
void print_string_flash(const char* string)
{
	char c;
	for(;;)
	{	c=pgm_read_byte(string++);
		if (!c)
			break;
		uart_tx_byte(c);
	}
}


	}
	else if(byte_cnt == DATA_FRAME_LENGTH + 1)
	{
		UDR1 = 13;
		byte_cnt++;
	}
	else
	{
		checksum = 0;
		byte_cnt = 0;
		data_ok = 0;
	}
}

uint8_t uart0_tx_frame()
{
	uart_tx_byte(PREAMBLE);
	uart_tx_byte(device_address);
	return 1;
}


uint8_t uart1_tx_frame()
{
	uart1_tx_byte(PREAMBLE);
	uart1_tx_byte(device_address);
	return 1;
}

void uart_init0 ()
{
	UCSR0A |= (0<<U2X0) | (0<<MPCM0);
	UCSR0B = (1<<RXCIE0) | (1<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
	UCSR0C = (0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<< UCSZ01) | (1<< UCSZ00) | (0<< UCPOL0);
	UBRR0H = 0;
	UBRR0L = BAUD_VALUE; //9600 = 103 16 MHz
	
}

void uart_init1 ()
{
	UCSR1A |= (0<<U2X1) | (0<<MPCM1);
	UCSR1B = (1<<RXCIE1) | (1<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
	UCSR1C = (0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<< UCSZ11) | (1<< UCSZ10) | (0<< UCPOL1);
	UBRR1H = 0;
	UBRR1L = BAUD_VALUE; //9600 = 103 16 MHz
	
}

uint16_t get_num_from_ascii(char* in)
{
	uint8_t digcnt = 0;
	for(uint8_t i=0; i<6;i++)
	{
		if(in[i] != 0)
		{
			digcnt++;
		}
	}
	uint16_t num = 0;
	switch (digcnt)
	{
		case 1: num = in[0] - 48;
		break;
		case 2: num = in[1] - 48;
				num = num+((in[0]-48)*10);
		break;
		case 3: num = in[2] - 48;
				num = num+((in[1]-48)*10);
				num = num+((in[0]-48)*100);
		break;
		case 4: num = in[3] - 48;
				num = num+((in[2]-48)*10);
				num = num+((in[1]-48)*100);
				num = num+((in[0]-48)*1000);
		
		break;
		case 5: num = in[4] - 48;
				num = num+((in[3]-48)*10);
				num = num+((in[2]-48)*100);
				num = num+((in[1]-48)*1000);
				num = num+((in[0]-48)*10000);
	}		
	return num;
}

uint8_t get_h_from_ascii(char* in)
{
	uint8_t h = in[1] - 48;
	h = h+((in[0]-48)*10);
	return h;
}

uint8_t get_min_from_ascii(char* in)
{
	uint8_t min = in[3] - 48;
	min = min+((in[2]-48)*10);
	return min;
}


void uart_tx_byte(unsigned char c)
{
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich */
	{
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UDR0 = c;
	}
}

void uart1_tx_byte(unsigned char c)
{
	while (!(UCSR1A & (1<<UDRE1)))  /* warten bis Senden moeglich */
	{
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UDR1 = c;
	}
}

uint8_t uart_tx_string(char* string) 
{
	uint8_t cnt = 0;
	while(*string) 
	{
		uart_tx_byte(*string);
		string++;
		cnt++;
	}
	return cnt;
}

void uart_tx_command(uint16_t com)
{
	uart_tx_byte(com);
	com >>= 8;
	uart_tx_byte(com);
}


void uart_tx_hex8(uint8_t data)
{	
	uint8_t buf = data;
	uart_tx_byte('0');
	uart_tx_byte('x');
	buf = data >> 4;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
	buf = data & 0b00001111;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
}

void uart_tx_hex16(uint16_t data)
{	
	uint16_t buf = data;
	uart_tx_byte('0');
	uart_tx_byte('x');
	buf = (uint16_t)data >> 12;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data >> 8;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data >> 4;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
}


void uart_tx_num16(uint16_t number)
{
	uint8_t digit1 = 0;
	uint8_t digit2 = 0;
	uint8_t digit3 = 0;
	uint8_t digit4 = 0;
    while (number >= 10000) 
	{
        digit1++;
        number -= 10000;
    }
    while (number >= 1000) 
	{
        digit2++;
        number -= 1000;
    }
	while (number >= 100) 
	{
        digit3++;
        number -= 100;
    }
    while (number >= 10) 
	{
        digit4++;
        number -= 10;
    }
	uart_tx_byte(48+digit1);
	uart_tx_byte(48+digit2);
	uart_tx_byte(48+digit3);
	uart_tx_byte(48+digit4);
	uart_tx_byte(48+number);
}

void uart_tx_number2(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero,uint8_t decpoint) //
{
	uint8_t digits[5] = {0,0,0,0,0};
	uint8_t var_dig = 0;
	uint8_t digit_cnt_buf = digit_cnt;
	while (number >= 10000)
	{
		digits[4]++;
		number -= 10000;
		var_dig |= (1<<5);
	}
	while (number >= 1000)
	{
		digits[3]++;
		number -= 1000;
		var_dig |= (1<<4);
	}
	while (number >= 100)
	{
		digits[2]++;
		number -= 100;
		var_dig |= (1<<3);
	}
	while (number >= 10)
	{
		digits[1]++;
		number -= 10;
		var_dig |= (1<<2);
	}
	digits[0] = number;
	if(leading_zero == 0 || leading_zero == 1)
	{
		if(var_dig & (1<<5))
		{
			digit_cnt = 5;
		}
		else if(var_dig & (1<<4))
		{
			digit_cnt = 4;
		}
		else if(var_dig & (1<<3))
		{
			digit_cnt = 3;
		}
		else if(var_dig & (1<<2))
		{
			digit_cnt = 2;
		}
		else
		{
			digit_cnt = 1;
		}
		if(leading_zero == 1)
		{
			for(uint8_t i = digit_cnt_buf-digit_cnt; i>0;i--)
			{
				uart_tx_string(" ");
			}
		}
	}
	while(digit_cnt > 0)
	{
		uart_tx_byte(48+digits[digit_cnt-1]);
		if(digit_cnt == (5-decpoint))
		{
			uart_tx_string(".");
		}
		digit_cnt--;
	}
}

void uart_tx_number(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero) //
{
	uint8_t digits[5] = {0,0,0,0,0};
	uint8_t var_dig = 0;
	uint8_t digit_cnt_buf = digit_cnt;
    while (number >= 10000) 
	{
        digits[4]++;
        number -= 10000;
		var_dig |= (1<<5);
    }
    while (number >= 1000) 
	{
        digits[3]++;
        number -= 1000;
		var_dig |= (1<<4);
    }
	while (number >= 100) 
	{
        digits[2]++;
        number -= 100;
		var_dig |= (1<<3);
    }
    while (number >= 10) 
	{
        digits[1]++;
        number -= 10;
		var_dig |= (1<<2);
    }
	digits[0] = number;
	if(leading_zero == 0 || leading_zero == 1)
	{
		if(var_dig & (1<<5))
		{
			digit_cnt = 5;
		}
		else if(var_dig & (1<<4))
		{
			digit_cnt = 4;
		}
		else if(var_dig & (1<<3))
		{
			digit_cnt = 3;
		}
		else if(var_dig & (1<<2))
		{
			digit_cnt = 2;
		}
		else
		{
			digit_cnt = 1;
		}
		if(leading_zero == 1)
		{
			for(uint8_t i = digit_cnt_buf-digit_cnt; i>0;i--)
			{
				uart_tx_string(" ");
			}
		}
	}
	while(digit_cnt > 0)
	{
		uart_tx_byte(48+digits[digit_cnt-1]);
		digit_cnt--;
	}
}



void uart_tx_time(uint8_t s, uint8_t m, uint8_t h,uint16_t d)
{
	if(d)
	{
		uart_tx_number(d,5,0);
		uart_tx_string(".");
	}
	uart_tx_number(h,2,2);
	uart_tx_string(":");
	uart_tx_number(m,2,2);
	uart_tx_string(":");
	uart_tx_number(s,2,2);
}

void uart_tx_temperature(int16_t temp,uint8_t unit)
{
	if(temp >= 0)
	{
		uint8_t digit = 0;
		while (temp >= 100) 
		{
	        digit += 10;
	        temp -= 100;
	    }
	    while (temp >= 10) 
		{
	        digit++;
	        temp -= 10;
	    }
		uart_tx_string("+");
		uart_tx_number(digit,2,2);
		uart_tx_string(".");
		uart_tx_byte(48+temp);
		if(unit)
		uart_tx_string("'C");
	}
	else
	{
		uint8_t digit_ = 0;
		while (temp <= -100) 
		{
	        digit_ += 10;
	        temp += 100;
	    }
	    while (temp <= -10) 
		{
	        digit_++;
	        temp += 10;
	    }
		uart_tx_string("-");
		uart_tx_number(digit_,2,2);
		uart_tx_string(".");
		uart_tx_byte(48 + ~temp);
		if(unit)
		uart_tx_string("'C");
	}
}

void uart_tx_line_break()
{
	uart_tx_byte('\r');
	uart_tx_byte('\n');
}
void print_string_flash(const char* string)
{
	char c;
	for(;;)
	{	c=pgm_read_byte(string++);
		if (!c)
			break;
		uart_tx_byte(c);
	}
}


		{
			address = udr_buffer;
		}
		else if(uart_rx_cnt >= 2 && uart_rx_cnt < DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES && address == device_address)
		{
			data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES] = udr_buffer;
			checksum ^= data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES];
		}
		else if(uart_rx_cnt == DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES)
		{
			checksum_in = udr_buffer;
		}
		uart_rx_cnt++;
	}
	else //CR received
	{
		if(checksum == checksum_in)
		{
			data_ok = 1;
		}
		uart_rx_cnt = 0;
		address = 0;
		checksum_in = 0xBB;
		checksum = 0;
	}
}


ISR(USART0_TX_vect)
{
	static uint8_t checksum = 0;
	static uint8_t byte_cnt = 0;
	if(byte_cnt < DATA_FRAME_LENGTH)
	{
		UDR0 = (data_frame_out[byte_cnt]);
		checksum ^= data_frame_out[byte_cnt];
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH)
	{
		UDR0 = checksum;
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH + 1)
	{
		UDR0 = 13;
		byte_cnt++;
	}
	else
	{
		checksum = 0;
		byte_cnt = 0;
		data_ok = 0;
	}
}

ISR(USART1_RX_vect)
{
char udr_buffer = UDR1;
static uint8_t uart_rx_cnt = 0;
static uint8_t address = 0;
static uint8_t checksum = 0;
static uint8_t checksum_in = 0;
if(udr_buffer != 13) //CR
{
	if(uart_rx_cnt == 1)
	{
		address = udr_buffer;
	}
	else if(uart_rx_cnt >= 2 && uart_rx_cnt < DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES && address == device_address)
	{
		data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES] = udr_buffer;
		checksum ^= data_frame_in[uart_rx_cnt-NUMBER_OF_START_BYTES];
	}
	else if(uart_rx_cnt == DATA_FRAME_LENGTH + NUMBER_OF_START_BYTES)
	{
		checksum_in = udr_buffer;
	}
	uart_rx_cnt++;
}
else //CR received
{
	if(checksum == checksum_in)
	{
		data_ok = 1;
	}
	uart_rx_cnt = 0;
	address = 0;
	checksum_in = 0xBB;
	checksum = 0;
}
}


ISR(USART1_TX_vect)
{
	static uint8_t checksum = 0;
	static uint8_t byte_cnt = 0;
	if(byte_cnt < DATA_FRAME_LENGTH)
	{
		UDR1 = (data_frame_out[byte_cnt]);
		checksum ^= data_frame_out[byte_cnt];
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH)
	{
		UDR1 = checksum;
		byte_cnt++;
	}
	else if(byte_cnt == DATA_FRAME_LENGTH + 1)
	{
		UDR1 = 13;
		byte_cnt++;
	}
	else
	{
		checksum = 0;
		byte_cnt = 0;
		data_ok = 0;
	}
}

uint8_t uart0_tx_frame()
{
	uart_tx_byte(PREAMBLE);
	uart_tx_byte(device_address);
	return 1;
}


uint8_t uart1_tx_frame()
{
	uart1_tx_byte(PREAMBLE);
	uart1_tx_byte(device_address);
	return 1;
}

void uart_init0 ()
{
	UCSR0A |= (0<<U2X0) | (0<<MPCM0);
	UCSR0B = (1<<RXCIE0) | (1<<TXCIE0) | (0<<UDRIE0) | (1<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
	UCSR0C = (0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<< UCSZ01) | (1<< UCSZ00) | (0<< UCPOL0);
	UBRR0H = 0;
	UBRR0L = BAUD_VALUE; //9600 = 103 16 MHz
	
}

void uart_init1 ()
{
	UCSR1A |= (0<<U2X1) | (0<<MPCM1);
	UCSR1B = (1<<RXCIE1) | (1<<TXCIE1) | (0<<UDRIE1) | (1<<RXEN1) | (1<<TXEN1) | (0<<UCSZ12) | (0<<RXB81) | (0<<TXB81);
	UCSR1C = (0<<UMSEL11) | (0<<UMSEL10) | (0<<UPM11) | (0<<UPM10) | (0<<USBS1) | (1<< UCSZ11) | (1<< UCSZ10) | (0<< UCPOL1);
	UBRR1H = 0;
	UBRR1L = BAUD_VALUE; //9600 = 103 16 MHz
	
}

uint16_t get_num_from_ascii(char* in)
{
	uint8_t digcnt = 0;
	for(uint8_t i=0; i<6;i++)
	{
		if(in[i] != 0)
		{
			digcnt++;
		}
	}
	uint16_t num = 0;
	switch (digcnt)
	{
		case 1: num = in[0] - 48;
		break;
		case 2: num = in[1] - 48;
				num = num+((in[0]-48)*10);
		break;
		case 3: num = in[2] - 48;
				num = num+((in[1]-48)*10);
				num = num+((in[0]-48)*100);
		break;
		case 4: num = in[3] - 48;
				num = num+((in[2]-48)*10);
				num = num+((in[1]-48)*100);
				num = num+((in[0]-48)*1000);
		
		break;
		case 5: num = in[4] - 48;
				num = num+((in[3]-48)*10);
				num = num+((in[2]-48)*100);
				num = num+((in[1]-48)*1000);
				num = num+((in[0]-48)*10000);
	}		
	return num;
}

uint8_t get_h_from_ascii(char* in)
{
	uint8_t h = in[1] - 48;
	h = h+((in[0]-48)*10);
	return h;
}

uint8_t get_min_from_ascii(char* in)
{
	uint8_t min = in[3] - 48;
	min = min+((in[2]-48)*10);
	return min;
}


void uart_tx_byte(unsigned char c)
{
	while (!(UCSR0A & (1<<UDRE0)))  /* warten bis Senden moeglich */
	{
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UDR0 = c;
	}
}

void uart1_tx_byte(unsigned char c)
{
	while (!(UCSR1A & (1<<UDRE1)))  /* warten bis Senden moeglich */
	{
	}
	ATOMIC_BLOCK(ATOMIC_FORCEON)
	{
		UDR1 = c;
	}
}

uint8_t uart_tx_string(char* string) 
{
	uint8_t cnt = 0;
	while(*string) 
	{
		uart_tx_byte(*string);
		string++;
		cnt++;
	}
	return cnt;
}

void uart_tx_command(uint16_t com)
{
	uart_tx_byte(com);
	com >>= 8;
	uart_tx_byte(com);
}


void uart_tx_hex8(uint8_t data)
{	
	uint8_t buf = data;
	uart_tx_byte('0');
	uart_tx_byte('x');
	buf = data >> 4;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
	buf = data & 0b00001111;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
}

void uart_tx_hex16(uint16_t data)
{	
	uint16_t buf = data;
	uart_tx_byte('0');
	uart_tx_byte('x');
	buf = (uint16_t)data >> 12;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data >> 8;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data >> 4;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}

	buf = (uint16_t)data;
	buf = buf & 0x000F;
	if(buf < 10)
	{
		uart_tx_byte(48+buf);
	}
	else
	{
		uart_tx_byte(65+(buf-10));
	}
}


void uart_tx_num16(uint16_t number)
{
	uint8_t digit1 = 0;
	uint8_t digit2 = 0;
	uint8_t digit3 = 0;
	uint8_t digit4 = 0;
    while (number >= 10000) 
	{
        digit1++;
        number -= 10000;
    }
    while (number >= 1000) 
	{
        digit2++;
        number -= 1000;
    }
	while (number >= 100) 
	{
        digit3++;
        number -= 100;
    }
    while (number >= 10) 
	{
        digit4++;
        number -= 10;
    }
	uart_tx_byte(48+digit1);
	uart_tx_byte(48+digit2);
	uart_tx_byte(48+digit3);
	uart_tx_byte(48+digit4);
	uart_tx_byte(48+number);
}

void uart_tx_number2(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero,uint8_t decpoint) //
{
	uint8_t digits[5] = {0,0,0,0,0};
	uint8_t var_dig = 0;
	uint8_t digit_cnt_buf = digit_cnt;
	while (number >= 10000)
	{
		digits[4]++;
		number -= 10000;
		var_dig |= (1<<5);
	}
	while (number >= 1000)
	{
		digits[3]++;
		number -= 1000;
		var_dig |= (1<<4);
	}
	while (number >= 100)
	{
		digits[2]++;
		number -= 100;
		var_dig |= (1<<3);
	}
	while (number >= 10)
	{
		digits[1]++;
		number -= 10;
		var_dig |= (1<<2);
	}
	digits[0] = number;
	if(leading_zero == 0 || leading_zero == 1)
	{
		if(var_dig & (1<<5))
		{
			digit_cnt = 5;
		}
		else if(var_dig & (1<<4))
		{
			digit_cnt = 4;
		}
		else if(var_dig & (1<<3))
		{
			digit_cnt = 3;
		}
		else if(var_dig & (1<<2))
		{
			digit_cnt = 2;
		}
		else
		{
			digit_cnt = 1;
		}
		if(leading_zero == 1)
		{
			for(uint8_t i = digit_cnt_buf-digit_cnt; i>0;i--)
			{
				uart_tx_string(" ");
			}
		}
	}
	while(digit_cnt > 0)
	{
		uart_tx_byte(48+digits[digit_cnt-1]);
		if(digit_cnt == (5-decpoint))
		{
			uart_tx_string(".");
		}
		digit_cnt--;
	}
}

void uart_tx_number(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero) //
{
	uint8_t digits[5] = {0,0,0,0,0};
	uint8_t var_dig = 0;
	uint8_t digit_cnt_buf = digit_cnt;
    while (number >= 10000) 
	{
        digits[4]++;
        number -= 10000;
		var_dig |= (1<<5);
    }
    while (number >= 1000) 
	{
        digits[3]++;
        number -= 1000;
		var_dig |= (1<<4);
    }
	while (number >= 100) 
	{
        digits[2]++;
        number -= 100;
		var_dig |= (1<<3);
    }
    while (number >= 10) 
	{
        digits[1]++;
        number -= 10;
		var_dig |= (1<<2);
    }
	digits[0] = number;
	if(leading_zero == 0 || leading_zero == 1)
	{
		if(var_dig & (1<<5))
		{
			digit_cnt = 5;
		}
		else if(var_dig & (1<<4))
		{
			digit_cnt = 4;
		}
		else if(var_dig & (1<<3))
		{
			digit_cnt = 3;
		}
		else if(var_dig & (1<<2))
		{
			digit_cnt = 2;
		}
		else
		{
			digit_cnt = 1;
		}
		if(leading_zero == 1)
		{
			for(uint8_t i = digit_cnt_buf-digit_cnt; i>0;i--)
			{
				uart_tx_string(" ");
			}
		}
	}
	while(digit_cnt > 0)
	{
		uart_tx_byte(48+digits[digit_cnt-1]);
		digit_cnt--;
	}
}



void uart_tx_time(uint8_t s, uint8_t m, uint8_t h,uint16_t d)
{
	if(d)
	{
		uart_tx_number(d,5,0);
		uart_tx_string(".");
	}
	uart_tx_number(h,2,2);
	uart_tx_string(":");
	uart_tx_number(m,2,2);
	uart_tx_string(":");
	uart_tx_number(s,2,2);
}

void uart_tx_temperature(int16_t temp,uint8_t unit)
{
	if(temp >= 0)
	{
		uint8_t digit = 0;
		while (temp >= 100) 
		{
	        digit += 10;
	        temp -= 100;
	    }
	    while (temp >= 10) 
		{
	        digit++;
	        temp -= 10;
	    }
		uart_tx_string("+");
		uart_tx_number(digit,2,2);
		uart_tx_string(".");
		uart_tx_byte(48+temp);
		if(unit)
		uart_tx_string("'C");
	}
	else
	{
		uint8_t digit_ = 0;
		while (temp <= -100) 
		{
	        digit_ += 10;
	        temp += 100;
	    }
	    while (temp <= -10) 
		{
	        digit_++;
	        temp += 10;
	    }
		uart_tx_string("-");
		uart_tx_number(digit_,2,2);
		uart_tx_string(".");
		uart_tx_byte(48 + ~temp);
		if(unit)
		uart_tx_string("'C");
	}
}

void uart_tx_line_break()
{
	uart_tx_byte('\r');
	uart_tx_byte('\n');
}
void print_string_flash(const char* string)
{
	char c;
	for(;;)
	{	c=pgm_read_byte(string++);
		if (!c)
			break;
		uart_tx_byte(c);
	}
}

