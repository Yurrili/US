/*
 * GccApplication1.c
 *
 * Created: 2014-11-05 16:15:39
 *  Author: student
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define MAX_TASKS_NUMBER = 8



#include "tablica.c"


ISR(TIMER0_COMP_vect){
	int index;
	for(index = 0; index < MAX_TASKS_NUMBER ;index++)
	{
			
	}
}

void taskFunction(void *ptr){
	
}

int main(void)
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;
	sei();	//wlacza przerwania
	TIMSK |= (1 << OCIE0); // generowanie przerwania przy 250
    TCCR0 |= (1<<CS01) | (1<< CS00);//ustawienie preskalera na 64
	while(1)
    {
    }
}