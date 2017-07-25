/*
 * LowLevel.h
 *
 * Created: 27.03.2015 16:52:51
 *  Author: Nicolas Dammin
 */ 


#ifndef LOWLEVEL_H_
#define LOWLEVEL_H_


/*	Definitions of the Hardware-Pins	*/
#define MOSI_PIN	PB5
#define CLK_PIN		PB7
#define LATCH_PIN	PB1
#define RES_PIN		PB0

#define RED_PIN		PB2
#define GREEN_PIN	PB4
#define BLUE_PIN	PB3
	
#define RED		0x01
#define GREEN	0x04
#define BLUE	0x02

#define TIME_START	0x01
#define TIME_END	0x02

void SPI_Interrupt(void);

void set_led(uint8_t reg, uint8_t bit, uint8_t color, uint8_t col_five);
void reset_led(uint8_t reg, uint8_t bit, uint8_t color);

void eep_get_time(uint8_t byTime, uint8_t *pbyHour, uint8_t *pbyMin);
void eep_set_time(uint8_t byTime, uint8_t *pbyHour, uint8_t *pbyMin);


#endif /* LOWLEVEL_H_ */