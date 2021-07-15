
#define F_CPU 1000000
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
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
/*volatile bool shape_O_array[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
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
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};*/
volatile bool shape_O_array[4][4]={{FALSE,  TRUE,  TRUE, FALSE},
{FALSE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_I_array[4][4]={{FALSE,  TRUE, FALSE, FALSE},
{FALSE,  TRUE, FALSE, FALSE},
{FALSE,  TRUE, FALSE, FALSE},
{FALSE,  TRUE, FALSE, FALSE}};

volatile bool shape_L_array[4][4]={{FALSE,  TRUE, FALSE, FALSE},
{FALSE,  TRUE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_J_array[4][4]={{FALSE, FALSE,  TRUE, FALSE},
{FALSE, FALSE,  TRUE, FALSE},
{FALSE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_S_array[4][4]={{FALSE,  TRUE,  TRUE, FALSE},
{ TRUE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_Z_array[4][4]={{ TRUE,  TRUE, FALSE, FALSE},
{FALSE,  TRUE,  TRUE, FALSE},
{FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE}};

volatile bool shape_T_array[4][4]={{ TRUE,  TRUE,  TRUE, FALSE},
{FALSE,  TRUE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE}};
bool current_shape_array[4][4];
bool current_display[8][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
};
int current_shape = -1;
volatile int current_R = 2;
volatile int current_C = 2;
char ch = 'x';
/*int get_col(int row)
{
	int col_value = 0x00;
	for (int j=0; j<8; j++){
		if(current_shape_array[row][j] == TRUE){
			col_value |= 1<<(j);
		}
	}
	return col_value;
}*/

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
ISR(USART_RXC_vect)
{
	ch = UDR;
}
void generate_shape(int shape){
	//int shape = 0;
	//int shape = rand()%7;
	current_shape = shape;
	if( shape == 0){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_O_array[i][j];
			}
		}
	}
	else if(shape == 1){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_I_array[i][j];
			}
		}
	}
	else if(shape == 2){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_L_array[i][j];
			}
		}
	}
	else if(shape == 3){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_J_array[i][j];
			}
		}
	}
	else if(shape == 4){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_S_array[i][j];
			}
		}
	}
	else if(shape == 5){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_Z_array[i][j];
			}
		}
	}
	else if(shape == 6){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_T_array[i][j];
			}
		}
	}
}
int get_col(int row)
{
	int col_value = 0x00;
	for (int j=0; j<8; j++){
		if(current_display[row][j] == TRUE){
			col_value |= 1<<(j);
		}
	}
	return col_value;
}
void set_shape(bool shape_array[][4])
{
	for(int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if(current_shape_array[i][j] == TRUE){
				current_display[current_R+i][current_C+j] = current_shape_array[i][j];
			}
		}
	}
	//_delay_ms(50);
}
void remove_shape(bool shape_array[][4])
{
	for(int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if(shape_array[i][j] == TRUE){
				current_display[current_R+i][current_C+j] = FALSE;
			}
		}
	}
	//_delay_ms(100);
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
	sei();
	char ch1 ='-';	//next shape
	int score_update = 0, i = 0;
    while (1) 
    {
		PORTA = ~row[i];
		PORTB = get_col(i);
		i++;
		if(i == 8) i = 0;
		int flag = 1;
	   // int ch = UART_receive();
		if(ch == 10){
			score_update++;
		}
		else if(ch == 11){
			score_update += 8;
		}
		else if(ch == 9){
			flag = 0;
		}
		/*else if(ch > -1 && ch < 7){
			remove_shape(current_shape_array);
			generate_shape(ch);
			set_shape(current_shape_array);
			ch1 = 'x';
		}*/
		else if(ch == 0){
			//shape_O
			remove_shape(current_shape_array);
			generate_shape(0);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_O_array[i][j];
				//}
			//}
			ch1 = 'O';
		}
		else if(ch == 1){
			//shape_I
			remove_shape(current_shape_array);
			generate_shape(1);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_I_array[i][j];
				//}
			//}
			ch1 = 'I';
		}
		else if(ch == 2){
			//shape_L
			remove_shape(current_shape_array);
			generate_shape(2);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_L_array[i][j];
				//}
			//}
			ch1 = 'L';
		}
		else if(ch == 3){
			//shape_J
			remove_shape(current_shape_array);
			generate_shape(3);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_J_array[i][j];
				//}
			//}
			ch1 = 'J';
		}
		else if(ch == 4){
			//shape_S
			remove_shape(current_shape_array);
			generate_shape(4);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_S_array[i][j];
				//}
			//}
			ch1 = 'S';
		}
		else if(ch ==5){
			//shape_Z
			remove_shape(current_shape_array);
			generate_shape(5);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_Z_array[i][j];
				//}
			//}
			ch1 = 'Z';
		}
		else if(ch == 6){
			//shape_T
			remove_shape(current_shape_array);
			generate_shape(6);
			set_shape(current_shape_array);
			//for(int i = 0 ; i < 8; i++){
				//for(int j = 0; j < 8; j++){
					//current_shape_array[i][j] = shape_T_array[i][j];
				//}
			//}
			ch1 = 'T';
		}
		char score[10];	
		dtostrf(score_update , 0, 2, score);
		if(flag == 1 ){
			//Lcd4_Clear();
			char msg[] = "Score : ";
			char msg1[] = "Next piece :   ";
			msg1[13] = ch1;
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String(msg);
			Lcd4_Write_String(score);
			Lcd4_Set_Cursor(2,1);
			Lcd4_Write_String(msg1);
		}
		
		else{
			//Lcd4_Clear();
			char msg[] = "Final Score : ";
			char msg1[] = "Game Over!    ";
			Lcd4_Set_Cursor(1,1);
			Lcd4_Write_String(msg);
			Lcd4_Write_String(score);
			Lcd4_Set_Cursor(2,1);
			Lcd4_Write_String(msg1);
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

