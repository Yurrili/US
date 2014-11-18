//Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak

#include "cooperative_scheduler.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define MAX_NUMBER_OF_TASKS  8

typedef struct TabEl{		 //struktura
	task_ptr func_ptr;      // wskaznik do funkcji
	int counter;			// ilosc zgloszen do wykonania
	int priorytet;
	int period;				// po ilu przejsciach timera bedzie wywolana funkcja
	int ready;				//czy gotowe do wykonania
	void * params;			//parametry dla przekazanej funkcji
} ;

struct TabEl tablica[MAX_NUMBER_OF_TASKS];

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
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;		//tryb CTC
	sei();										//wlacza przerwania
	TIMSK |= (1 << OCIE0);						// Output Compare Match Interrupt Enable 
	TCCR0 |= (1<<CS01) | (1<< CS00);			//ustawienie preskalera na 64
}


void shedule(){
	int index;
	for(index = 0; index < MAX_NUMBER_OF_TASKS ;index++)
	{
		if(tablica[index].ready != -1){							// -1 to w tablicy nie ma tutaj zadnego tasku
			if(tablica[index].counter == 0)	{					//zeby nam nie przyspieszalo liczenia przepisujemy ponownie wartosc period
				tablica[index].counter = tablica[index].period;
				} else {
					
				tablica[index].counter--;						//gdy mozna zmniejszamy ilosc wywolan (oczekiwanie)
				
				if(tablica[index].counter == 0){				//gdy mina czas nalezy zaznaczyc zadania do wykonania, nie mozna dac 1 bo zadanie moglo byc kilka razy wywolane do pracy
					tablica[index].ready++;
					//wait=0; //ma zadanie do wykonania
				}
			}
		}
		
	}
}

void AddTask(int priority,int period, task_ptr func_ptr, void *params){	//tablica[priorytet] przechowuje dane zadanie z okreslonym priorytetem
	tablica[priority].func_ptr = func_ptr;
	tablica[priority].period = period; 
	tablica[priority].counter = period;
	tablica[priority].ready = 0;
	tablica[priority].params = params;
}

void execute(){
	while(1){
		int k;
		
//			if(wait==0){//gdy w kolejce pojawia sie jakies zadanie do wykonania
			
				cli();														//blokuj przerwanie na czas przegladania zadania z taskami
			
				for(k = 0; k < MAX_NUMBER_OF_TASKS; k++){
					
						if(tablica[k].ready > 0){							//kiedy ready to wywoluje funkcje i ustawia conter znowu na okres odliczania
							tablica[k].func_ptr(tablica[k].params);			// wywolaj funkcje z parametrami
							tablica[k].ready --;							//zanotuj wywolanie
						}
					
//						if(k == MAX_NUMBER_OF_TASKS-1){
//							wait = 1;
//						}	
				}
			
				sei();
			}
//	}
}
