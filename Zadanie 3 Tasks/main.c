#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "cooperative_scheduler.h"


ISR(TIMER0_COMP_vect){
	shedule();
}


int main(void)
{
	init_scheduler();
	AddTask(0,50, NULL, NULL);
	AddTask(1, 20, NULL, NULL);
	AddTask(7, 500, NULL, NULL);
	init_timer_irq();
	execute();
}