//Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
#ifndef __COOPERATIVE_SCHEDULER__
#define __COOPERATIVE_SCHEDULER__

typedef void (*task_ptr)(void *);				//zdefiniowany typ dla funkcji przekazywanych do shedule

void init_scheduler();							//inicjalizacja tablicy dla zadan na -1 w ready
void init_timer_irq();							//ustawienie timera 
void shedule();									//funkcja wykonywana w przerwaniu
void AddTask(int ,int , task_ptr , void* );		//dodawanie nowych taskow do tablicy
void execute();									//glowna funkcja przegl¹dajaca i wykonujaca poszczegolne taski


#endif
