
#define F_CPU 1000000
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
char row[] = {1, 2, 4, 8, 16, 32, 64, 128};
typedef enum { O, I, L, J, S, Z,  T } shape_type;

/*
bool shape___array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
								   {FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
								   {FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
								   {FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
								   {FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};
*/
bool shape_O_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool shape_I_array[8][8]={{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE}};

bool shape_L_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool shape_J_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool shape_S_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool shape_Z_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool shape_T_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE,  TRUE,  TRUE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool current_shape_array[8][8];

int get_col(int row)
{
	int col_value = 0x00;
	for (int j=0; j<8; j++){
		if(current_shape_array[row][j] == TRUE){
			col_value |= 1<<(j);
		}
	}
	return col_value;
}

void UART_init(void){
	int UBRR_Value = 25; // 2400 baud rate

	UBRRH = (unsigned char) (UBRR_Value >> 8);

	UBRRL = (unsigned char) UBRR_Value;

	UCSRB = (1 << RXEN)| (1<TXEN);

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
	MCUCSR |= 1<<JTD;
	MCUCSR |= 1<<JTD;
    DDRD = 0b11111110;
    DDRC = 0xFF;
	DDRA = 0xFF;
	DDRB = 0xFF;
	UART_init();
	Lcd4_Init();
	char ch1 ='-';	//next shape
	int score_update = 0;
	int flag = 1;
    while (1) 
    {
		
	    int ch = UART_receive();
		if(ch == 10){
			score_update++;
		}
		else if(ch == 11){
			score_update += 8;
		}
		else if(ch == 9){
			flag = 0;
		}
		else if(ch == 7){
			flag = 1;
		}
		else if(ch == 0){
			//shape_O
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_O_array[i][j];
				}
			}
			ch1 = 'O';
		}
		else if(ch == 1){
			//shape_I
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_I_array[i][j];
				}
			}
			ch1 = 'I';
		}
		else if(ch == 2){
			//shape_L
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_L_array[i][j];
				}
			}
			ch1 = 'L';
		}
		else if(ch == 3){
			//shape_J
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_J_array[i][j];
				}
			}
			ch1 = 'J';
		}
		else if(ch == 4){
			//shape_S
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_S_array[i][j];
				}
			}
			ch1 = 'S';
		}
		else if(ch ==5){
			//shape_Z
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_Z_array[i][j];
				}
			}
			ch1 = 'Z';
		}
		else if(ch == 6){
			//shape_T
			for(int i = 0 ; i < 8; i++){
				for(int j = 0; j < 8; j++){
					current_shape_array[i][j] = shape_T_array[i][j];
				}
			}
			ch1 = 'T';
		}
		char score[3];	
		dtostrf(score_update , 3, 0, score);
		
		//char score_msg[16];
		//char second_msg[16];
		if(flag == 1 ){
			//Lcd4_Clear();
			char score_msg[] = " Score:         ";
			for(int score_i=0; score_i<3; score_i++){
				if(score[score_i] == '.')
					break;
				score_msg[8+score_i] = score[score_i];
			}
			char second_msg [] = " Next piece :   ";
			second_msg[14] = ch1;
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(score_msg);
			//Lcd4_Write_String(score);
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(second_msg);
		}
		
		else{
			//Lcd4_Clear();
			char score_msg[] = "Final Score:    ";
			for(int score_i=0; score_i<3; score_i++){
				if(score[score_i] == '.')
				break;
				score_msg[12+score_i] = score[score_i];
			}
			char second_msg[] = " Game Over!     ";
			Lcd4_Set_Cursor(1,0);
			Lcd4_Write_String(score_msg);
			//Lcd4_Write_String(score);
			Lcd4_Set_Cursor(2,0);
			Lcd4_Write_String(second_msg);
			score_update = 0;
		}
		/*
		for(int t = 0 ; t < 500; t++){
			for(int i=0; i<8; i++){
				PORTA = ~row[i];
				PORTB = get_col(i);
			}
		}*/	
		
		//_delay_us(10);
    }
}

