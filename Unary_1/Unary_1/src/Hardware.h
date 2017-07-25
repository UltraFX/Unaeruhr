/*
 * Hardware.h
 *
 * Created: 27.03.2015 16:55:40
 *  Author: Nicolas Dammin
 */ 


#ifndef HARDWARE_H_
#define HARDWARE_H_

#define HOUR_24 0
#define HOUR_12 1

#define DS1307_ID    0xD0        // I2C DS1307 Device Identifier
#define DS1307_ADDR  0x00        // I2C DS1307 Device Address
#define I2C_START 0
#define I2C_DATA 1
#define I2C_DATA_ACK 2
#define I2C_STOP 3
#define ACK 1
#define NACK 0


/*#define START		0x08
#define REP_START	0x10
#define AD_ACK		0x18
#define AD_NACK		0x20
#define DAT_ACK		0x28
#define DAT_NACK	0x30
#define LOSS		0x38*/

void initSPI(void);
void SPI_Send(char data);
void initTimer(void);
void initUART(void);
uint8_t initTWI (uint32_t TWI_Bitrate);

void UART_Send(char c);
void writeUSART(char *c);
void Debug_send_buffer();

unsigned char i2c_befehl(unsigned char typ, unsigned char rw);
char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type, unsigned char restart);
void i2c_stop(void);
char i2c_write(char data);
char i2c_read(char *data, char ack_type);

extern char buffer[100];

#endif /* HARDWARE_H_ */