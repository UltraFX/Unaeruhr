/*
 * MedLevel.h
 *
 * Created: 27.03.2015 16:53:07
 *  Author: Nicolas Dammin
 */ 


#ifndef MEDLEVEL_H_
#define MEDLEVEL_H_

#define ONE			0x80
#define TWO			0x01
#define THREE		0x02
#define FOUR		0x04
#define FIVE		0x08
#define SIX			0x10
#define SEVEN		0x20
#define EIGHT		0x40
#define NINE		0x80

#define ONE_HR		0x01
#define TWO_HR		0x02

#define ONE_MS		0x01
#define TWO_MS		0x02
#define THREE_MS	0x04
#define FOUR_MS		0x08
#define FIVE_MS		0x10

#define HR_ONES		0
#define HR_TENS		1
#define MIN_ONES	2
#define MIN_TENS	3
#define SEC_ONES	4
#define SEC_TENS	5

#define NINE_INV	0x00
#define EIGHT_INV	0x80 //0x01
#define SEVEN_INV	0xC0 //0x03
#define SIX_INV		0xE0 //0x07
#define FIVE_INV	0xF0 //0x0f
#define FOUR_INV	0xF8 //0x1f
#define THREE_INV	0xFC //x3f
#define TWO_INV		0xFE //0x7f
#define ONE_INV		0xff
#define ZERO_INV	0xff

#define TN_FOUR_INV		0x10 //0x01
#define TN_THREE_INV	0x18 //0x03
#define TN_TWO_INV		0x1C //0x07
#define TN_ONE_INV		0x1E //x0f
#define TN_ZERO_INV		0x1f

#define H_ZERO_INV		0x03
#define H_ONE_INV		0x02

void setSeconds(uint8_t seconds, uint8_t color_tens, uint8_t color_ones, uint8_t color_five);
void setMinutes(uint8_t minutes, uint8_t color_tens, uint8_t color_ones, uint8_t color_five);
void setHours(uint8_t hours, uint8_t color_tens, uint8_t color_ones, uint8_t color_five);

void lightRest(uint8_t color);

void SecRest(uint8_t seconds, uint8_t color, bool res);
void MinRest(uint8_t minutes, uint8_t color, bool res);
void HrRest(uint8_t hrs, uint8_t color, bool res);

#endif /* MEDLEVEL_H_ */