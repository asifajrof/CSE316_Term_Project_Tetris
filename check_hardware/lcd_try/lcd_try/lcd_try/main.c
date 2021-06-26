
#define F_CPU 1000000
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <float.h>
#include<stdlib.h>


int main(void)
{
	DDRD = 0b11111110;
	DDRC = 0xFF;
	Lcd4_Init();
	int score_update = 0;
	while(1)
	{
		char score[10];
		dtostrf(score_update , 0, 2, score);
		char msg[] = "Score : ";
		char msg1[] = "Next piece :   ";
		msg1[13] = 'T';
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(msg);
		Lcd4_Write_String(score);
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(msg1);
		
	}
}