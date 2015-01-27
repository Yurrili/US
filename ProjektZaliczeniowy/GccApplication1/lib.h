/*
 * Zad4_US.h
 *
 * Created: 2014-11-26 17:42:29
 *  Author: student
 */ 


#ifndef ZAD4_US_H_
#define ZAD4_US_H_

//predefined max number of tasks
#define MAX_NUMBER_OF_TASKS 8
//typedef to make it simple
typedef void(*TASK_PTR)(void*);
//structure used to hold all data about tasks, that includes: task itself, parameters,
//period in milliseconds, ready state, and time left to run task
typedef struct{
	TASK_PTR task;
	int period;
	int ready;
	void* param;
	int togo;
} TASK;
//table of tasks
TASK tasks[MAX_NUMBER_OF_TASKS];
//declarations of functions
void schedule(void);
void execute(void);
void AddTask(int priority, int period, TASK_PTR func_ptr, void * params);

#endif /* ZAD4_US_H_ */