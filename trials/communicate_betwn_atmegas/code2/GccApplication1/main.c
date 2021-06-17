/*
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
*/

/*
1705092
symbol => 2
rotate => RIGHT
intrpt => INT0
triggr => rising edge
*/

#include <avr/io.h>

#define F_CPU 1000000
#define RIGHT 1
#define LEFT -1
typedef uint8_t bool;
#define FALSE 0x00;
#define TRUE 0xFF;

#include <util/delay.h>
#include <avr/interrupt.h>

bool rotate = FALSE;

ISR(INT0_vect)
{
	rotate = ~rotate;
	_delay_ms(1);
}

void rotateSymbol(uint8_t *symbol, int length, int direction)
{
	int index = 0;
	if (direction>0){index = length-1;}
	uint8_t temp = symbol[index];
	for(int i=0;i<length;i++)
	{
		index -= direction;
		symbol[index+direction] = symbol[index];
	}
	symbol[index+direction] = temp;
}

int main(void)
{
	DDRD=0x00;
	PORTD=0xFF;
	
	GICR = 1<<INT0;
	MCUCR = 1<<ISC01 | 1<<ISC00;
	
	sei();
	
	DDRA = 0xFF;
	DDRB = 0xFF;
	

	int length=8;	//matrix length each side
	uint8_t colPin[] = {(1<<PB7),(1<<PB6),(1<<PB5),(1<<PB4),(1<<PB3),(1<<PB2),(1<<PB1),(1<<PB0)};
	uint8_t symbol[]=
	{0b00000000,
		0b11000110,
		0b11100111,
		0b11110001,
		0b11011001,
		0b11001111,
		0b11000110,
	0b00000000};//binary codes for symbol pattern
	int j=0;
	
	while(1)
	{
		if (rotate){rotateSymbol(symbol,length,RIGHT);}
		for(int i=0;i<50;i++)
		{
			PORTA = colPin[j];
			PORTB = ~symbol[j];
			_delay_ms(5);
			j++;
			j%=length;
			
		}
	}
}