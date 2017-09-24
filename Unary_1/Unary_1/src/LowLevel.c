/*
 * LowLevel.c
 *
 * Created: 27.03.2015 16:52:12
 *  Author: Nicolas Dammin
 */ 

#include "main.h"
#include "LowLevel.h"
#include "MedLevel.h"

uint8_t startTime[2] EEMEM;
uint8_t endTime[2] EEMEM;

uint8_t sr[6] = {0}; //Variable for each shift register
uint8_t reg_nr = 0, reg_count = 0, pin_cnt = 0, left_shift = 0x00;

uint8_t led_r[6] = {0}, led_g[6] = {0}, led_b[6] = {0};
	
uint8_t test_var = 0;
	
uint8_t count = 0;

void Send_Register_Data(void)
{	
	for(uint8_t i = 0; i < 6; i++)
		SPI_Send(sr[5-i]);
		
	PORTB |= (1 << LATCH_PIN);
	_delay_us(1);
	PORTB &= ~(1<<LATCH_PIN);
}

void set_led(uint8_t reg, uint8_t bit, uint8_t color, uint8_t col_five)
{
	if(col_five != 0 && bit == FIVE)
	{
		if(col_five & RED)
		led_r[reg] |= FIVE;
		
		if(col_five & GREEN)
		led_g[reg] |= FIVE;
		
		if(col_five & BLUE)
		led_b[reg] |= FIVE;
		
		if(color & RED)
		led_r[reg] |= (bit & ~(FIVE));
		
		if(color & GREEN)
		led_g[reg] |= (bit & ~(FIVE));
		
		if(color & BLUE)
		led_b[reg] |= (bit & ~(FIVE));		
	}
	else
	{
		if(color & RED)
			led_r[reg] |= bit;
		
		if(color & GREEN)
			led_g[reg] |= bit;
		
		if(color & BLUE)
			led_b[reg] |= bit;
	}
}

void reset_led(uint8_t reg, uint8_t bit, uint8_t color)
{
	
	if(color & RED)
	led_r[reg] &= ~bit;
	
	if(color & GREEN)
	led_g[reg] &= ~bit;
	
	if(color & BLUE)
	led_b[reg] &= ~bit;
}

void SPI_Interrupt(void)
{	
	if(reg_count >= 6) //All 6 Bytes for all Registers sent
	{			
		if(!(led_r[reg_nr] & left_shift))PORTB |= (1 << RED_PIN);		//Check if red signal should be enabled for current selected LED
		if(!(led_g[reg_nr] & left_shift))PORTB |= (1 << GREEN_PIN);	//Check if green signal should be enabled for current selected LED
		if(!(led_b[reg_nr] & left_shift))PORTB |= (1 << BLUE_PIN);		//Check if blue signal should be enabled for current selected LED
		
		PORTB |= (1 << LATCH_PIN);	//Enable Latch -> Enable current LED
		reg_count = 0;				//Reset Register-Number to first one
		PORTB &= ~(1 << LATCH_PIN); //Disable Latch again
		
		PORTB &= ~(1 << RES_PIN);	//Reset all Registers
		PORTB |= (1<<RES_PIN);		//Deselect Reset-Signal
		
		_delay_us(3);
		
		if(led_r[reg_nr] & left_shift)PORTB &= ~(1 <<RED_PIN);		//Check if red signal should be disabled for current selected LED
		if(led_g[reg_nr] & left_shift)PORTB &= ~(1 << GREEN_PIN);	//Check if green signal should be disabled for current selected LED
		if(led_b[reg_nr] & left_shift)PORTB &= ~(1 << BLUE_PIN);	//Check if blue signal should be disabled for current selected LED
		
	}
	
	if(reg_nr >= 6)	//current selected Register
		reg_nr = 0;
		
	if(reg_count == reg_nr)	//Does the current byte correspond to selected Register ?
	{		
		if(sr[reg_nr] == 0)	//Register-Byte set to 0 ? -> Set first Bit
		{	
				left_shift = 1;			//first Bit selected otherwise
			
			sr[reg_nr] = left_shift; 
			
			SPDR = sr[reg_nr];		//write Byte to register
		}
		else if(sr[reg_nr] == 0x80)
		{
			left_shift = 0;				//register-value set to 0
			sr[reg_nr] = left_shift;
			SPDR = sr[reg_nr];			//write Byte to Register	
			reg_nr++;
		}
		else
		{
			if(reg_nr == 1 || reg_nr == 3 || reg_nr == 5) //check all tens, filter out pins that are not needed
			{
				if(left_shift == 0x10)
					left_shift <<= 2;
			}
			
			left_shift <<= 1;	//left shift Byte by one (next LED on Register)
			
			sr[reg_nr] = left_shift;
			SPDR = sr[reg_nr];	//write Byte to Register
		}
	}
	else
	{
		SPDR = 0x00;	//write 0 for all unselected Registers
	}
	
	if(reg_count == 0)
	{
		
	}
	
	reg_count++;	//next Register
}


void eep_get_time(uint8_t byTime, uint8_t *pbyHour, uint8_t *pbyMin)
{
	if(byTime == TIME_START)
	{
		*pbyHour = eeprom_read_byte(&startTime[0]);
		*pbyMin = eeprom_read_byte(&startTime[1]);
	}
	else
	{
		*pbyHour = eeprom_read_byte(&endTime[0]);
		*pbyMin = eeprom_read_byte(&endTime[1]);
	}
}

void eep_set_time(uint8_t byTime, uint8_t *pbyHour, uint8_t *pbyMin)
{
	if(byTime == TIME_START)
	{
		eeprom_update_byte(&startTime[0], *pbyHour);
		eeprom_update_byte(&startTime[1], *pbyMin);
	}
	else
	{
		eeprom_write_byte(&endTime[0], *pbyHour);
		eeprom_write_byte(&endTime[1], *pbyMin);
	}
}