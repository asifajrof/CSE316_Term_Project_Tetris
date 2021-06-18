/*
 * mc1.c
 *
 * Created: 6/18/2021 1:07:35 PM
 * Author : ASUS
 */ 
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#define FOSC 1000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1
//#define USART_BAUDRATE 9600
#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

void UART_init(unsigned int ubrr)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);	/* Turn on transmission and reception */
	UCSRC |= (1 << URSEL) | (3 << UCSZ0) | (1 << USBS);/* Use 8-bit char size */
	UBRRL = ubrr;			/* Load lower 8-bits of the baud rate */
	UBRRH = (ubrr>> 8);		/* Load upper 8-bits*/
}

void USART_Transmit(unsigned char data){
	while(!(UCSRA & (1<<UDRE)));
	UDR = data;
}
int main(void)
{
    DDRA &= ~(1<<PA0);
	DDRD &= 0b11111101;
    PORTA = (1<<PA0);
	UART_init(MYUBRR);
	char ch = '0';
    while (1) 
    {
		 if(!(PINA & (1<<PA0))){
			USART_Transmit(ch);
			_delay_ms(200);
			ch++;
			if(ch == '7') ch = '0';
			
		 }
		 PORTD |= (1<<PD3) ;
			 
    }
}
