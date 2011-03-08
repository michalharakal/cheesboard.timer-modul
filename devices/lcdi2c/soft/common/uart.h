#ifndef UART_H
#define UART_H
/************************************************************************
Title:    UART library with circular buffers
Author:   Michal Harakal <harakal@web.de>
File:     $Id $
Software: AVR-GCC 3.4
Hardware: at90s4433/atmega8
Usage:    see Doxygen manual
************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#define ANSWER (char) '#'
#define BADANSWER (char) '!'

#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This file requires AVR-GCC 3.4 or later!"
#endif

/*
** function prototypes
*/

/**
   @brief   Initialize UART and set baudrate 
   @param   baudrate Specify baudrate according atmel datasheet
   @return  none
*/
extern void uart_init(unsigned int baudrate);


/**
 *  @brief   wait until any data received and return one char
 *
 *  @param   void
 *  @return  received byte from ringbuffer
 */
extern unsigned char uart_getc(void);

/**
 *  @brief   sends char to uart
 *  @param   character to send
 *  @return  sended chars
 */
extern int uart_putc (unsigned char data, FILE* dummy);

/**
 *  @brief   string to uart
 *
 *  @param   buff buffer for data
 */
extern void uart_put(char *buff);

/**
 *  @brief   checks receive buffer
 *
 *  @param   void
 *  @return  results 0 (FALSE) if the receive buffer is empty
 */
extern unsigned char uart_data_ready(void);

/**
 *  @brief   clears receive buffer
 *
 *  @param   void
 *  @return  none
 */
extern void uart_clear_receive_buffer(void);

/**
 *  @brief   wait until any data received and return desired count of chars
 *
 *  @param   buff buffer for data
 *  @param   count count of data to read
 *  @return  received byte from ringbuffer
 */
extern void uart_get(char *buff, unsigned char count);

/**
 *  @brief   wait until any data received and return char
 *
 *  @return  received byte from ringbuffer
 */
extern uint8_t uart_getc_wait(void);

/**
 *  @brief get last char from buffer
 *
 *  @param   void
 *  @return  last char from buffer
 */
extern unsigned char uart_get_lastc(void);

#endif


#ifdef __cplusplus
}
#endif
