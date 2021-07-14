#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <time.h>
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7
#include "lcd.h"
#include <float.h>
#include<stdlib.h>
typedef uint8_t bool;
#define FALSE 0x00
#define TRUE 0xFF
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
typedef enum { O, I, L, J, S, Z,  T } shape_type;

volatile char row[] = {0b11011000, 0b11011001, 0b11011010, 0b11011011, 0b11011100, 0b11011101, 0b11011110, 0b11011111};
int rand_val[100] =   {6 ,1 ,3 ,4 ,0 ,4 ,6 ,6 ,4 ,1 ,
					   5 ,3 ,2 ,6 ,4 ,0 ,4 ,5 ,2 ,1 ,
					   5 ,4 ,4 ,4 ,6 ,0 ,3 ,0 ,6 ,3 ,
					   4 ,3 ,6 ,0 ,1 ,2 ,4 ,2 ,2 ,3 ,5 ,
					   4 ,3 ,2 ,2 ,3 ,6 ,0 ,6 ,5 ,1 ,6 ,
					   1 ,1 ,2 ,5 ,2 ,3 ,0 ,1 ,1 ,5 ,4 ,1,
					   5 ,6 ,3 ,5 ,5 ,3 ,3 ,2 ,5 ,1 ,6 ,6 ,1 ,
					   5 ,3 ,1 ,2 ,5 ,4 ,4 ,3 ,1 ,4 ,0 ,3 ,3 ,
					   4 ,0 ,2 ,0 ,0 ,3 ,2 ,2 ,1 ,1 };
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

volatile bool current_display[16][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool temp_shape_array[4][4];
bool current_shape_array[4][4];
volatile int current_R = 0;
volatile int current_C = 2;
int current_shape = -1;
int score_update = 0;

void row_shift(int length, bool shape_array[][4], int direction, int shift_count)	//no wrap around
{
	for(int counter=0; counter<shift_count; counter++){
		int index = 0;
		if (direction>0){
			index = length-1;
		}

		for (int i=0; i<length-1; i++){
			for (int j=0; j<length; j++){
				shape_array[index][j] = shape_array[index-direction][j];
			}
			index = index - direction;
		}
		for (int j=0; j<length; j++){
			shape_array[index][j] = FALSE;
		}
	}
}

void col_shift(int length, bool shape_array[][4], int direction, int shift_count)	//no wrap around
{
	for(int counter=0; counter<shift_count; counter++){
		int index = 0;
		if (direction>0){
			index = length-1;
		}

		for (int j=0; j<length-1; j++){
			for (int i=0; i<length; i++){
				shape_array[i][index] = shape_array[i][index-direction];
			}
			index = index - direction;
		}
		for (int i=0; i<length; i++){
			shape_array[i][index] = FALSE;
		}
	}
}

void align_top_left_justify(bool shape_array[][4])
{
	//top left justify shape after rotation
	//top
	int shift_count = -1;
	int shift_direction = UP;
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if(shape_array[i][j] == TRUE){
				shift_count = i;
				break;
			}
		}
		if (shift_count != -1){
			break;
		}
	}
	row_shift(4, shape_array,shift_direction,shift_count);
	
	//left justify
	int width = 0;
	int w1 = -1, w2 = -1;
	for (int j=0; j<4; j++){
		for (int i=0; i<4; i++){
			if(shape_array[i][j] == TRUE){
				if(w1 == -1){
					w1 = j;
				}
				w2 = j;
				break;
			}
		}
		if((w1 != -1) && (w2<j)){
			break;
		}
	}
	width = w2 - w1 + 1;
	if(width == 1 || width == 2){
		shift_count = w1 - 1;
	}
	else if(width == 3 || width == 4){
		shift_count = w1 - 0;
	}
	if(shift_count<0){
		shift_count = -shift_count;
		shift_direction = RIGHT;
	}
	else{
		shift_direction = LEFT;
	}
	col_shift(4,shape_array,shift_direction,shift_count);
}

void rotate_shape(bool shape_array[][4])
{
	//rotate clockwise.
	//transpose, mirror.
	bool temp[4][4];
	
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			temp[i][j] = shape_array[i][j];	//copy
		}
	}
	
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			temp_shape_array[i][j] = temp[j][i];	//transpose
		}
	}
	
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			temp[i][j] = temp_shape_array[i][j];
		}
	}

	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			temp_shape_array[i][j] = temp[i][3-j];	//mirror
		}
	}
	align_top_left_justify(temp_shape_array);
}

