/*
 * atmega1.c
 *
 * Created: 7/15/2021 5:20:19 PM
 * Author : asifajrof
 */ 

#include <avr/io.h>


int main(void)
{
    //set led pin for output and turn off led
	DDRB |= (1<<PINB1);
	PORTB &= ~(1<<PINB1);
	
	//initialization of button and debouncing variables
	DDRB &= ~(1<<PINB0);
	PORTB |= (1<<PINB0);
	int pressed = 0;
	int pressed_confidence_level = 0;
	int released_confidence_level = 0;
	
	//communication uart specifications (parity, stop bits, data bit length)
	int UBRR_Value = 6;
	UBRRH = (unsigned char) (UBRR_Value >> 8);
	UBRRL = (unsigned char) UBRR_Value;
	UCSRB = (1<<RXEN) | (1<<TXEN);
	UCSRC = (1<<USBS) | (3<<UCSZ0);
	
    while (1){
		if(bit_is_clear(PINB,0)){
			pressed_confidence_level++;
			released_confidence_level = 0;
			if(pressed_confidence_level > 100){
				if(pressed == 0){
					pressed = 1;
					PORTB ^= (1<<PINB1);
					//send data to recieving uC
					while(!(UCSRA&(1<<UDRE)));
					UDR = 0xF0;
				}
				pressed_confidence_level = 0;
			}
		}
		else{
			released_confidence_level++;
			pressed_confidence_level = 0;
			if(released_confidence_level > 100){
				pressed = 0;
				released_confidence_level = 0;
			}
		}
    }
}

