#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>

#include "schedule.h"

//#define MAX_NUMBER_OF_TASKS 5

struct Task task_array[MAX_NUMBER_OF_TASKS];

ISR(TIMER0_COMP_vect) {
	schedule();
}

void init_timer() {
	OCR0 = 250;
	TIMSK |= 1 << OCIE0;
	TCCR0 |= 1 << WGM01 | 1 << CS00 | 1 << CS01;
	sei();
}

void schedule() {
	int i;
	for(i = 0; i < MAX_NUMBER_OF_TASKS; ++i) {
		if (task_array[i].ready_in != 0)
		task_array[i].ready_in--;
	}
}

void add_task(int priority, int period, foo_ptr foo, foo_params params) {
	struct Task * task = task_array + priority;
	task->foo = foo;
	task->interval = period;
	task->ready_in = period;
	task->params = params;
}

void execute() {
	init_timer();
	
	struct Task* iterator;
	
	while (1) {
		cli();
		for(iterator = task_array; iterator < task_array + MAX_NUMBER_OF_TASKS; ++iterator) {
			if (iterator->foo != NULL && iterator->ready_in == 0) {
				iterator->foo(iterator->params);
				iterator->ready_in = iterator->interval;
			}
		}
		sei();
	}
}