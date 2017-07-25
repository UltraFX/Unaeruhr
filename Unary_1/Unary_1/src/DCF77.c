/*
 * DCF77.c
 *
 * Created: 21.04.2017 23:35:52
 *  Author: Nicolas Dammin
 */ 

#include "main.h"

uint16_t wCnt_Sig = 0;
uint16_t wCnt_Zero = 0;

uint8_t byaTime[60] = {0};
uint8_t byTimeCnt = 0;

/*  local function prototypes */
static uint8_t dcf77_signal(void);

/* function definitions */
void dcf77_init(void)
{
	/* init pins */
	DDRC &= ~(1 << PC7);
	PORTC &= ~((1 << PC6));
	//PORTC |= (1 << PC7);
	
	/* init timer */
	/*TCCR1B = (1 << WGM12) | (1 << CS12); //Select CTC mode (Compare Match) and enable Prescaler 1/256
	OCR1A = 5684;						 //Compare-Match at 100ms
	
	TIMSK |= (1 << OCIE1A);*/
}

void dcf77_run(void)
{
	if(dcf77_signal() != 0)
	{	
		PORTD |= (1 << PD3);
		if(wCnt_Zero >= 950)
		{
			wCnt_Sig = 0;
		}
		else
		{
			wCnt_Sig = 0;
			PORTD &= ~(1 << PD4);
		}
	}
	else
	{
		PORTD &= ~(1 << PD3);
		wCnt_Zero = 0;
		
		if(wCnt_Sig > 95 && wCnt_Sig < 105)
		{
			byaTime[byTimeCnt] = 0;
		}
		else if(wCnt_Sig > 195 && wCnt_Sig < 205)
		{
			byaTime[byTimeCnt] = 1;
		}
		else if(wCnt_Sig > 1995 && wCnt_Sig < 2005)
		{
			byTimeCnt = 0;
			PORTD |= (1 << PD4);
		}
	}
}

static uint8_t dcf77_signal(void)
{
	return PINC & (1 << PC7);
}

void dcf77_timer_handler(void)
{
	wCnt_Sig++;
	wCnt_Zero++;
}
