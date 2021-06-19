#define  F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
//char row[] = {1, 2, 4, 8, 16, 32, 64, 128};
char column1[8][8] = {{0, 0, 1, 1, 1, 0 , 0 , 0},
					 {0, 0 , 0, 0, 0, 1, 1, 1},
					 {0, 0, 1, 1, 1, 0 , 0 , 0},
					 {0, 0 , 0, 0, 0, 1, 1, 1},
					 {0, 0, 1, 1, 1, 0 , 0 , 0},
					 {0, 0 , 0, 0, 0, 1, 1, 1},
					 {0, 0, 1, 1, 1, 0 , 0 , 0},
					 {0, 0 , 0, 0, 0, 1, 1, 1}} ;	
						 
	
int main(void)
{
    DDRB = 0xFF;
	DDRD = 0xFF;
	DDRC = 0xFF;
    while (1) 
    {
		for(int i = 0 ; i < 8; i++ ){
			PORTB &= 0b11111000;
			PORTB |= (i & 0b00000111);
			
			for(int j = 0; j < 8; j++){
				if(column1[i][j] == 1){
					PORTD &= 0b11100000;
					PORTD |= (j & 0b00000111);
					_delay_us(500);
				}
				else{
					PORTD |= 1 << PD4;
				}
			}
			for(int j = 8; j < 16; j++){
				if(column1[i][j-8] == 1){
					PORTD &= 0b11100000;
					PORTD |= (j & 0b00001111);
					_delay_us(500);
				}
				else{
					PORTD |= 1 << PD4;
				}
			}
			for(int j = 0; j < 8; j++){
				if(column1[i][j] == 1){
					PORTC &= 0b11110000;
					PORTC |= (j & 0b00000111);
					_delay_us(500);
				}
				else{
					PORTC |= 1 << PC3;
				}
			}
			//_delay_ms(10);
		}
    }
}

