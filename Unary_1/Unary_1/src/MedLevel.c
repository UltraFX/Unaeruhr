/*
 * MedLevel.c
 *
 * Created: 27.03.2015 16:52:25
 *  Author: Nicolas Dammin
 */ 

#include "main.h"
#include "LowLevel.h"
#include "MedLevel.h"

uint8_t current_seconds, current_minutes, current_hours;

void setSeconds(uint8_t seconds, uint8_t color_tens, uint8_t color_ones, uint8_t color_five)
{
	current_seconds = seconds;
	
	reset_led(SEC_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
	reset_led(SEC_ONES, 0xff, RED | GREEN | BLUE);
	
	//Ones - 1 is a special case (controlled by Tens)
	if(seconds % 10 >= 1)
		set_led(SEC_TENS, ONE, color_ones, 0);
		
	switch(seconds % 10)
	{
		case 9: set_led(SEC_ONES, NINE, color_ones, color_five);
		case 8: set_led(SEC_ONES, EIGHT, color_ones, color_five);
		case 7: set_led(SEC_ONES, SEVEN, color_ones, color_five);
		case 6: set_led(SEC_ONES, SIX, color_ones, color_five);
		case 5: set_led(SEC_ONES, FIVE, color_ones, color_five);
		case 4: set_led(SEC_ONES, FOUR, color_ones, color_five);
		case 3: set_led(SEC_ONES, THREE, color_ones, color_five);
		case 2: set_led(SEC_ONES, TWO, color_ones, color_five);
	}
	
	//Tens
	switch(seconds - (seconds%10))
	{
		case 50: set_led(SEC_TENS, FIVE_MS, color_tens, 0);
		case 40: set_led(SEC_TENS, FOUR_MS, color_tens, 0);
		case 30: set_led(SEC_TENS, THREE_MS, color_tens, 0);
		case 20: set_led(SEC_TENS, TWO_MS, color_tens, 0);
		case 10: set_led(SEC_TENS, ONE_MS, color_tens, 0);
	}
}

void setMinutes(uint8_t minutes, uint8_t color_tens, uint8_t color_ones, uint8_t color_five)
{
	current_minutes = minutes;
	
	reset_led(MIN_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
	reset_led(MIN_ONES, 0xff, RED | GREEN | BLUE);
	
	//Ones - 1 is a special case (controlled by Tens)
	if(minutes % 10 >= 1)
	set_led(MIN_TENS, ONE, color_ones, 0);
	
	switch(minutes % 10)
	{
		case 9: set_led(MIN_ONES, NINE, color_ones, color_five);
		case 8: set_led(MIN_ONES, EIGHT, color_ones, color_five);
		case 7: set_led(MIN_ONES, SEVEN, color_ones, color_five);
		case 6: set_led(MIN_ONES, SIX, color_ones, color_five);
		case 5: set_led(MIN_ONES, FIVE, color_ones, color_five);
		case 4: set_led(MIN_ONES, FOUR, color_ones, color_five);
		case 3: set_led(MIN_ONES, THREE, color_ones, color_five);
		case 2: set_led(MIN_ONES, TWO, color_ones, color_five);
	}
	
	//Tens
	switch(minutes - (minutes%10))
	{
		case 50: set_led(MIN_TENS, FIVE_MS, color_tens, 0);
		case 40: set_led(MIN_TENS, FOUR_MS, color_tens, 0);
		case 30: set_led(MIN_TENS, THREE_MS, color_tens, 0);
		case 20: set_led(MIN_TENS, TWO_MS, color_tens, 0);
		case 10: set_led(MIN_TENS, ONE_MS, color_tens, 0);
	}
}

void setHours(uint8_t hours, uint8_t color_tens, uint8_t color_ones, uint8_t color_five)
{
	current_hours = hours;
	
	reset_led(HR_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
	reset_led(HR_ONES, 0xff, RED | GREEN | BLUE);
	
	//Ones - 1 is a special case (controlled by Tens)
	if(hours % 10 >= 1)
	set_led(HR_TENS, ONE, color_ones, 0);
	
	switch(hours % 10)
	{
		case 9: set_led(HR_ONES, NINE, color_ones, color_five);
		case 8: set_led(HR_ONES, EIGHT, color_ones, color_five);
		case 7: set_led(HR_ONES, SEVEN, color_ones, color_five);
		case 6: set_led(HR_ONES, SIX, color_ones, color_five);
		case 5: set_led(HR_ONES, FIVE, color_ones, color_five);
		case 4: set_led(HR_ONES, FOUR, color_ones, color_five);
		case 3: set_led(HR_ONES, THREE, color_ones, color_five);
		case 2: set_led(HR_ONES, TWO, color_ones, color_five);
	}
	
	//Tens
	switch(hours - (hours%10))
	{
		case 20: set_led(HR_TENS, TWO_MS, color_tens, 0);
		case 10: set_led(HR_TENS, ONE_MS, color_tens, 0);
	}
}

void lightRest(uint8_t color)
{
	switch(current_seconds-current_seconds%10)
	{
	case 00: set_led(SEC_TENS, TN_ZERO_INV, color, 0); break;
	case 10: set_led(SEC_TENS, TN_ONE_INV, color, 0); break;
	case 20: set_led(SEC_TENS, TN_TWO_INV, color, 0); break;
	case 30: set_led(SEC_TENS, TN_THREE_INV, color, 0); break;
	case 40: set_led(SEC_TENS, TN_FOUR_INV, color, 0); break;
	}
	
	switch(current_minutes%10)
	{
	case 0: set_led(MIN_ONES, ZERO_INV, color, 0); set_led(MIN_TENS, ONE, color, 0); break;
	case 1: set_led(MIN_ONES, ONE_INV, color, 0); break;
	case 2: set_led(MIN_ONES, TWO_INV, color, 0); break;
	case 3: set_led(MIN_ONES, THREE_INV, color, 0); break;
	case 4: set_led(MIN_ONES, FOUR_INV, color, 0); break;
	case 5: set_led(MIN_ONES, FIVE_INV, color, 0); break;
	case 6: set_led(MIN_ONES, SIX_INV, color, 0); break;
	case 7: set_led(MIN_ONES, SEVEN_INV, color, 0); break;
	case 8: set_led(MIN_ONES, EIGHT_INV, color, 0); break;
	}
	
	switch(current_minutes-current_minutes%10)
	{
	case 00: set_led(MIN_TENS, TN_ZERO_INV, color, 0); break;
	case 10: set_led(MIN_TENS, TN_ONE_INV, color, 0); break;
	case 20: set_led(MIN_TENS, TN_TWO_INV, color, 0); break;
	case 30: set_led(MIN_TENS, TN_THREE_INV, color, 0); break;
	case 40: set_led(MIN_TENS, TN_FOUR_INV, color, 0); break;
	}
	
	switch(current_hours%10)
	{
	case 0: set_led(HR_ONES, ZERO_INV, color, 0); set_led(HR_TENS, ONE, color, 0); break;
	case 1: set_led(HR_ONES, ONE_INV, color, 0); break;
	case 2: set_led(HR_ONES, TWO_INV, color, 0); break;
	case 3: set_led(HR_ONES, THREE_INV, color, 0); break;
	case 4: set_led(HR_ONES, FOUR_INV, color, 0); break;
	case 5: set_led(HR_ONES, FIVE_INV, color, 0); break;
	case 6: set_led(HR_ONES, SIX_INV, color, 0); break;
	case 7: set_led(HR_ONES, SEVEN_INV, color, 0); break;
	case 8: set_led(HR_ONES, EIGHT_INV, color, 0); break;
	}
	
	switch(current_hours-current_hours%10)
	{
	case 00: set_led(HR_TENS, H_ZERO_INV, color, 0); break;
	case 10: set_led(HR_TENS, H_ONE_INV, color, 0); break;
	}
}

void SecRest(uint8_t seconds, uint8_t color, bool res) {
	
	if(res) {
		reset_led(SEC_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
		reset_led(SEC_ONES, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
	}
	
	if(seconds%10 == 0) {
		switch(seconds-seconds%10)
		{
			case 00: set_led(SEC_TENS, TN_ZERO_INV, color, 0); break;
			case 10: set_led(SEC_TENS, TN_ONE_INV, color, 0); break;
			case 20: set_led(SEC_TENS, TN_TWO_INV, color, 0); break;
			case 30: set_led(SEC_TENS, TN_THREE_INV, color, 0); break;
			case 40: set_led(SEC_TENS, TN_FOUR_INV, color, 0); break;
			case 50: set_led(SEC_TENS, 0, color, 0); break;
		}
	}
	
	switch(seconds%10)
	{
		case 0: set_led(SEC_ONES, ZERO_INV, color, 0); set_led(SEC_TENS, ONE, color, 0); break;
		case 1: set_led(SEC_ONES, ONE_INV, color, 0); break;
		case 2: set_led(SEC_ONES, TWO_INV, color, 0); break;
		case 3: set_led(SEC_ONES, THREE_INV, color, 0); break;
		case 4: set_led(SEC_ONES, FOUR_INV, color, 0); break;
		case 5: set_led(SEC_ONES, FIVE_INV, color, 0); break;
		case 6: set_led(SEC_ONES, SIX_INV, color, 0); break;
		case 7: set_led(SEC_ONES, SEVEN_INV, color, 0); break;
		case 8: set_led(SEC_ONES, EIGHT_INV, color, 0); break;
		case 9: set_led(SEC_ONES, NINE_INV, color, 0); break;
	}
}

void MinRest(uint8_t minutes, uint8_t color, bool res) {
	
	if(res) {
		reset_led(MIN_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
		reset_led(MIN_ONES, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
	}
	
	if(minutes%10 == 0) {
		switch(minutes-minutes%10)
		{
			case 00: set_led(MIN_TENS, TN_ZERO_INV, color, 0); break;
			case 10: set_led(MIN_TENS, TN_ONE_INV, color, 0); break;
			case 20: set_led(MIN_TENS, TN_TWO_INV, color, 0); break;
			case 30: set_led(MIN_TENS, TN_THREE_INV, color, 0); break;
			case 40: set_led(MIN_TENS, TN_FOUR_INV, color, 0); break;
			case 50: set_led(MIN_TENS, 0, color, 0); break;
		}
	}
	
	switch(minutes%10)
	{
		case 0: set_led(MIN_ONES, ZERO_INV, color, 0); set_led(MIN_TENS, ONE, color, 0); break;
		case 1: set_led(MIN_ONES, ONE_INV, color, 0); break;
		case 2: set_led(MIN_ONES, TWO_INV, color, 0); break;
		case 3: set_led(MIN_ONES, THREE_INV, color, 0); break;
		case 4: set_led(MIN_ONES, FOUR_INV, color, 0); break;
		case 5: set_led(MIN_ONES, FIVE_INV, color, 0); break;
		case 6: set_led(MIN_ONES, SIX_INV, color, 0); break;
		case 7: set_led(MIN_ONES, SEVEN_INV, color, 0); break;
		case 8: set_led(MIN_ONES, EIGHT_INV, color, 0); break;
		case 9: set_led(MIN_ONES, NINE_INV, color, 0); break;
	}
}

void HrRest(uint8_t hrs, uint8_t color, bool res) {
	
	if(res) {
		reset_led(HR_TENS, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
		reset_led(HR_ONES, 0xff, RED | GREEN | BLUE); //switch all Second-LEDs off to refresh
	}
	
	if(hrs%10 == 0) {
		switch(hrs-hrs%10)
		{
			case 00: set_led(HR_TENS, H_ZERO_INV, color, 0); break;
			case 10: set_led(HR_TENS, H_ONE_INV, color, 0); break;
			case 20: set_led(HR_TENS, 0, color, 0); break;
			case 30: set_led(HR_TENS, 0, color, 0); break;
			case 40: set_led(HR_TENS, 0, color, 0); break;
			case 50: set_led(HR_TENS, 0, color, 0); break;
		}
	}
	
	switch(hrs%10)
	{
		case 0: set_led(HR_ONES, ZERO_INV, color, 0); set_led(HR_TENS, ONE, color, 0); break;
		case 1: set_led(HR_ONES, ONE_INV, color, 0); break;
		case 2: set_led(HR_ONES, TWO_INV, color, 0); break;
		case 3: set_led(HR_ONES, THREE_INV, color, 0); break;
		case 4: set_led(HR_ONES, FOUR_INV, color, 0); break;
		case 5: set_led(HR_ONES, FIVE_INV, color, 0); break;
		case 6: set_led(HR_ONES, SIX_INV, color, 0); break;
		case 7: set_led(HR_ONES, SEVEN_INV, color, 0); break;
		case 8: set_led(HR_ONES, EIGHT_INV, color, 0); break;
		case 9: set_led(HR_ONES, NINE_INV, color, 0); break;
	}
}