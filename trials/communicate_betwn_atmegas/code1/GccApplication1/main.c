#include <avr/io.h>

#define F_CPU 1000000
typedef uint8_t bool;
#define FALSE 0x00;
#define TRUE 0xFF;

#include <util/delay.h>

int main(void)
{
	DDRD=0xFF;
	PORTD=0x00;

	while(1)
	{
		
		PORTD |= 1<<PD2;
		_delay_ms(200);
		PORTD &= ~(1<<PD2);
		_delay_ms(200);
	}
}