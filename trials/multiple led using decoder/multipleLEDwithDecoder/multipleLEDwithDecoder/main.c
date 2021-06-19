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
	int delay_D1 = 5000; 
	//int delay_D2 = 1000;
	//int delay_C = 000;
	//int delay__ = 100;
    DDRB = 0xFF;
	DDRD = 0xFF;
	DDRC = 0xFF;
    while (1) 
    {
		for(int j = 0 ; j < 16; j++ ){
			if(j<8){
				PORTD &= 0b11100000;
				PORTD |= (j & 0b00000111);
				for(int i = 0; i < 8; i++){
					if(column1[i][j] == 1){
						PORTB &= 0b11111000;
						PORTB |= (i & 0b00000111);
						_delay_us(delay_D1);
					}
					else{
						PORTD |= 1 << PD4;
					}
				}
			}
			else{
				
			}
			_delay_us(1000);
		}
    }
}

