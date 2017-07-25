/*
 * buttons.h
 *
 * Created: 23.04.2017 15:50:55
 *  Author: Nicolas Dammin
 */ 


#ifndef BUTTONS_H_
#define BUTTONS_H_

#define BUTTON1		0x01
#define BUTTON2		0x02
#define BUTTON3		0x04

#define BUTTONS_DDR         DDRD
#define BUTTONS_PORT        PORTD
#define BUTTONS_PIN         PIND

#define HW_BUTTON0            1
#define HW_BUTTON1            2
#define HW_BUTTON2            4

void buttons_init(void);
void buttons_handler(void);

uint8_t buttons_get_first(void);
uint8_t buttons_get_second(void);
uint8_t buttons_get_third(void);

#endif /* BUTTONS_H_ */