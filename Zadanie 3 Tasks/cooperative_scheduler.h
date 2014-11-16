#ifndef cooperative_scheduler
#define cooperative_scheduler
void init_scheduler();
void init_timer_irq();
void shedule();
void AddTask(int ,int , task_ptr , void* );
void execute();


#endif