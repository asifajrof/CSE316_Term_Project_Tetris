#define  F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
char row[] = {1, 2, 4, 8, 16, 32, 64, 128};
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
		for(int i = 1 ; i <= 8; i++ ){
			PORTB = i;
			for(int j = 0; j < 8; j++){
				if(column1[i-1][j] == 1){
					PORTD = ~j;
					//_delay_ms(10);
				}
			}
			for(int j = 8; j < 16; j++){
				if(column1[i-1][j-8] == 0){
					PORTD = ~j;
					//_delay_ms(10);
				}
			}
			for(int j = 0; j < 8; j++){
				if(column1[i-1][j]){
					PORTC = ~j;
					//_delay_ms(10);
				}
			}
			//_delay_ms(10);
		}
    }
}

