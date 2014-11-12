#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "tablica.h"


ISR(TIMER0_COMP_vect){

		shedule();		
}

int main(void)
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;
	sei();	//wlacza przerwania
	TIMSK |= (1 << OCIE0); // generowanie przerwania przy 250
	TCCR0 |= (1<<CS01) | (1<< CS00);//ustawienie preskalera na 64
	
	execute();
}
