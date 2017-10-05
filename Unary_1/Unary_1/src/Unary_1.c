/*
 * Unary_1.c
 *
 * Created: 25.03.2015 17:16:02
 *  Author: Nicolas Dammin
 */ 


#include "main.h"

bool date = false;
uint8_t bySetType = 0;

uint8_t		g_byState = STATE_CLOCK;
uint8_t		g_byButtonFlag[3] = {0};
uint16_t	g_wButtonPressedCounter1 = 0;
uint16_t	g_wButtonPressedCounter2 = 0;
uint16_t	g_wButtonPressedCounter3 = 0;

int main(void)
{
	
	DDRD |= (1 << PD3) | (1 << PD4);
	DDRC &= ~(1 << PC2);
	PORTD = 0x00;// |= (1 << PD5);
	
	initSPI(); //initialize SPI for driving shift registers of the LEDs
	initTimer();
	initUART();
	dcf77_init();
	buttons_init();
	
	if(initTWI(100000))
	{
		//Read_DS1307();
	}
	else
	{
		writeUSART("Fehler! TWI-Bitrate zu hoch!\n");
	}
	
	/*new_time[0] = 00; //Seconds
	new_time[1] = 55; //Minutes
	new_time[2] = 19; //Hours
	new_time[3] = 00; //kp
	new_time[4] = 21; //Day
	new_time[5] = 9; //Month
	new_time[6] = 17; //Year
	Write_DS1307();*/
	
	testInit();
	Start_DS1307();
	
	Read_DS1307();
	
	sprintf(buffer, "Es ist der %02d.%02d.%02d um %02d:%02d:%02d Uhr.\n", rtc_data[4], rtc_data[5], rtc_data[6], rtc_data[2], rtc_data[1], rtc_data[0]);
	Debug_send_buffer();
	
	sei();
	
	g_byState = STATE_CLOCK_ENTRY;
	
    while(1)
    {
		//dcf77_run();
		
		/* Get button states */
		buttons_handler();
		
		/*if(PINC & (1 << PC2))
		{
			PORTD |= (1 << PD3);
		}
		else
		{
			PORTD &= ~(1 << PD3);
		}*/
		
		/* Main state machine */
		switch(g_byState)
		{
		/* Init Clock */
		case STATE_CLOCK_ENTRY:
			g_byState = STATE_CLOCK;
			break;
		/* Show main clock */
		case STATE_CLOCK:
			mainProcedure();
			/* Switch to Time setup */
			if(buttons_get_first())
			{
				/* Long press: Enter time setup mode */
				if(g_wButtonPressedCounter1 >= 5)
				{
					bySetType = TYPE_TIME;
					g_byState = STATE_LOAD;
				}
				if(g_byButtonFlag[0] != 1)
				{
					count_sec = 0;
					g_byState = STATE_DATE_ENTRY;
				}
				g_byButtonFlag[0] = 1;
			}
			else
			{
				g_wButtonPressedCounter1 = 0;
				g_byButtonFlag[0] = 0;
			}

			/* Switch to Off-Setup */
			if(buttons_get_second())
			{
				if(g_wButtonPressedCounter2 >= 5)
				{
					bySetType = TYPE_OFF_TIME;
					g_byState = STATE_LOAD_START;
				}
			}
			else
			{
				g_wButtonPressedCounter2 = 0;
			}
			
			/* Switch to Off-Setup */
			if(buttons_get_third())
			{
				if(g_wButtonPressedCounter3 >= 5)
				{
					bySetType = TYPE_ON_TIME;
					g_byState = STATE_LOAD_END;
				}
			}
			else
			{
				g_wButtonPressedCounter3 = 0;
			}
			break;
		/* Init date display */	
		case STATE_DATE_ENTRY:
			g_byState = STATE_DATE;
			break;
		/* Show date */	
		case STATE_DATE:
			if(count_sec >= 10) 
			{
				g_byState = STATE_CLOCK_ENTRY;
			} 
			else
			{
				showDate();
			}
			
			if(buttons_get_first())
			{
				/* Long press: Enter time setup mode */
				if(g_wButtonPressedCounter1 >= 5)
				{
					bySetType = TYPE_TIME;
					g_byState = STATE_LOAD;
				}
				g_byButtonFlag[0] = 1;
			}
			else
			{
				g_wButtonPressedCounter1 = 0;
				g_byButtonFlag[0] = 0;
			}
			break;	
		/* Set current date and time */	
		case STATE_SET_MONTH:
			blink(STATE_SET_MONTH);
			
			if(buttons_get_first())
			{
				if(g_byButtonFlag[0] != 1)
				{
					g_byState = STATE_SET_DAY;
					g_byButtonFlag[0] = 1;	
				}
			}
			else
			{
				g_byButtonFlag[0] = 0;
			}
			break;
		case STATE_SET_DAY:
			blink(STATE_SET_DAY);
			
			if(buttons_get_first())
			{
				if(g_byButtonFlag[0] != 1)
				{
					g_byState = STATE_SET_HOURS;
					g_byButtonFlag[0] = 1;
				}
			}
			else
			{
				g_byButtonFlag[0] = 0;
			}
			break;
		case STATE_SET_HOURS:
			blink(STATE_SET_HOURS);
			
			if(buttons_get_first())
			{
				if(g_byButtonFlag[0] != 1)
				{
					g_byState = STATE_SET_MINUTES;
					g_byButtonFlag[0] = 1;
				}
			}
			else
			{
				g_byButtonFlag[0] = 0;
			}
			break;
		case STATE_SET_MINUTES:
			blink(STATE_SET_MINUTES);
			
			if(buttons_get_first())
			{
				if(g_byButtonFlag[0] != 1)
				{
					switch(bySetType)
					{
						case TYPE_TIME:
						g_byState = STATE_STORE;
						break;
						case TYPE_OFF_TIME:
						g_byState = STATE_STORE_START;
						break;
						case TYPE_ON_TIME:
						g_byState = STATE_STORE_END;
						break;
						default:
						g_byState = STATE_CLOCK_ENTRY;
						break;
					}
					g_wButtonPressedCounter1 = 0;
					g_byButtonFlag[0] = 1;
				}
			}
			else
			{
				g_byButtonFlag[0] = 0;
			}
			break;
		case STATE_STORE:
			storeTime();
			g_byState = STATE_CLOCK_ENTRY;
			break;
		/* load current time from RTC */	
		case STATE_LOAD:
			loadTime();
			g_byState = STATE_SET_MONTH;
			break;
		/* load Off-Time from EEPROM */	
        case STATE_LOAD_START:
			loadOffTime();
			g_byState = STATE_SET_HOURS;
			break;
		/* Load On-Time from EEPROM */	
		case STATE_LOAD_END:
			loadOnTime();
			g_byState = STATE_SET_HOURS;
			break;
		/* Store Off-Time to EEPROM */
		case STATE_STORE_START:
			storeOffTime();
			g_byState = STATE_CLOCK_ENTRY;
			break;
		/* Store On-Time to EEPROM */	
		case STATE_STORE_END:
			storeOnTime();
			g_byState = STATE_CLOCK_ENTRY;
			break;		
		default:
			g_byState = STATE_CLOCK_ENTRY;
		}
		
		
    }
}