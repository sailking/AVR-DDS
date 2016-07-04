#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/io.h>
#include <avr/pgmspace.h>

#define NUMBER_OF_START_BYTES 2
#define PREAMBLE 0xAA
#define ADDR_MULTIMETER 1
#define ADDR_LOCKIN 2
#define ADDR_COUNTER 3

extern const uint8_t device_address;

#define DATA_FRAME_LENGTH 40*4
extern volatile uint8_t data_frame_out[DATA_FRAME_LENGTH];

#define BAUD_VALUE 1 // 51

extern volatile uint8_t data_ok;
extern volatile uint8_t data_frame_in[DATA_FRAME_LENGTH];


void uart_init0 ();
void uart_init1 ();
uint16_t get_num_from_ascii(char* in);
void uart_tx_byte(unsigned char c);
void uart1_tx_byte(unsigned char c);
uint8_t uart_tx_string(char* string);
void uart_tx_command(uint16_t com);
void uart_tx_num16(uint16_t number);
void uart_tx_line_break();
void print_string_flash(const char* string);
void uart_tx_hex8(uint8_t data);
void uart_tx_hex16(uint16_t data);
void uart_tx_temperature(int16_t temp,uint8_t unit);
void uart_tx_number(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero);
void uart_tx_time(uint8_t s, uint8_t m, uint8_t h,uint16_t d);
void clear_text_buffer();
uint8_t get_h_from_ascii(char* in);
uint8_t get_min_from_ascii(char* in);
void uart_tx_number2(uint16_t number, uint8_t digit_cnt, uint8_t leading_zero,uint8_t decpoint);
uint8_t uart0_tx_frame();
uint8_t uart1_tx_frame();