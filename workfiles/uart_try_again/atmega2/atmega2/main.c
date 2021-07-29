/*
 * atmega2.c
 *
 * Created: 7/15/2021 5:49:05 PM
 * Author : asifajrof
 */ 

#include <avr/io.h>


int main(void)
{
	DDRB |= (1<<PINB0);
	
	//communication UART specifications (parity, stop bits, data bit length)
	int UBRR_Value = 6; // this is for 2400 baud
	
	UBRRH = (unsigned char) (UBRR_Value >> 8);
	UBRRL = (unsigned char) UBRR_Value;
	UCSRB = (1<<RXEN) | (1<TXEN);
	UCSRC = (1<<USBS) | (3<<UCSZ0);
	
	unsigned char receiveData;
	
    while (1){
		while(!(UCSRA & (1<<RXC)));
		receiveData = UDR;
		
		if(receiveData == 0xF0) PORTB ^=(1<<PINB0);
    }
}

