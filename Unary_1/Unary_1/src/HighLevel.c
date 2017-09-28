/*
 * HighLevel.c
 *
 * Created: 27.03.2015 16:52:37
 *  Author: Nicolas Dammin
 */ 

#include "main.h"
#include "LowLevel.h"
#include "MedLevel.h"



uint16_t	count_sec = 0;
uint8_t		count_effect = 0;
uint16_t	wBlinkCounter = 0;
uint8_t		bySetMin, bySetMax;

uint8_t		byPressCount = 0;
uint16_t	wNumberDelay = 0;

static uint8_t byMonths = 4, byDays = 27, byHours = 3, byMinutes = 15;

/* Hours and Minutes of Off-Time */
uint8_t stop_hour = 0, stop_minute = 0;

/* Hours and Minutes of On-Time */
uint8_t start_hour = 0, start_minute = 0;

uint8_t seconds = 0, minutes = 0, hours = 0, eff_col_state = STATE_INTRO, eff_num_state = 0, eff_color = RED, eff_pre_color = RED;
volatile bool secInt = false, effect_change = false;
uint8_t effectNr = 0;
uint32_t mSeconds = 0;
volatile uint8_t bySecOld, bySec;

uint16_t eff_sec_ones = 0,
		 eff_sec_tens = 0,
		 eff_min_ones = 0,
		 eff_min_tens = 0,
		 eff_hr_ones = 0,
		 eff_hr_tens = 0;

void intSeconds(void)
{
	secInt = true;
	count_sec++;
	g_wButtonPressedCounter1++;
	g_wButtonPressedCounter2++;
	g_wButtonPressedCounter3++;
	byPressCount++;
	bySec++;
	if(bySec == 60)
	{
		bySec = 0;
	}
}

void intMilliseconds(void)
{
	wNumberDelay++;
	if(count_effect >= 100)
	{
		count_effect = 0;
		effect_change = true;
	}
	else
	{
		count_effect++;
	}	
	wBlinkCounter++;
	/*if(mSeconds >= EFFECT_TIME) {
		mSeconds = 0;
		effect_change = true;
	} else
		mSeconds++;*/
}

void testInit(void)
{
		eep_get_time(TIME_START, &start_hour, &start_minute);
		eep_get_time(TIME_END, &stop_hour, &stop_minute);
}

void loadTime(void)
{
	Read_DS1307();

	byMonths = rtc_data[5];
	byDays = rtc_data[4];
	byHours = rtc_data[2];
	byMinutes = rtc_data[1];
}

void loadOffTime(void)
{
	eep_get_time(TIME_START, &stop_hour, &stop_minute);
	
	byMonths = 0;
	byDays = 0;
	byHours = stop_hour;
	byMinutes = stop_minute;
}

void loadOnTime(void)
{	
	eep_get_time(TIME_END, &start_hour, &start_minute);
	
	byMonths = 0;
	byDays = 0;
	byHours = start_hour;
	byMinutes = start_minute;
}

void storeTime(void)
{
	new_time[0] = 00; //Seconds
	new_time[1] = byMinutes; //Minutes
	new_time[2] = byHours; //Hours
	new_time[3] = 00; //kp
	new_time[4] = byDays; //Day
	new_time[5] = byMonths; //Month
	new_time[6] = 17; //Year
	Write_DS1307();
}

void storeOffTime(void)
{
	eep_set_time(TIME_START, &byHours, &byMinutes);
}

void storeOnTime(void)
{
	eep_set_time(TIME_END, &byHours, &byMinutes);
}

