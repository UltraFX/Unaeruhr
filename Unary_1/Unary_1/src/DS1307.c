/*
 * DS1307.c
 *
 *  Created on: 27.11.2012
 */

#include "main.h"

volatile char rtc_data[7];
uint8_t hour_mode = 0, ampm_mode = 0;
char new_time[7];

// Convert Decimal to Binary Coded Decimal (BCD)
char dec2bcd(char num)
{
  return ((num/10 * 16) + (num % 10));
}

// Convert Binary Coded Decimal (BCD) to Decimal
char bcd2dec(char num)
{
  return ((num/16 * 10) + (num % 16));
}

int8_t Read_DS1307(void)
{
	#ifdef DEBUG_ENABLE
		//writeUSART("Get Time Data from DS1307...\n");
	#endif
	
   char data;
   int8_t state = 0;
   // First we initial the pointer register to address 0x00
   // Start the I2C Write Transmission
   if( (state = i2c_start(DS1307_ID,DS1307_ADDR,TW_WRITE, 0)) )
   {
	   if(state == -1)
		writeUSART("Fehler beim Verbindungsaufbau! (ARB-Fehler, BUS-Error\n");
	   else
		writeUSART("Fehler beim Verbindungsaufbau! (kein ACK)\n");	
   }

    // Start from Address 0x00
   if(i2c_write(0x00))
   {
		writeUSART("Fehler beim Schreiben!\n");
		return -1;
   }
   
   // Stop I2C Transmission
   i2c_stop();

   // Start the I2C Read Transmission
   if(i2c_start(DS1307_ID,DS1307_ADDR,TW_READ, 0))
	{
		if(state == -1)
		writeUSART("Fehler beim Verbindungsaufbau! (ARB-Fehler, BUS-Error\n");
		else
		writeUSART("Fehler beim Verbindungsaufbau! (kein ACK)\n");
		return -1;
	}
	 
	i2c_read(&data,ACK);

	rtc_data[0]=bcd2dec(data & 0x7F);

	// Read the Minute Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	rtc_data[1]=bcd2dec(data);
	// Read the Hour Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	if ((data & 0x40) == 0x40) {
		hour_mode = HOUR_12;
		ampm_mode=(data & 0x20) >> 5;   // ampm_mode: 0-AM, 1-PM
		rtc_data[2]=bcd2dec(data & 0x1F);
		} else {
		hour_mode = HOUR_24;
		ampm_mode=0;
		rtc_data[2]=bcd2dec(data & 0x3F);
	}
	// Read the Day of Week Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	rtc_data[3]=bcd2dec(data);
	// Read the Day of Month Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	rtc_data[4]=bcd2dec(data);
	// Read the Month Register, Send Master Acknowledge
	i2c_read(&data,ACK);
	rtc_data[5]=bcd2dec(data);

	// Read the Year Register, Send Master No Acknowledge
	i2c_read(&data,NACK);
	rtc_data[6]=bcd2dec(data);
	
   // Stop I2C Transmission
   i2c_stop();
   
   return 0;
}


void Write_DS1307(void)
{
   unsigned char i, hour_format;
   // Make sure we enable the Oscillator control bit CH=0 on Register 0x00
   new_time[0]=new_time[0] & 0x7F;
   // Start the I2C Write Transmission
   i2c_start(DS1307_ID,DS1307_ADDR,TW_WRITE, 0);
   // Start from Address 0x00
   i2c_write(0x00);
   // Write the data to the DS1307 address start at 0x00
   // DS1307 automatically will increase the Address.

   for (i=0; i<7; i++) {
     if (i == 2) {
	   hour_format=dec2bcd(new_time[i]);
	   if (hour_mode) {
	     hour_format |= (1 << 6);
   	     if (ampm_mode)
	       hour_format |= (1 << 5);
         else
	       hour_format &= ~(1 << 5);
	   } else {
	     hour_format &= ~(1 << 6);
       }
	   i2c_write(hour_format);
	 } else {
       i2c_write(dec2bcd(new_time[i]));
     }
   }
   i2c_write(0xFF); //Enable 1Hz Squarewave generation (interrupt)
   // Stop I2C Transmission

   i2c_stop();
}

void Start_DS1307(void)
{
	Read_DS1307();

	rtc_data[0]=rtc_data[0] & 0x7F;

	// Start the I2C Write Transmission
	i2c_start(DS1307_ID,DS1307_ADDR,TW_WRITE, 0);
	// Start from Address 0x00
	i2c_write(0x00);

	i2c_write(rtc_data[0]);

	/*i2c_write(0xFF); //Enable 1Hz Squarewave generation (interrupt)
	// Stop I2C Transmission
	*/

	i2c_stop();
}

