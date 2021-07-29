
#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>


void UART_init(void){
	int UBRR_Value = 25; // 2400 baud rate

	UBRRH = (unsigned char) (UBRR_Value >> 8);

	UBRRL = (unsigned char) UBRR_Value;

	UCSRB = (1 << RXEN);

	UCSRC = (1 << USBS) | (3 << UCSZ0);
	//UCSRC = 0b10001110;	
}
unsigned char UART_receive(void){	
	// Wait until RXC flag is set to logic 1	
	while ((UCSRA & (1<<RXC)) == 0x00);	
	return UDR; // Read the received character from UDR
}
int main(void)
{

	DDRD = 0b11111110;//PORTD pin0 as INPUT
	
	DDRC = 0xFF;//PORTB as OUTPUT
	

	//unsigned char receiveData;
//	while ((UCSRA & (1 << RXC)) == 0x00);
	//receiveData = UDR;
	UART_init();
	while (1)

	{
		PORTD = 0b11111100;
		char ch = UART_receive();
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
			PORTC |= (1<<PC0);
			_delay_ms(200);
		}
	}

}