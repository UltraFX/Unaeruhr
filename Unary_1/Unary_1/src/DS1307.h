/*
 * DS1307.h
 *
 *  Created on: 27.11.2012
 */

#ifndef DS1307_H_
#define DS1307_H_

char dec2bcd(char num);
char bcd2dec(char num);
int8_t Read_DS1307(void);
void Write_DS1307(void);
void Start_DS1307(void);

extern volatile char rtc_data[7];
extern char new_time[7];

#endif /* DS1307_H_ */
