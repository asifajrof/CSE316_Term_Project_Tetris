#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include "ADC_H.h"
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include "lcd.h"


#include <float.h>
#include<stdlib.h>

int main(void)
{
	DDRD = 0b11111110;
	DDRC = 0xFF;
	char buffer[20];
	int ADC_Value;
	ADC_Init();
	Lcd4_Init();
	int score_update = 0;
	Lcd4_Clear();
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
		
		ADC_Value = ADC_Read(0);		/* Read the status on X-OUT pin using channel 0 */
		sprintf(buffer, "X=%d   ", ADC_Value);
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(buffer);
		//LCD_String_xy(1, 0, buffer);
		
		ADC_Value = ADC_Read(1);		/* Read the status on Y-OUT pin using channel 0 */
		sprintf(buffer, "Y=%d   ", ADC_Value);
		Lcd4_Set_Cursor(2,1);
		Lcd4_Write_String(buffer);
		//LCD_String_xy(1, 8, buffer);

		ADC_Value = ADC_Read(2);		/* Read the status on SWITCH pin using channel 0 */
		if(ADC_Value < 600){
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String("Switch pressed   ");
		}
		//LCD_String_xy(2, 0,"Switch pressed   " );
		else{
			Lcd4_Set_Cursor(2,1);
			Lcd4_Write_String("Switch open      ");
		}
		//LCD_String_xy(2, 0, "Switch open      ");
	}
}