bool check_valid(int row, int col, bool shape_array[][4])
{
	for(int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if(shape_array[i][j] == TRUE){
				if((row+i) > 15){
					//bottom row reached
					//invalid
					return FALSE;
				}
				if((col+j) < 0 || (col+j) > 7){
					//left/right col reached
					//invalid
					return FALSE;
				}
				if(current_display[row+i][col+j] == TRUE){
					//position filled
					//invalid
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}

void set_shape(bool shape_array[][4])
{
	for(int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			if(shape_array[i][j] == TRUE){
				current_display[current_R+i][current_C+j] = shape_array[i][j];
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

void remove_row(int row){
	//shift rows downwards from row to 1
	for(int i = row ; i > 0 ; i--){
		for(int j = 0 ; j< 8; j++){
			current_display[i][j] = current_display[i-1][j];
		}
	}
	//put false in row 0
	for(int i = 0 ; i < 8; i++){
		current_display[0][i] = FALSE;
	}
}
void showScore(){
	char score[10];
	dtostrf(score_update , 0, 2, score);
	char msg[] = "Score : ";
	Lcd4_Set_Cursor(1,1);
	Lcd4_Write_String(msg);
	Lcd4_Write_String(score);
}
void update_score1x(){
	uint8_t temp ;
	for(int i = 0 ; i < 16 ; i++){
		temp = TRUE;
		for(int j = 0 ; j < 8; j++){
			temp &= current_display[i][j];
		}
		if(temp == TRUE){
			//UART_send(10);
			score_update++;
			PORTC |= (1<< PC5);
			_delay_ms(200);
			PORTC &= ~(1<< PC5);
			_delay_ms(200);
			remove_row(i);
			showScore();
		}
	}
}
void update_score2x(){
	uint8_t temp ;
	for(int i = 0 ; i < 12 ; i++){
		temp = TRUE;
		int ii = i;
		for(int t = 0; t < 4; t++){
			for(int j = 0 ; j < 8; j++){
				temp &= current_display[ii][j];
			}
			ii++;
		}
		if(temp == TRUE){
			//UART_send(11);
			score_update += 8;
			PORTC |= (1<< PC5);
			_delay_ms(200);
			PORTC &= ~(1<< PC5);
			_delay_ms(200);
			for(int t = 0; t < 4; t++){
				remove_row(i);
			}
			showScore();
		}
	}
}
void go_left(){
	remove_shape(current_shape_array);
	if(check_valid(current_R, current_C-1 , current_shape_array) == TRUE){
		current_C--;
	}
	set_shape(current_shape_array);
}
void go_right(){
	remove_shape(current_shape_array);
	if(check_valid(current_R, current_C+1 , current_shape_array) == TRUE){
		current_C++;
	}
	set_shape(current_shape_array);
}
void go_down(){
	remove_shape(current_shape_array);
	if(check_valid(current_R+1, current_C , current_shape_array) == TRUE){
		current_R++;
		set_shape(current_shape_array);
	}
	else{
		set_shape(current_shape_array);
		current_C = 2;
		current_R = 0;
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = FALSE;
			}
		}
		current_shape = -1;
		update_score2x();
		update_score1x();
	}
}
void generate_shape(){
	//int shape = 0;
	int shape = rand()%7;
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

void start_again(){
	for(int i = 0; i < 16; i++){
		for(int j = 0; j < 8; j++){
			current_display[i][j] = FALSE;
		}
	}
}
void printScreen() {
	for(int j = 0; j < 100; j++){
		for(int i = 0; i < 8; i++){
			PORTC = row[i]; // common row connection
			PORTA = get_col(i); // upper matrix column
			PORTB = get_col(i+8); // lower matrix column
		}
	}
	
}


void showNextPiece(int x){
	char msg1[] = "Next piece :   ";
	//msg1[13] = ;
	Lcd4_Set_Cursor(2,1);
	Lcd4_Write_String(msg1);
}
int main(void)
{
	MCUCSR |= 1<<JTD;
	MCUCSR |= 1<<JTD;
	srand(time(NULL));
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0b11110000 ;
	Lcd4_Init();
	int i = 0, count = 0; 
	//r = 0;
	//UART_init();
	while (1)
	{
	/*	PORTC = row[i]; // common row connection
		PORTA = get_col(i); // upper matrix column
		PORTB = get_col(i+8); // lower matrix column
		i++;
		if(i > 7) i = 0;*/
		//_delay_ms(4);
		printScreen();
		if(current_R == 0 && current_C == 2 && current_shape == -1){
			generate_shape();
			if(check_valid(0 , 2 , current_shape_array) == TRUE){
			//	UART_send(current_shape);
				showNextPiece(current_shape);
				set_shape(current_shape_array);
				printScreen();
				//_delay_ms(2);
			}
			else{
				//UART_send(9);
				printScreen();
			/*	PORTC |= (1<< PC5);
				_delay_ms(200);
				PORTC &= ~(1<< PC5);
				_delay_ms(200);
				start_again();*/
				_delay_ms(1000);
			}
		}
		count++;
		if(count == 25){
			go_down();
			count = 0;
			printScreen();
		}
	/*	if(!(PIND & (1<<PD0))){
			go_left();
			_delay_ms(200);
			printScreen();
		}
		if(!(PIND & (1<<PD1))){
			go_right();
			_delay_ms(200);
			printScreen();
		}
		if(!(PIND & (1<<PD2))){
			go_down();
			_delay_ms(200);
			printScreen();
		}
		if(!(PIND & (1<<PD3))){
			rotate_shape(current_shape_array);
			remove_shape(current_shape_array);
			if(check_valid(current_R, current_C, temp_shape_array) == TRUE){
				for(int i = 0 ; i < 4; i++){
					for(int j = 0; j < 4; j++){
						current_shape_array[i][j] = temp_shape_array[i][j];
					}
				}
			}
			set_shape(current_shape_array);
			printScreen();
		}*/
		showScore();
	}
}

