/*
 * HighLevel.h
 *
 * Created: 27.03.2015 16:53:14
 *  Author: Nicolas Dammin
 */ 


#ifndef HIGHLEVEL_H_
#define HIGHLEVEL_H_

void intSeconds(void);
void intMilliseconds(void);

void testInit(void);
void mainProcedure(void);
void effect1(void);
void effect2(void);

void blink(uint8_t bySection);
void loadTime(void);
void storeTime(void);

//effect-states
enum{STATE_INTRO, STATE_RED, STATE_RED2, STATE_PINK, STATE_TURKEY, STATE_BLUE, STATE_GREEN, STATE_YELLOW, STATE_LAST, STATE_OFF};
enum{INIT, SECS, MINS, HRS, OFF};
	
enum{STATE_INIT, STATE_INIT2, STATE_REDSECS, STATE_REDMINS, STATE_COLOR, STATE_CHANGE};
	
extern uint16_t count_sec;

void showDate(void);

#endif /* HIGHLEVEL_H_ */