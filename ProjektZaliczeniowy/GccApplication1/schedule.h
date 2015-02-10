
#ifndef clock
#define clock

#define MAX_NUMBER_OF_TASKS 8


typedef void(*TASK_PTR)(void*);

typedef struct{
		TASK_PTR task;
		int period;
		int ready;
		void* param;
		int togo;
} TASK;

//tasks

TASK tasks[MAX_NUMBER_OF_TASKS];


void schedule(void);
void execute(void);
void AddTask(int priority, int period, TASK_PTR func_ptr, void * params);

void USART_Init();
unsigned char USART_Receive(void);
void USART_Transmit(unsigned char data);
int foo(void);
char* getstr(int a, int len);

#endif 