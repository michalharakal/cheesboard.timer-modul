#ifdef __cplusplus
extern "C" {
#endif

#ifndef _KEYS_H
#define _KEYS_H   1

// global variable
extern volatile uint8_t key_press;		

// key handler
extern void KeyDebounceTimerHandler(void);

extern uint8_t get_key_press( uint8_t key_mask );

#endif 

#ifdef __cplusplus
}
#endif
