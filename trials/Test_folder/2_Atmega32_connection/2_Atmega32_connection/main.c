#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


void UART_init(void){
	
	int UBBRValue = 25;//AS described before setting baud rate

	//Put the upper part of the baud number here (bits 8 to 11)

	UBRRH = (unsigned char) (UBBRValue >> 8);

	//Put the remaining part of the baud number here

	UBRRL = (unsigned char) UBBRValue;

	//Enable the receiver and transmitter

	UCSRB = (1 << TXEN)|(1 << RXEN);

	//Set 2 stop bits and data bit length is 8-bit

	//UCSRC = (1 << USBS) | (3 << UCSZ0);
	UCSRC = 0b10001110;
}

void UART_send(unsigned char data){		// wait until UDRE flag is set to logic 1		while ((UCSRA & (1<<UDRE)) == 0x00);		UDR = data; // Write character to UDR for transmission	}


int main(void)

{

	DDRD = 0b11111110;
	DDRC = 0b00000000;
	char ch = '0';
	UART_init();
	while (1)

	{
		PORTD = 0x00;
		
		if ((PINC&(1<<PC0))){

			UART_send(ch);
			ch++;
			if(ch == '8') ch = '0';
			PORTD |= 0b00000010;
			_delay_ms(2000);
			PORTD = 0x00;
			
		}
		
	}
}
