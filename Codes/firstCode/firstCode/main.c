/*
 * c_test_bool.c
 *
 * Created: 6/17/2021 3:32:34 PM
 * Author : USER
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>

#define F_CPU 1000000

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

uint8_t current_R = 0;
uint8_t current_C = 0;

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

int main(void)
{
    while (1)
    {
		
    }
}

