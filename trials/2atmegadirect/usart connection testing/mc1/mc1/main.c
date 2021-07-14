/*
 * mc1.c
 *
 * Created: 6/18/2021 1:07:35 PM
 * Author : ASUS
 */ 
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0b11111111;
	DDRA = 0b00000000;
	int n = 1;
    while (1) 
    {
		 if(!(PINA & (1<<PA0))){
			PORTD = n;
			_delay_ms(200);
			PORTD = 0;
			_delay_ms(200);
			n++;
			if(n == 9) n = 1;
			
		 }
			 
    }
}
