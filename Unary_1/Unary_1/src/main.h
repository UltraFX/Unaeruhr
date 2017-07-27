/*
 * main.h
 *
 * Created: 25.03.2015 17:19:15
 *  Author: Nicolas Dammin
 */ 

#include <avr/io.h>

#ifndef MAIN_H_
#define MAIN_H_

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif

#define STATE_CLOCK_ENTRY	0x1
#define STATE_CLOCK			0x2
#define STATE_DATE_ENTRY	0x3
#define STATE_DATE			0x4
#define STATE_SET_MONTH		0x5
#define STATE_SET_DAY		0x6
#define STATE_SET_HOURS		0x7
#define STATE_SET_MINUTES	0x8
#define STATE_STORE			0x9
#define STATE_LOAD			0xA
#define STATE_LOAD_START	0xB
#define STATE_LOAD_END		0xC
#define STATE_STORE_START	0xD
#define STATE_STORE_STOP	0xE

extern uint8_t	g_byState;
extern uint16_t	g_wButtonPressedCounter1, g_wButtonPressedCounter2;
uint8_t			g_byButtonFlag[3];

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "hardware.h"
#include "HighLevel.h"
#include "LED.h"
#include "DS1307.h"
//#include "debounce.h"
#include "DCF77.h"
#include "buttons.h"

#define EFFECT_TIME	3UL //Effect-Changing Time in ms


#endif /* MAIN_H_ */