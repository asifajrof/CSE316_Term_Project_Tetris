
#define F_CPU 8000000
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <float.h>
#include<stdlib.h>
typedef uint8_t bool;
#define FALSE 0x00
#define TRUE 0xFF
volatile char row[] = {1, 2, 4, 8, 16, 32, 64, 128};
typedef enum { O, I, L, J, S, Z,  T } shape_type;

/*
volatile bool shape___array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
								   {FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
								   {FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
								   {FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};
*/
volatile bool shape_O_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_I_array[8][8]={{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE}};

volatile bool shape_L_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_J_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_S_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_Z_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_T_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

volatile bool current_shape_array[8][8];

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
    DDRD = 0b11111110;
    DDRC = 0xFF;
	DDRA = 0xFF;
	DDRB = 0xFF;
	UART_init();
	Lcd4_Init();
	int i = 0;
	int score_update = 0;
    while (1) 
    {
		PORTA = ~row[i];
		//PORTB = ;
		i++;
		if( i > 7) i = 0;
		int ch = UART_receive();
		if(ch == 10){
			score_update++;
		}
		else if(ch == 11){
			score_update += 4;
		}
		else if(ch == 0){
			
		}
		else if(ch == 1){
			
		}
		else if(ch == 2){
			
		}
		else if(ch == 3){
			
		}
		else if(ch == 4){
			
		}
		else if(ch ==5){
			
		}
		else if(ch == 6){
			
		}
		char score[10];
		dtostrf(score_update , 0, 2, score);
		char msg[] = "Score : ";
		Lcd4_Set_Cursor(1,1);
		Lcd4_Write_String(msg);
		Lcd4_Write_String(score);
    }
}

