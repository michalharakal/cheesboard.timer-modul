#ifndef _STATUS_H
#define _STATUS_H

#define STATUS_NORMAL             0
#define STATUS_MOTOR_MOVING       1
#define STATUS_MOTOR_PAUSE        2
#define STATUS_MOTOR_END          3

#define CMD_SEND_WIGA_DATA       'D'


#define CMD_SEND_TIME_DATA        'T'
#define CMD_SEND_DATE_DATA        'D'
#define CMD_SEND_ABEND_DATA       'A'
#define CMD_SEND_DCF_STATUS_DATA  'F'
#define CMD_SEND_SUNSET_DATA      'S'
#define CMD_SEND_WINDOW_DATA      'W'
#define CMD_SEND_DELAY_DATA       'L'
#define CMD_SEND_WATERING_DATA    'R'

#define I2C_WIGA_LCD_ADRESS     (12 << 1)
#define I2C_ABEND_LCD_ADRESS     (13 << 1)

#endif //_STATUS_H
