/*
 * c_test_bool.c
 *
 * Created: 6/17/2021 3:32:34 PM
 * Author : USER
 */ 

#define F_CPU 1000000

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

typedef uint8_t bool;
#define FALSE 0x00
#define TRUE 0xFF

bool shape_D_array[4][4]={{FALSE,  TRUE,  TRUE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE}};

bool shape_I_array[4][4]={{FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE}};

bool shape_L_array[4][4]={{FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE}};

bool shape_J_array[4][4]={{FALSE, FALSE,  TRUE, FALSE},
						  {FALSE, FALSE,  TRUE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE}};

bool shape_S_array[4][4]={{FALSE,  TRUE,  TRUE, FALSE},
						  { TRUE,  TRUE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE}};

bool shape_Z_array[4][4]={{ TRUE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE}};

bool shape_T_array[4][4]={{ TRUE,  TRUE,  TRUE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE}};

bool current_display[16][8]={{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
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

uint8_t current_R = 0;
uint8_t current_C = 0;

void rotate_shape(bool shape_array[][4])
{
	//rotate clockwise.
	//transpose, mirror.
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			temp_shape_array[i][j] = shape_array[j][i];	//transpose
		}
	}
	
	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			shape_array[i][j] = temp_shape_array[i][j];
		}
	}

	for (int i=0; i<4; i++){
		for (int j=0; j<4; j++){
			temp_shape_array[i][j] = shape_array[i][3-j];	//mirror
		}
	}
}

void align_top_left_justify(bool shape_array[][4])
{
	//top left justify shape after rotation
}

bool check_valid(uint8_t row, uint8_t col, bool shape_array[][4])
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
			current_display[current_R+i][current_C+j] = shape_array[i][j];
		}
	}
}

uint8_t get_col(uint8_t row)
{
	uint8_t col_value = 0x00;
	for (int j=0; j<8; j++){
		if(current_display[row][j] == TRUE){
			col_value |= 1<<(7-j);
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
	uint8_t update = 0;
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
	uint8_t update = 0;
	for(int i = 0 ; i < 12 ; i++){
		temp = TRUE;
		uint8_t ii = i;
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

int main(void)
{
    while (1)
    {
		
    }
}

