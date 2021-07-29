#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

volatile char row[] = {1, 2, 4, 8, 16, 32, 64, 128};

int main(void)
{
    /* Replace with your application code */
	
	MCUCSR |= 1<<JTD;
	MCUCSR |= 1<<JTD;
	
	DDRA = 0xF0;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xF0;
	uint8_t led_col = 0x00;
	int i = 7;
	
    while (1) 
    {
		PORTC = 0x00; // common row connection
		PORTC |= row[i]; // common row connection
		PORTB = 0x00; // upper matrix column
		// lower matrix column
		led_col = ~(0b01010101);
		PORTA &= 0x0F;
		PORTD &= 0x0F;
		PORTA |= (led_col & 0x0F) << 4;
		PORTD |= (led_col & 0xF0);
		
		_delay_ms(2);
		PORTB = ~0x0; // upper matrix column
		//PORTA = ~0x0; // lower matrix column
		PORTA |= 0xF0;
		PORTD |= 0xF0;
		
		if(i == 7) i = 0;
		else i++;
    }
}

