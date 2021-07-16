#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <time.h>
typedef uint8_t bool;
#define FALSE 0x00
#define TRUE 0xFF
#define UP -1
#define DOWN 1
#define LEFT -1
#define RIGHT 1
typedef enum { O, I, L, J, S, Z,  T } shape_type;
	
void ADC_Init()
{
	//DDRA = 0x00;		/* Make ADC port as input */
	ADCSRA = 0b10000001;		/* Enable ADC, fr/128  */
	ADMUX =  0b01000000;		/* Vref: Avcc, ADC channel: 0 */
}

int ADC_Read(char channel)
{
	int ADC_value;
	
	ADMUX = (0x40) | (channel & 0x07);/* set input channel to read */
	ADCSRA |= (1<<ADSC);	/* start conversion */
	while((ADCSRA &(1<<ADIF))== 0);	/* monitor end of conversion interrupt flag */
	
	ADCSRA |= (1<<ADIF);	/* clear interrupt flag */
	ADC_value = (int)ADCL;	/* read lower byte */
	ADC_value = ADC_value + (int)ADCH*256;/* read higher 2 bits, Multiply with weightage */

	return ADC_value;		/* return digital value */
}

volatile char row[] = {1, 2, 4, 8, 16, 32, 64, 128};
int rand_val[100] =   {0 ,2 ,2 ,0 ,0 ,2 ,2 ,6 ,4 ,1 ,
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
	
bool play[16][8]=
{{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, TRUE,  TRUE,  FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, TRUE,  TRUE,  TRUE,  FALSE, FALSE, FALSE},
{FALSE, FALSE, TRUE,  TRUE,  TRUE,  TRUE,  FALSE, FALSE},
{FALSE, FALSE, TRUE,  TRUE,  TRUE,  TRUE,  FALSE, FALSE},
{FALSE, FALSE, TRUE,  TRUE,  TRUE,  FALSE, FALSE, FALSE},
{FALSE, FALSE, TRUE,  TRUE,  FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, TRUE,  FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE},
{FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE}};

bool temp_shape_array[4][4];
bool current_shape_array[4][4];
volatile int current_R = 0;
volatile int current_C = 2;
int current_shape = -1;
int count_speed = 175;
int count_count = 0;

void UART_init(void){
	
	int UBBRValue = 25;//AS described before setting baud rate

	//Put the upper part of the baud number here (bits 8 to 11)

	UBRRH = (unsigned char) (UBBRValue >> 8);

	//Put the remaining part of the baud number here

	UBRRL = (unsigned char) UBBRValue;

	//Enable the receiver and transmitter

	UCSRB = (1<<RXEN) |(1 << TXEN);

	//Set 2 stop bits and data bit length is 8-bit

	UCSRC = (1 << USBS) | (3 << UCSZ0);
	//UCSRC = 0b10001110;
}

void UART_send(unsigned char data){
	// wait until UDRE flag is set to logic 1
	while ((UCSRA & (1<<UDRE)) == 0x00);
	UDR = data; // Write character to UDR for transmission
	_delay_ms(4);
	
}


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
void update_score1x(){
	uint8_t temp ;
	for(int i = 0 ; i < 16 ; i++){
		temp = TRUE;
		for(int j = 0 ; j < 8; j++){
			temp &= current_display[i][j];
		}
		if(temp == TRUE){
			UART_send(10);
			PORTA |= 1 << PA2 ;
			_delay_ms(200);
			PORTA &= ~(1 << PA2);
			_delay_ms(200);
			remove_row(i);
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
			UART_send(11);
			PORTA |= 1 << PA2 ;
			_delay_ms(200);
			PORTA &= ~(1 << PA2);
			_delay_ms(200);
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
	count_speed = 175;
	count_count = 0;
	
	PORTA |= 1 << PA2 ;
	_delay_ms(200);
	PORTA &= ~(1 << PA2);
	_delay_ms(200);
}
void display(int i){
	uint8_t led_col = 0x00;
	PORTC = 0x00; // common row connection
	PORTC |= row[i]; // common row connection
	PORTB = ~get_col(i); // upper matrix column
	// lower matrix column
	led_col = ~get_col(i+8);
	PORTA |= 0xF0; // clear
	PORTD |= 0xF0; //clear
	PORTA &= ((led_col & 0x0F) << 4) | 0x0F; // col0-col3 --> A4-A7
	PORTD &= (led_col & 0xF0) | 0x0F; // col4- col7 --> D4-D7
	_delay_ms(2);
	PORTB = ~0x0; // upper matrix column
	//PORTA = ~0x0; // lower matrix column
	PORTA |= 0xF0;
	PORTD |= 0xF0;
}
void new_game(){
	int ADC_Value_Y = -1;
	for(int i = 0 ;i < 16 ; i++){
		for(int j = 0; j < 8; j++){
			current_display[i][j] = play[i][j];
		}
	}
	int i = 7;
	while(1){
		display(i);
		if(i == 7) i = 0;
		else i++;
		ADC_Value_Y = ADC_Read(1);
		if(ADC_Value_Y < 100){
			start_again();
			UART_send(7);
			return;
		}
	}
}




void new_piece(){
	if(current_R == 0 && current_C == 2 && current_shape == -1){
		generate_shape();
		//generate_shape(rand_val[r]);
		//remove_shape(current_shape_array);
		if(check_valid(0 , 2 , current_shape_array) == TRUE){
			//set_shape(current_shape_array);
			//_delay_ms(2);
		}
		else{
			UART_send(9);
			start_again();
			new_game();
			_delay_ms(200);
		}
		UART_send(current_shape);
		set_shape(current_shape_array);
	}
}

void movement(){
	int ADC_Value_X = -1, ADC_Value_Y = -1;
	ADC_Value_X = ADC_Read(0);
	ADC_Value_Y = ADC_Read(1);
	if(ADC_Value_X < 100){
		go_left();
		_delay_ms(200);
	}
	else if(ADC_Value_X > 900){
		go_right();
		_delay_ms(200);
	}
	else if(ADC_Value_Y > 900){
		go_down();
		_delay_ms(100);
	}
	else if(ADC_Value_Y < 100){
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
		_delay_ms(250);
	}
}

void mcu_init(){
	MCUCSR |= 1<<JTD;
	MCUCSR |= 1<<JTD;
	DDRA = 0b11111100;
	DDRB = 0xFF;
	DDRC = 0xFF;
	DDRD = 0xFF;
	PORTA &= 0b11111011; // for buzzer 
}
int main(void)
{
	srand(time(NULL));
	mcu_init();
	int i = 7, count = 0; //r = 0;
	ADC_Init();
	UART_init();
	while (1)
	{
		display(i);
		if(i == 7) i = 0;
		else i++;
		//_delay_us(1500);
		new_piece();
		//new_piece(r);
		//r++;
		//if(r == 100) r = 0;
		count++;
		if(count == count_speed){
			go_down();
			count_count++;
			count = 0;
			if(count_count == 50){
				count_count = 0;
				if(count_speed >=100)
					count_speed -= 50;
			}
			_delay_ms(5);
		}
		movement();
	}
}

