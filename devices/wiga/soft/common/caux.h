/************************************************************************* 
* Title:    C include file for auxiliary functions on cheesboar system
* Author:   Michal Harakal
* File:     $Id $
* Software: AVR-GCC 3.4.3 / avr-libc 1.2.3
* Target:   any AVR device running TWI
**************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _CAUX_H
#define _CAUX_H   1

#define BYTE uint8_t
#define WORD uint16_t
/**
 @brief reads all analog channels on PHILIPS I2C AD and returns value of desired channael
 @param  channel number
 @return 8-bit ananlog value of desires channel
 */
extern uint8_t i2c_ReadAD(uint8_t channel);

/**
 @brief reads all analog channel and returns value of desired channael
 @param  channel number
 @param  channel internal if true internal reference source is used
 @return 10-bit analog value of desires channel
 */
extern uint16_t ReadAD(uint8_t channel, uint8_t internal);

/**
 @brief sets callback function for 1 second timer
 @param  pointer on callback function
 */
extern void RegisterCallback1Sec(void (*OneSecFnc)(void));

/**
 @brief sets callback function for 10 milisecond timer
 @param  pointer on callback function
 */
extern void RegisterCallback10MiliSec(void (*TenMiliSecFnc)(void));

/**
 @brief sets callback function for 10 milisecond timer
 */
extern void RegisterCallbackMainIdle(void (*MainIdle)(void));

/**
 @brief sets callback function for handling of serial port
 @param  pointer on callback function
 */
extern void RegisterUserUart(void (*uartFnc)(char c));

/**
 @brief sets callback function for imemdiate handling of timer 8 interrupt
 @param  pointer on callback function
 */
extern void RegisterTimer8(void (*timer8Fnc)(void));


/**
 @brief sets callback function for 10 milisecond timer
 @param  pointer on callback function
 */
extern void ExecuteMain(void);

/**
 @brief inits 8-bit timers on ATMega
 */
extern void Init8Timer(void);

/**
 @brief inits 16-bit timers on ATMega
 */
extern void Init16Timer(void);

extern volatile uint32_t timer1_counter;

#endif

#ifdef __cplusplus
}
#endif
