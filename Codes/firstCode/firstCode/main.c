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

bool shape_D_array[4][4]={{FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE}};

bool shape_I_array[4][4]={{FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE}};

bool shape_L_array[4][4]={{FALSE, FALSE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE}};

bool shape_J_array[4][4]={{FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE,  TRUE, FALSE},
						  {FALSE, FALSE,  TRUE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE}};

bool shape_S_array[4][4]={{FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE},
						  { TRUE,  TRUE, FALSE, FALSE}};

bool shape_Z_array[4][4]={{FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  { TRUE,  TRUE, FALSE, FALSE},
						  {FALSE,  TRUE,  TRUE, FALSE}};

bool shape_T_array[4][4]={{FALSE, FALSE, FALSE, FALSE},
						  {FALSE, FALSE, FALSE, FALSE},
						  { TRUE,  TRUE,  TRUE, FALSE},
						  {FALSE,  TRUE, FALSE, FALSE}};


int main(void)
{
    while (1) 
    {
    }
}

