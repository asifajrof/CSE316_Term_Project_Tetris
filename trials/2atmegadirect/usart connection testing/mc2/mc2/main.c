#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0b11110000;
	DDRC = 0b11111111;
	
	while (1)
	{
		if(PIND & 0b00000001){
			PORTC = 0x00;
			PORTC |= (1<<PC1);
			_delay_ms(350);
		}
		else if(PIND & 0b00000010){
			PORTC = 0x00;
			PORTC |= (1<<PC2);
			_delay_ms(350);
		}
		else if(PIND & 0b00000011){
			PORTC = 0x00;
			PORTC |= (1<<PC3);
			_delay_ms(350);
		}
		else if(PIND & 0b00000100){
			PORTC = 0x00;
			PORTC |= (1<<PC4);
			_delay_ms(350);
		}
		else if(PIND & 0b00000101){
			PORTC = 0x00;
			PORTC |= (1<<PC5);
			_delay_ms(350);
		}
		else if(PIND & 0b00000110){
			PORTC = 0x00;
			PORTC |= (1<<PC6);
			_delay_ms(350);
		}
		else if(PIND & 00000111){
			PORTC = 0x00;
			PORTC |= (1<<PC7);
			_delay_ms(350);
		}
		
	}
}