/* Blinking routine to setup clock or On-/Off-Time */
void blink(uint8_t bySection)
{
	static uint8_t byBlinkState;
	static uint8_t *pbyTime = NULL;

	if(wBlinkCounter >= 500)
	{
		wBlinkCounter = 0;
		byBlinkState ^= 1;
	}

	/* Light on */
	if(byBlinkState == 1)
	{
		switch(bySection)
		{
		case STATE_SET_MONTH:
			pbyTime = &byMonths;
			bySetMin = 1;
			bySetMax = 12;
			setHours(0, RED, RED, RED | BLUE);
			setSeconds(byDays, GREEN, GREEN, GREEN | RED);
			setMinutes(byMonths, RED | BLUE, RED | BLUE, RED);
			break;
		case STATE_SET_DAY:
			pbyTime = &byDays;
			bySetMin = 1;
			bySetMax = 31;
			setHours(0, RED, RED, RED | BLUE);
			setSeconds(byDays, GREEN, GREEN, GREEN | RED);
			setMinutes(byMonths, RED | BLUE, RED | BLUE, RED);
			break;
		case STATE_SET_HOURS:
			pbyTime = &byHours;
			bySetMin = 0;
			bySetMax = 23;
			setHours(byHours, RED, RED, RED | BLUE);
			setMinutes(byMinutes, GREEN, GREEN, GREEN | RED);
			setSeconds(0, BLUE, BLUE, BLUE | GREEN);
			break;
		case STATE_SET_MINUTES:
			pbyTime = &byMinutes;
			bySetMin = 0;
			bySetMax = 59;
			setHours(byHours, RED, RED, RED | BLUE);
			setMinutes(byMinutes, GREEN, GREEN, GREEN | RED);
			setSeconds(0, BLUE, BLUE, BLUE | GREEN);
			break;
			
		}
	}
	else 
	/* Light off */
	{
		switch(bySection)
		{
		case STATE_SET_MONTH:
			setMinutes(0, GREEN, GREEN, GREEN | RED);
			break;
		case STATE_SET_DAY:
			setSeconds(0, BLUE, BLUE, BLUE | GREEN);
			break;
		case STATE_SET_HOURS:
			setHours(0, RED, RED, RED | BLUE);
			break;
		case STATE_SET_MINUTES:
			setMinutes(0, GREEN, GREEN, GREEN | RED);
			break;
		}
	}

	/* decrease time */
	if(buttons_get_second())
	{
		/* single press */
		if(g_byButtonFlag[1] != 1)
		{
			byPressCount = 0;
			wNumberDelay = 0;
			count_sec = 0;
			if(pbyTime != NULL)
			{
				/* underflow: Set maximum time */
				if(*pbyTime == bySetMin)
				{
					*pbyTime = bySetMax;
				}
				else
				{
					/* decrease by 1 */
					*pbyTime = *pbyTime - 1;
				}
			}
		}
		g_byButtonFlag[1] = 1;

		/* automatic decrease when button stays pressed */
		if(byPressCount >= 2)
		{
			if(wNumberDelay >= 200)
			{
				if(pbyTime != NULL)
				{
					/* underflow: Set maximum time */
					if(*pbyTime == bySetMin)
					{
						*pbyTime = bySetMax;
					}
					else
					{
						/* decrease by 1 */
						*pbyTime = *pbyTime - 1;
					}
				}
				wNumberDelay = 0;
			}
		}
	}
	else
	{
		g_byButtonFlag[1] = 0;
	}

	/* increase time */
	if(buttons_get_third())
	{
		/* single press */
		if(g_byButtonFlag[2] != 1)
		{
			byPressCount = 0;
			wNumberDelay = 0;
			count_sec = 0;
			if(pbyTime != NULL)
			{
				/* overflow: Set minimum time */
				if(*pbyTime == bySetMax)
				{
					*pbyTime = bySetMin;
				}
				else
				{
					/* increase by one */
					*pbyTime = *pbyTime + 1;
				}
			}
		}
		g_byButtonFlag[2] = 1;

		/* automatic decrease when button stays pressed */
		if(byPressCount >= 2)
		{
			if(wNumberDelay >= 200)
			{
				if(pbyTime != NULL)
				{
					/* overflow: Set minimum time */
					if(*pbyTime == bySetMax)
					{
						*pbyTime = bySetMin;
					}
					else
					{
						/* increase by one */
						*pbyTime = *pbyTime + 1;
					}
				}
				wNumberDelay = 0;
			}
		}
	}
	else
	{
		g_byButtonFlag[2] = 0;
	}
}

