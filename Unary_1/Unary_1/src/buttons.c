/*
 * buttons.c
 *
 * Created: 23.04.2017 15:50:47
 *  Author: Nicolas Dammin
 */ 

#include "main.h"

uint8_t byaButtons[3];

void buttons_init(void)
{
	BUTTONS_DDR &= ~(HW_BUTTON0 | HW_BUTTON1 | HW_BUTTON2);
	BUTTONS_PORT |= (HW_BUTTON0 | HW_BUTTON1 | HW_BUTTON2);
}

void buttons_handler(void)
{
	byaButtons[2] = byaButtons[1];
	byaButtons[1] = byaButtons[0];
	byaButtons[0] = ~BUTTONS_PIN;
}

uint8_t buttons_get_first(void)
{
	uint8_t byRet = 0;
	
	if(((byaButtons[0] & HW_BUTTON0) && (byaButtons[1] & HW_BUTTON0)) ||
	   ((byaButtons[1] & HW_BUTTON0) && (byaButtons[2] & HW_BUTTON0)) ||
	   ((byaButtons[0] & HW_BUTTON0) && (byaButtons[2] & HW_BUTTON0)))
	{
		byRet = 1;
	}
	else 
	{
	}
	
	return byRet;
}

uint8_t buttons_get_second(void)
{
	uint8_t byRet = 0;
	
	if(((byaButtons[0] & HW_BUTTON1) && (byaButtons[1] & HW_BUTTON1)) ||
		((byaButtons[1] & HW_BUTTON1) && (byaButtons[2] & HW_BUTTON1)) ||
		((byaButtons[0] & HW_BUTTON1) && (byaButtons[2] & HW_BUTTON1)))
	{
		byRet = 1;
	}
	else
	{
	}
	
	return byRet;
}

uint8_t buttons_get_third(void)
{
	uint8_t byRet = 0;
	
	if(((byaButtons[0] & HW_BUTTON2) && (byaButtons[1] & HW_BUTTON2)) ||
		((byaButtons[1] & HW_BUTTON2) && (byaButtons[2] & HW_BUTTON2)) ||
		((byaButtons[0] & HW_BUTTON2) && (byaButtons[2] & HW_BUTTON2)))
	{
		byRet = 1;
	}
	else 
	{
	}
	
	return byRet;
}