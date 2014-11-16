#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "tablica.h"


ISR(TIMER0_COMP_vect){

	shedule();
}


int main(void)
{
		init_scheduler();
		AddTask(0,50, keypak, NULL);
		AddTask(1, 20, keypak1, NULL);
		AddTask(7, 500, NULL, NULL);
		init_timer_irq();
		execute();
}