void mainProcedure(void)
{
	
	uint8_t bTimeOn = 0;
	
	PORTD ^= (1 << PD3);
	
	if(bySec > bySecOld)
	{
		Read_DS1307();

		PORTD ^= (1 << PD4);
	}
	
	if(start_hour > stop_hour)
	{
		if(((rtc_data[2] == start_hour) && rtc_data[1] < start_minute) || rtc_data[2] < start_hour)
		{
			if(((rtc_data[2] == stop_hour) && rtc_data[1] > stop_minute) || rtc_data[2] > stop_hour)
			{
				bTimeOn = 1;
			}
		}
	}
	else
	{
		if(((rtc_data[2] == start_hour) && rtc_data[1] > start_minute) || rtc_data[2] > start_hour)
		{
			if(((rtc_data[2] == stop_hour) && rtc_data[1] < stop_minute) || rtc_data[2] < stop_hour)
			{
				bTimeOn = 1;
			}
		}
	}
	//bTimeOn = 1;
	
	/* check if time is out of On-/Off-Time */
	/*if(	((start_hour > stop_hour) &&
		(((rtc_data[2] == start_hour) && (rtc_data[1] <= start_minute)) || (rtc_data[2] < start_hour)) &&
		(((rtc_data[2] == stop_hour) && (rtc_data[1] >= stop_minute)) || (rtc_data[2] > stop_hour)))
		|| 
		((start_hour < stop_hour) &&
		(((rtc_data[2] == start_hour) && (rtc_data[1] >= start_minute)) || (rtc_data[2] > start_hour)) &&
		(((rtc_data[2] == stop_hour) && (rtc_data[1] <= stop_minute)) || (rtc_data[2] < stop_hour)))
		)*/
	
	if(1)//bTimeOn)
	{
		/* run effect 30s before the next hour */
		if(rtc_data[1] > 58 && rtc_data[0] > 30)
		{
			if(effectNr == 0)
			effect1();
			else
			effect2();
		}
		
		/* Update display each second */
		if(bySec != bySecOld)
		{
			if(!(rtc_data[1] > 58 && rtc_data[0] > 30))
			{
				effectNr = (rtc_data[2]%2 == 0)?0:1;
				setSeconds(rtc_data[0], BLUE, BLUE, BLUE | GREEN);
				setMinutes(rtc_data[1], GREEN, GREEN, GREEN | RED);
				setHours(rtc_data[2], RED, RED, RED | BLUE);
				
				if(rtc_data[0]%10 == 9)
				lightRest(GREEN | RED);
			}
		}
	}
	else
	{
		setSeconds(0, BLUE, BLUE, BLUE | GREEN);
		setMinutes(0, GREEN, GREEN, GREEN | RED);
		setHours(0, RED, RED, RED | BLUE);
	}
	
	bySec = bySecOld;
	
}

void showDate(void) {
	if(secInt != false) {
		secInt = false;
		
		Read_DS1307();
		
		setSeconds(rtc_data[4], GREEN, GREEN, GREEN | RED);
		setMinutes(rtc_data[5], RED | BLUE, RED | BLUE, RED);
		setHours(0, BLUE, BLUE, BLUE);
	}
}

void effect1(void) {
	if(effect_change) {
		effect_change = 0;
		
		switch(eff_num_state) {
		case INIT:
					eff_num_state = SECS;
					switch(eff_col_state) {
					case STATE_INTRO: eff_color = RED; eff_pre_color = RED; eff_col_state = STATE_RED; break;
					case STATE_RED: eff_color = RED | BLUE; eff_pre_color = RED; eff_col_state = STATE_PINK;  break;
					case STATE_PINK: eff_color = BLUE |GREEN; eff_pre_color = RED | BLUE; eff_col_state = STATE_TURKEY; break;
					case STATE_TURKEY: eff_color = BLUE; eff_pre_color = GREEN | BLUE; eff_col_state = STATE_BLUE; break;
					case STATE_BLUE: eff_color = GREEN; eff_pre_color = BLUE; eff_col_state = STATE_GREEN; break;
					case STATE_GREEN: eff_color = GREEN| RED; eff_pre_color = GREEN; eff_col_state = STATE_YELLOW; break;
					case STATE_YELLOW: eff_color = RED | GREEN; eff_pre_color = RED | GREEN; eff_col_state = STATE_INTRO; eff_num_state = OFF; break;
					}
					eff_sec_ones = eff_sec_tens = eff_min_ones = eff_min_tens = eff_hr_ones = eff_hr_tens = 0;
					mSeconds = 0;
		case SECS:	if(eff_sec_tens < 5) {
						setSeconds(++eff_sec_tens*10, eff_color, eff_color, eff_color);
						if(eff_col_state != STATE_RED) SecRest(eff_sec_tens*10, eff_pre_color, false);
						break;
					} else if(eff_sec_ones < 9) {
						setSeconds(eff_sec_tens*10+(++eff_sec_ones), eff_color, eff_color, eff_color);
						if(eff_col_state != STATE_RED) SecRest(eff_sec_tens*10+eff_sec_ones, eff_pre_color, false);
						break;
					} else 
						eff_num_state = MINS;
						
		case MINS:	if(eff_min_tens < 5) {
						setMinutes(++eff_min_tens*10, eff_color, eff_color, eff_color);
						if(eff_col_state != STATE_RED) MinRest(eff_min_tens*10, eff_pre_color, false);
						break;
					} else if(eff_min_ones < 9) {
						setMinutes(eff_min_tens*10+(++eff_min_ones), eff_color, eff_color, eff_color);
						if(eff_col_state != STATE_RED) MinRest(eff_min_tens*10+eff_min_ones, eff_pre_color, false);
						break;
					} else
						eff_num_state = HRS;
						
		case HRS:	if(eff_hr_tens < 2) {
						setHours(++eff_hr_tens*10, eff_color, eff_color, eff_color);
						if(eff_col_state != STATE_RED) HrRest(eff_hr_tens*10, eff_pre_color, false);
					} else if(eff_hr_ones < 9) {
						setHours(eff_hr_tens*10+(++eff_hr_ones), eff_color, eff_color, eff_color);
						if(eff_col_state != STATE_RED) HrRest(eff_hr_tens*10+eff_hr_ones, eff_pre_color, false);
					} else 
						eff_num_state = INIT;
					break;
						
		case OFF:	if(eff_num_state == OFF) {
						if(eff_sec_tens < 5) {
							SecRest((++eff_sec_tens)*10, eff_pre_color, true);
						} else if(eff_sec_tens == 5 && eff_sec_ones < 9) {
							reset_led(SEC_ONES, 0xff, RED | GREEN | BLUE);
							SecRest(++eff_sec_ones, eff_pre_color, true);
						} else if(eff_min_tens < 5) {
							MinRest((++eff_min_tens)*10, eff_pre_color, true);
						} else if(eff_min_tens == 5 && eff_min_ones < 9) {
							MinRest(++eff_min_ones, eff_pre_color, true);
						} else if(eff_hr_tens < 2) {
							HrRest((++eff_hr_tens)*10, eff_pre_color, true);
						} else if(eff_hr_tens == 2 && eff_hr_ones < 9) {
							HrRest(++eff_hr_ones, eff_pre_color, true);
						}
						else
							eff_num_state = INIT;
					}
					break;
		}
	}
}


