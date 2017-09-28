/*
 * Hardware.c
 *
 * Created: 27.03.2015 16:55:33
 *  Author: Nicolas Dammin
 */ 

#include "main.h"
#include "LowLevel.h"

char buffer[100];

uint32_t millisecs = 0;
uint32_t timeout = 0;

void initSPI(void)
{
	DDRB = 0xFF; //Set all Pins of Port B to output
	PORTB = 0x00; //Set all outputs of B to low
	
	//PORTB |= (1<<RES_PIN); //Reset high -> NO RESET
	
	//SPSR = 0x01; //Set SPI2X
	SPCR = (1<<SPIE) | (1<<SPE) | (1<<MSTR) | (1 << SPR0) | (1<<SPR1) ; //Enable SPI, set to master and Clock Rate 1/16 (1 MHz)
	
	SPDR = 0x00;
}

void SPI_Send(char data)
{
	SPDR = data; //send byte over SPI
	
	while(!(SPSR & (1<<SPIF))); //Wait untill Byte was sent
}

void initTimer(void) //Timer for 1 sec interrupts
{
	TCCR1B = (1 << WGM12) | (1 << CS12); //Select CTC mode (Compare Match) and enable Prescaler 1/256
	OCR1A = 63; //5684;						 //Compare-Match at 100ms
	
	TIMSK |= (1 << OCIE1A);
}

void initUART(void)
{
	UBRRL = 8;
	UBRRH = 0x00;
	UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);	
	
	UCSRB = (1 << RXEN) | (1 << TXEN);
}

//Zeichen über UART senden
void UART_Send(char c)
{
	while (!( UCSRA & (1 << UDRE)));
	UDR = c;
}

void writeUSART(char *c)
{
	while(*c != 0x00)
	{
		UART_Send(*c);
		c++;
	}
}

void Debug_send_buffer()
{
	int cnt = 0;
	while (buffer[cnt] != 0)
		UART_Send(buffer[cnt++]);
}

uint8_t initTWI (uint32_t TWI_Bitrate)
{
	/*
	** Set TWI bitrate
	** If bitrate is too high, then error return
	*/
	TWBR = ((F_CPU/TWI_Bitrate)-16)/2;
	if (TWBR < 11) return 0;

	return 1;
}

// START I2C Routine
unsigned char i2c_befehl(unsigned char typ, unsigned char rw) {
	switch(typ) {
		case I2C_START:  
			TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);  // Send Start Condition
			while (!(TWCR & (1 << TWINT)));
			if ((TWSR & 0xF8) != TW_START)
				return TWSR;
			break;
		case I2C_DATA:     // Send Data with No-Acknowledge
			TWCR &= ~(1 << TWEA);
			break;
		case I2C_DATA_ACK: // Send Data with Acknowledge
			TWCR |= (1 << TWEA);
			break;
		case I2C_STOP:     // Send Stop Condition
			TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		return 0;
	}
	
	return TWSR;
}



char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type, unsigned char restart)
{
	char r_val = -1;
	uint8_t		twst;

	if(restart)
	{;}
	

	r_val = i2c_befehl(I2C_START, rw_type);
	
	TWDR = (dev_id & 0xF0) | (dev_addr & 0x07) | rw_type;
	TWCR = (1 << TWINT) | (1 << TWEN);

	while (!(TWCR & (1 << TWINT)));
	
	twst = TWSR & 0xF8;
	if ((twst != TW_MT_DATA_ACK) && (twst != TW_MT_SLA_ACK)) return 0;
	
	r_val=0;

	return r_val;
}

void i2c_stop(void)
{
	// Transmit I2C Data
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
}

char i2c_write(char data)
{
	char r_val = -1;
	// Send the Data to I2C Bus
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); 
	
	// Transmit I2C Data
	while (!(TWCR & (1 << TWINT)));
	// Check the TWSR status
	if((TWSR & 0xF8) != TW_MT_DATA_ACK) return r_val;
	r_val=0;

	return r_val;
}

char i2c_read(char *data,char ack_type)
{
	char r_val = -1;

	//while (!(TWCR & (1 << TWINT)));
	
	//*data = TWDR;

	if (ack_type) {
		// Read I2C Data and Send Acknowledge
		TWCR |= (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
		timeout = 1000;
		while (timeout-- > 0 && !(TWCR & (1<<TWINT))); 
		*data = TWDR;
	} else {
		// Read I2C Data and Send No Acknowledge
		TWCR |= (1 << TWINT) | (1 << TWEN);
		timeout = 1000;
		while (timeout-- > 0 && !(TWCR & (1<<TWINT))); 
		*data = TWDR;
	}
	r_val=0;

	return r_val;
}

ISR(SPI_STC_vect)
{
	SPI_Interrupt();
}

ISR(TIMER1_COMPA_vect)
{
	dcf77_timer_handler();
	intMilliseconds();
	
	if(millisecs >= 1000)
	{
		intSeconds();
		millisecs = 0;
	}
	else
		millisecs++;
}