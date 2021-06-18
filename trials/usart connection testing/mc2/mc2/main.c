#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#define FOSC 1000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

void UART_init(unsigned int ubrr)
{
	UBRRH = (ubrr >> 8);	/* Load upper 8-bits */
	UBRRL = ubrr;		/* Load lower 8-bits of the baud rate */
	
	UCSRB |= (1 << RXEN) ;/* Turn on the transmission and reception */
	UCSRC |= (1 << URSEL) | (3 << UCSZ0) | (1 << USBS);/* Use 8-bit character sizes */

	
}


unsigned char USART_Receive(){
	while(!(UCSRA & (1 << RXC)));
	return UDR;
}
int main(void)
{
    DDRC = 0xFF;
	DDRD &= 0b11111100;
	UART_init(MYUBRR);
	
    while (1) 
    {
		PORTD |= (1<< PD3);
		char ch = USART_Receive();
		if(ch == '0'){
			PORTC = 0x00;
			PORTC |= (1<<PC1);
			_delay_ms(200);
		}
		else if(ch == '1'){
			PORTC = 0x00;
			PORTC |= (1<<PC2);
			_delay_ms(100);
		}
		else if(ch == '2'){
			PORTC = 0x00;
			PORTC |= (1<<PC3);
			_delay_ms(200);
		}
		else if(ch == '3'){
			PORTC = 0x00;
			PORTC |= (1<<PC4);
			_delay_ms(200);
		}
		else if(ch == '4'){
			PORTC = 0x00;
			PORTC |= (1<<PC5);
			_delay_ms(200);
		}
		else if(ch == '5'){
			PORTC = 0x00;
			PORTC |= (1<<PC6);
			_delay_ms(200);
		}
		else if(ch == '6'){
			PORTC = 0x00;
			PORTC |= (1<<PC7);
			_delay_ms(200);
		}
		else if(ch == '7'){
			PORTC = 0x00;
			PORTC |= (1<<PC1);
			_delay_ms(200);
		}
		
    }
}

