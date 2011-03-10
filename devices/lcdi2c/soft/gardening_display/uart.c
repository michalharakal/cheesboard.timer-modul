/*********************************************
 * uart lib
 *********************************************/

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "global.h"

/* UART Buffer Defines */

#define USART_RX_BUFFER_SIZE 32     // 2,4,8,16,32,64,128 or 256 bytes
#define USART_RX_BUFFER_MASK ( USART_RX_BUFFER_SIZE - 1 )

/* the RTSOUT must be on port D */
#define RTSOUT PD2
#define RTPORT PORTD
#define SLEEP  PD3
#define SLPORT PORTD


#if ( USART_RX_BUFFER_SIZE & USART_RX_BUFFER_MASK )
#error RX buffer size is not a power of 2
#endif

static unsigned char USART_RxBuf[USART_RX_BUFFER_SIZE]; 
static volatile unsigned char USART_RxHead;
static volatile unsigned char USART_RxTail; 

/**
 * @return results 0 (FALSE) if the receive buffer is empty
 */
unsigned char uart_data_ready( void )
{
  return ( USART_RxHead != USART_RxTail ); 
}

void uart_clear_receive_buffer(void)
{
  unsigned char x = 0;     

  USART_RxTail = x;
  USART_RxHead = x;
}


/**
 *  @brief   wait until any data received and return one char
 *
 *  @param   void
 *  @return  received byte from ringbuffer
 */
unsigned char uart_getc(void)
{
  unsigned char tmptail;
  
  // Wait for incomming data
  while ( USART_RxHead == USART_RxTail )  
    ;
  // Calculate buffer index
  tmptail = ( USART_RxTail + 1 ) & USART_RX_BUFFER_MASK;
  // Store new index
  USART_RxTail = tmptail;                
  // Return data
  return USART_RxBuf[tmptail];       
}

/**
 *  @brief get last char from buffer
 *
 *  @param   void
 *  @return  last char from buffer
 */
unsigned char uart_get_lastc(void)
{
  if ( USART_RxHead == USART_RxTail ) 
    return 0;
  else
    return USART_RxBuf[USART_RxHead];       
}

/**
 *  @brief   wait until any data received and return desired count of chars
 *
 *  @param   buff buffer for data
 *  @param   count count of data to read
 *  @return  received byte from ringbuffer
 */
void uart_get(char *buff, unsigned char count)
{
  while (count > 0) {
    *buff = uart_getc();
    buff++;
    count--;
  }
}

/**
 *  @brief   wait until any data received and return char
 *
 *  @return  received byte from ringbuffer
 */
uint8_t uart_getc_wait(void)
{
  while (!uart_data_ready())
    ;
  return uart_getc();
}

/**
 *  @brief   puts byte to ringbuffer for transmitting via UART
 *  @param   data byte to be transmitted
 *  @return  none
 */
int uart_putc (char data, FILE* dummy)
{
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = data;    
  
  return (0); 
}

/**
 *  @brief   string to uart
 *
 *  @param   buff buffer for data
 */
void uart_put(char *buff)
{
  while (*buff) {
    uart_putc(*buff, NULL);
    buff++;
  }
}

/**
 * Initializes USART 
 *
 * @param baudrate transmition baudrate
 */
void uart_init( unsigned int baudrate )
{
  unsigned char x; 

  // Set the baud rate
#if defined(__AVR_ATmega8__)
  UBRRL = baudrate;
#else
  UBRRL = baudrate; 
#endif
  // Enable UART receiver and transmitter
  sbi(UCSRB, TXEN);
  sbi(UCSRB, RXEN);
  sbi(UCSRB, RXCIE);  
  // Flush receive buffer
  x = 0;     

  USART_RxTail = x;
  USART_RxHead = x;
  
  //redirect printf (STDOUT) to serial channel
  fdevopen (uart_putc, NULL);
}

SIGNAL(SIG_UART_RECV)
{
  unsigned char data;
  unsigned char tmphead;

  // Read the received data
  data = UDR;
  // Calculate buffer index
  tmphead = ( USART_RxHead + 1 ) & USART_RX_BUFFER_MASK;
  // Store new index
  USART_RxHead = tmphead;     

  if ( tmphead == USART_RxTail )
  {
    // ERROR! Receive buffer overflow
  }
  USART_RxBuf[tmphead] = data; // Store received data in buffer
}



