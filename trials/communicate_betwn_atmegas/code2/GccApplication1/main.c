#include <avr/io.h>

#define F_CPU 1000000
#include <util/delay.h>

#define LED_BIT 1
#define INP_BIT 0

int main(void)
{
	DDRA  = (1 << LED_BIT); // PA1 output
	PORTA &= ~(1 << INP_BIT); // set PA0 0

	while(1)
	{
		if (PINA & (1 << INP_BIT))  // signal found
			PORTA |= (1 << LED_BIT);   // turn led on
		else
			PORTA &= ~(1 << LED_BIT);   // turn led on
		_delay_ms(100);
	}
}