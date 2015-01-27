/*
* lib.c
*
* Created: 2014-11-12 17:47:57
* Author: Damian Misiura, Michal Kurzynski
*/
//required for delay.h to work
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Util/delay.h>
#include "lib.h"

static int sei_on = 1; //0 - off, 1 - on; initially its on


//function executing tasks while looking through task table
void execute(void)
{
	//first assign all of remaining tasks, so we can actually pass through them
	int i;
	for(i = 0 ; i < MAX_NUMBER_OF_TASKS; i++)
	{
		if(tasks[i].task == 0)
		{
			tasks[i].param = 0;
			tasks[i].period = -1; //in case there is no task or task is non-periodic
			tasks[i].ready = 0; // 0 - not ready 1+ - ready
			tasks[i].task = 0;
			tasks[i].togo = -1;
		}
	}
	int table_counter = 0;
	//loop checking tasks
	while(1)
	{
		//if task is ready, stop handling interruption, then handle the task
		if(tasks[table_counter].ready > 0)
		{
			if(sei_on)
			{
				cli();
				sei_on = 0;
			}
			//all tasks are being processed after interruption handling is turned off
			tasks[table_counter].ready--;
			tasks[table_counter].task(tasks[table_counter].param);
			table_counter++;
		}
		//this task wasn't ready
		else
		{
			//move forward
			if(table_counter + 1 < MAX_NUMBER_OF_TASKS)
			table_counter++;
			//return to beginning of table, turn on interruption if needed
			else
			{
				if(sei_on == 0)
				{
					sei_on = 1;
					sei();
				}
				table_counter = 0;
			}
		}
	}
}
//schedule tasks, mark them as ready if needed
void schedule(void)
{
	int i;
	for(i = 0; i < MAX_NUMBER_OF_TASKS; i++)
	{
		//task ready
		if(tasks[i].togo == 0)
		{
			tasks[i].ready++;
			tasks[i].togo = tasks[i].period - 1;
		}
		//await time decrease
		else
		{
			tasks[i].togo--;
		}
	}
}
//adds new task, sets await time to period that tasks needs to wait for task to be ready
void AddTask(int priority, int period, TASK_PTR func_ptr, void * params)
{
	tasks[priority-1].period = period;
	tasks[priority-1].task = func_ptr;
	tasks[priority-1].param = params;
	tasks[priority-1].togo = period;
	tasks[priority-1].ready = 0;
}