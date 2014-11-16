#ifndef MAX_NUMBER_OF_TASKS
#define MAX_NUMBER_OF_TASKS  8 // ilosc elementow
#endif
#include cooperative_scheduler.h

typedef void (*task_ptr)(void *);
static int counter1=0;
static int wait = 1;

typedef struct TabEl{    //struktura
	task_ptr func_ptr;      // wskaznik do funk
	int counter;		
	int priorytet;
	int period;	// po ilu przejsciach timera bedzie wywolana funkcja
	int ready;//czy do wykonania
	void * params;
} ;

struct TabEl tablica[MAX_NUMBER_OF_TASKS];

void keypak()
{
	counter1++;
}
void keypak1()
{
	counter1--;
}

void init_scheduler()
{
	int index;
	for(index = 0; index < MAX_NUMBER_OF_TASKS ;index++)
	{
		tablica[index].ready = -1;
	}
}

void init_timer_irq()
{
	OCR0 = 250;//
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;
	sei();	//wlacza przerwania
	TIMSK |= (1 << OCIE0); // generowanie przerwania przy 250
	TCCR0 |= (1<<CS01) | (1<< CS00);//ustawienie preskalera na 64
}


void shedule(){
	int index;
	for(index = 0; index < MAX_NUMBER_OF_TASKS ;index++)
	{
		if(tablica[index].ready != -1){// -1 <- w tablicy nie ma tutaj zadnego tasku
			if(tablica[index].counter == 0){//zeby nam nie przyspieszalo liczenia
				tablica[index].counter = tablica[index].period;
				} else {
					
				tablica[index].counter--;
				
				if(tablica[index].counter == 0){
					tablica[index].ready = 1;
					wait=0; //ma zadanie do wykonania
				}
			}
		}
		
	}
}

void AddTask(int priority,int period, task_ptr func_ptr, void *params){
	tablica[priority].func_ptr = func_ptr;
	tablica[priority].period = period; 
	tablica[priority].counter = period;
	tablica[priority].ready = 0;
	tablica[priority].params = params;
}

void execute(){
	
	while(1){
		int k;
		
			if(wait==0){//gdy w kolejce pojawia sie jakies zadanie do wykonania
			
				cli();	//blokuj przerwanie na czas przegladania zadania z taskami
			
				for(k = 0; k < MAX_NUMBER_OF_TASKS; k++){
					
						if(tablica[k].ready == 1){//kiedy ready to wywoluje funkcje i ustawia conter znowu na okres odliczania
							tablica[k].func_ptr(tablica[k].params);//tutaj powinno wywolac funkcje, ale raczej tego nie robi ^.^
							tablica[k].ready = 0;
						}
					
						if(k == MAX_NUMBER_OF_TASKS-1){
							wait = 1;
						}	
				}
			
				sei();
			}
	}
}
