
#include <avr/io.h>

#define F_CPU 1000000

#include <util/delay.h>
#include <avr/interrupt.h>


ISR(INT0_vect)
{
	PORTA |= 1<<PA0;
	_delay_ms(1000);
	PORTA &= ~(1<<PA0);
}


int main(void)
{
	DDRD=0x00;
	PORTD=0xFF;
	
	GICR = 1<<INT0;
	MCUCR = 1<<ISC01 | 1<<ISC00;
	
	sei();
	
	DDRA = 0xFF;
	PORTA = 0x00;

	while(1)
	{
		_delay_ms(1);
	}
}