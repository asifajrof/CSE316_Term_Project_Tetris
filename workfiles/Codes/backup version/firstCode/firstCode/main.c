#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include<time.h>

typedef uint8_t bool;
#define FALSE 0x00
#define TRUE 0xFF
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
typedef enum { O, I, L, J, S, Z,  T } shape_type;

volatile char row[] = {1, 2, 4, 8, 16, 32, 64, 128};

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
			current_display[row][j] = current_display[row-1][j];
		}
	}
	//put false in row 0
	for(int i = 0 ; i < 8; i++){
		current_display[0][i] = FALSE;
	}
}
void update_score1x(){
	uint8_t temp ;
	int update = 0;
	for(int i = 0 ; i < 16 ; i++){
		temp = TRUE;
		for(int j = 0 ; j < 8; j++){
			temp &= current_display[i][j];
		}
		if(temp == TRUE){
			update++; // need to send second atmega a signal
			remove_row(i);
		}
	}
}
void update_score2x(){
	uint8_t temp ;
	int update = 0;
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
			update = update + 8; // need to send second atmega a signal
			for(int t = 0; t < 4; t++){
				remove_row(i);
			}
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
	}
	set_shape(current_shape_array);
}
void generate_shape(){
	int shape;
	shape = rand()%7;
	if( shape == O){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_O_array[i][j];
			}
		}
	}
	else if(shape == I){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_I_array[i][j];
			}
		}
	}
	else if(shape == L){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_L_array[i][j];
			}
		}
	}
	else if(shape == J){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_J_array[i][j];
			}
		}
	}
	else if(shape == S){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_S_array[i][j];
			}
		}
	}
	else if(shape == Z){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_Z_array[i][j];
			}
		}
	}
	else if(shape == T){
		for(int i = 0 ; i < 4; i++){
			for(int j = 0; j < 4; j++){
				current_shape_array[i][j] = shape_T_array[i][j];
			}
		}
	}
}

int main(void)
{
	srand(time(0));
	DDRA = 0xFF;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0b10000111 ;
	int i = 0, count = 0;
	int flag = 0;
	while (1)
	{
		PORTC = ~row[i]; // common row connection
		PORTA = get_col(i); // upper matrix column
		PORTB = get_col(i+8); // lower matrix column
		i++;
		if(i > 7) i = 0;
		_delay_ms(3);
		if(current_R == 0 && current_C == 2){
			generate_shape();
			remove_shape(current_shape_array);
			if(check_valid(0 , 2 , current_shape_array) == TRUE)
			set_shape(current_shape_array);
		/*	else{
				PORTD |= (1<< PD2);
				_delay_ms(200);
				PORTD &= ~(1<< PD2);
				_delay_ms(200);
			} */
		}
		count++;
		if(count == 50){
			go_down();
			count = 0;
		}
		if(count == 25 && flag == 0){
			go_left();
			flag = 1;
		}
		/*if(!(PIND & (1<<PD3))){
			go_left();
		}
		if(!(PIND & (1<<PD4))){
			go_right();
		}
		if(!(PIND & (1<<PD5))){
			go_down();
		}
		if(!(PIND & (1<<PD6))){
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
		} */
		
		
		
	}
}

