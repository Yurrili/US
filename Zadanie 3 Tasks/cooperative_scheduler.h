#ifndef __COOPERATIVE_SCHEDULER__
#define __COOPERATIVE_SCHEDULER__
typedef void (*task_ptr)(void *);

void init_scheduler();
void init_timer_irq();
void shedule();
void AddTask(int ,int , task_ptr , void* );
void execute();


#endif