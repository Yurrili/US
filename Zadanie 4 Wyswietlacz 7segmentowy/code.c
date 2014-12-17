
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//represents active screen at the moment
int active = 0;
//if dot
int dot_active = 1;

#define SEG_ALL 0x7F
#define SEG_A 1
#define SEG_B 2
#define SEG_C 4
#define SEG_D 8
#define SEG_E 16
#define SEG_F 32
#define SEG_G 64

static uint8_t display[] = {
	~( SEG_ALL - SEG_G ),
	~( SEG_B + SEG_C ),
	~( SEG_ALL - SEG_F - SEG_C ),
	~( SEG_ALL - SEG_F - SEG_E ),
	~( SEG_F + SEG_G + SEG_B + SEG_C ),
	~( SEG_ALL - SEG_B - SEG_E ),
	~( SEG_ALL - SEG_B ),
	~( SEG_A + SEG_B+SEG_C ),
	~( SEG_ALL ),
	~( SEG_ALL - SEG_E )
};
static int watch = 0;//timer
static int dot = 0b10000000;
//segments
int first = 0;
int second = 0;
int third = 0;
int fourth = 0;

ISR(TIMER0_COMP_vect){
	shedule();

	watch++;

}

void timer_count(){//counting specific seg
	if(watch < 10000){
	
			first = watch % 10;
			
			if(watch > 9 ) second = watch / 10;
			
			if(watch > 99 ) third = watch / 100;
			
			if(watch > 999 )fourth = watch / 1000;
	
	} else {
	
		if( watch < 100000){
			dot_active = 0;
			first = watch / 10;
			second = watch / 100;
			third = watch / 1000;
			fourth = watch / 10000;
		}else{
			watch = 0;
			dot_active = 1;
		}
	}
	
}



void init_timer_irq()
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;		//tryb CTC
	sei();										//wlacza przerwania
	TIMSK |= (1 << OCIE0);						// Output Compare Match Interrupt Enable
	TCCR0 |= (1<<CS01) | (1<< CS00);			//ustawienie preskalera na 64
}

void screen(void* param)
{
	//make sure we use port D
	DDRD = 0xff;
	PORTD = 0xFF;
	//every if sends a new digit to show and screen its going to use
	if(active == 0)
	{
		PORTA = display[first];
	}
	else if(active == 1 )
	{
		if( dot_active == 1){
			PORTA = display[second] + dot;
		} else {	
			PORTA = display[second];
		}
		
	}
	else if(active == 2)
	{
		PORTA = digit[third];
	}
	else if(active == 3)
	{
		PORTA = digit[fourth];
	}
	active = (active + 1) % 4;
}



int main(void)
{
	AddTask(1, 8, screen, 0)
	init_timer_irq();
	execute();

}