void effect2(void) {
	static uint8_t col_1 = RED, col_2 = RED, col_3 = RED, col_4 = RED;
	
	if(effect_change) {
		effect_change = 0;
		
		switch(eff_num_state) {
			case STATE_INIT:
						reset_led(MIN_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
						reset_led(SEC_TENS, 0xff, RED | GREEN | BLUE);
						reset_led(HR_TENS, 0xff, RED | GREEN | BLUE);
						reset_led(MIN_ONES, 0xff, RED | GREEN | BLUE);
						reset_led(SEC_ONES, 0xff, RED | GREEN | BLUE);
						reset_led(HR_ONES, 0xff, RED | GREEN | BLUE);
						eff_sec_ones = eff_sec_tens = eff_min_ones = eff_min_tens = eff_hr_ones = eff_hr_tens = 0;
						col_1 = RED; col_2 = RED | BLUE; col_3 = GREEN | BLUE;
						mSeconds = 0;
						eff_col_state = STATE_RED;
						eff_num_state = STATE_REDSECS;
						
			case STATE_REDSECS:	if(eff_sec_tens < 5) {
									setSeconds(++eff_sec_tens*10, col_1, col_1, 0);
									break;
								} else if(eff_sec_ones < 9) {
									setSeconds(eff_sec_tens*10+(++eff_sec_ones), col_1, col_1, 0);
									break;
								} else
								eff_num_state = STATE_REDMINS;
			
			case STATE_REDMINS:	if(eff_min_tens < 5) {
									setMinutes(++eff_min_tens*10, col_1, col_1, 0);
									setSeconds(eff_min_tens*10, col_2, col_2, 0);
									SecRest(eff_min_tens*10, col_1, false);
									break;
								} else if(eff_min_ones < 9) {
									setMinutes(eff_min_tens*10+(++eff_min_ones), col_1, col_1, 0);
									setSeconds(eff_sec_tens*10+(eff_min_ones), col_2, col_2, 0);
									SecRest(eff_min_tens*10+eff_min_ones, col_1, false);
									break;
								} else
									eff_num_state = STATE_COLOR;
						
			case STATE_COLOR:	eff_sec_ones = eff_sec_tens = eff_min_ones = eff_min_tens = eff_hr_ones = eff_hr_tens = 0;
								eff_num_state = STATE_CHANGE;
								switch(eff_col_state) {
									case STATE_RED:	col_1 = RED, col_2 = RED | BLUE; col_3 = GREEN | BLUE; eff_col_state = STATE_PINK; break;
									case STATE_PINK: col_4 = RED; col_1 = RED | BLUE; col_2 = GREEN | BLUE; col_3 = BLUE; eff_col_state = STATE_TURKEY; break;
									case STATE_TURKEY: col_4 = RED | BLUE; col_1 = GREEN | BLUE; col_2 = BLUE; col_3 = GREEN; eff_col_state = STATE_BLUE; break;
									case STATE_BLUE: col_4 = BLUE | GREEN; col_1 = BLUE; col_2 = GREEN; col_3 = GREEN | RED; eff_col_state = STATE_GREEN; break;
									case STATE_GREEN: col_4 = BLUE; col_1 = GREEN; col_2 = GREEN | RED; col_3 = GREEN | RED; eff_col_state = STATE_YELLOW; break;
									case STATE_YELLOW: col_4 = GREEN; col_1 = GREEN | RED; col_2 = GREEN | RED; col_3 = GREEN | RED; eff_col_state = STATE_LAST; break;
									case STATE_LAST: col_4 = GREEN | RED; col_1 = GREEN | RED; col_2 = GREEN | RED; col_3 = GREEN | RED; eff_num_state = STATE_INIT2; break;
								}
				
			case STATE_CHANGE:	if(eff_min_tens < 5) {
									setMinutes(++eff_min_tens*10, col_2, col_2, 0);
									setSeconds(eff_min_tens*10, col_3, col_3, 0);
									if(eff_min_tens < 3) setHours(eff_min_tens*10, col_1, col_1, 0);
									SecRest(eff_min_tens*10, col_2, false);
									MinRest(eff_min_tens*10, col_1, false);
									if(eff_col_state > STATE_PINK) HrRest(eff_min_tens*10, col_4, false);
								} else if(eff_min_ones < 9) {
									setMinutes(eff_min_tens*10+(++eff_min_ones), col_2, col_2, 0);
									setSeconds(eff_min_tens*10+(eff_min_ones), col_3, col_3, 0);
									setHours(20+(eff_min_ones), col_1, col_1, 0);
									SecRest(eff_min_tens*10+eff_min_ones, col_2, false);
									MinRest(eff_min_tens*10+eff_min_ones, col_1, false);
									if(eff_col_state > STATE_PINK) HrRest(20+eff_min_ones, col_4, false);
								} else
									eff_num_state = STATE_COLOR;
								break;
								
			case STATE_INIT2:
								eff_sec_ones = eff_sec_tens = eff_min_ones = eff_min_tens = eff_hr_ones = eff_hr_tens = 0;
								col_1 = RED; col_2 = RED | BLUE; col_3 = GREEN | BLUE;
								mSeconds = 0;
								eff_col_state = STATE_RED;
								eff_num_state = STATE_REDSECS;
			/*case HRS:	if(eff_hr_tens < 2) {
				setHours(++eff_hr_tens*10, eff_color, eff_color, eff_color);
				if(eff_col_state != STATE_RED) HrRest(eff_hr_tens*10, eff_pre_color, false);
				} else if(eff_hr_ones < 9) {
				setHours(eff_hr_tens*10+(++eff_hr_ones), eff_color, eff_color, eff_color);
				if(eff_col_state != STATE_RED) HrRest(eff_hr_tens*10+eff_hr_ones, eff_pre_color, false);
			} else
			eff_num_state = INIT;
			break;
			
			case OFF:	if(eff_num_state == OFF) {
				if(eff_sec_tens < 5) {
					SecRest((++eff_sec_tens)*10, eff_pre_color, true);
					} else if(eff_sec_tens == 5 && eff_sec_ones < 9) {
					reset_led(SEC_ONES, 0xff, RED | GREEN | BLUE);
					SecRest(++eff_sec_ones, eff_pre_color, true);
					} else if(eff_min_tens < 5) {
					MinRest((++eff_min_tens)*10, eff_pre_color, true);
					} else if(eff_min_tens == 5 && eff_min_ones < 9) {
					MinRest(++eff_min_ones, eff_pre_color, true);
					} else if(eff_hr_tens < 2) {
					HrRest((++eff_hr_tens)*10, eff_pre_color, true);
					} else if(eff_hr_tens == 2 && eff_hr_ones < 9) {
					HrRest(++eff_hr_ones, eff_pre_color, true);
				}
				else
				eff_num_state = INIT;
			}
			break;*/
		}
	}
}