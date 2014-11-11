tablica.c
#define MAX_TASKS_NUMBER  8
typedef struct TabEl {
	void *ptr;
	int licznik;
	int numer;
};

struct TabEl tablica[MAX_TASKS_NUMBER];	


gccapp.c
/*
 * GccApplication1.c
 *
 * Created: 2014-11-05 16:15:39
 *  Author: student
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#define ciemne 0b00000000 //- = - wygaszenie diód
#include "tablica.c"
#include <util/delay.h>



ISR(TIMER0_COMP_vect){
	int index;
	for(index = 0; index < MAX_TASKS_NUMBER ;index++)
	{
		tablica[index].licznik--; 
	}
}

void *taskFunction(int numer)
{
	PORTA = numer;
	_delay_ms(30);
	PORTA = ciemne;
}

int main(void)
{
	DDRA = 0xFF; // ustawienie porty A na output
	int x;
	for (x=0;x<MAX_TASKS_NUMBER;x++)
	{
		tablica[x].ptr = taskFunction(x);
		tablica[x].numer=x;
		tablica[x].licznik=rand()%8;
	}
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;
	sei();	//wlacza przerwania
	TIMSK |= (1 << OCIE0); // generowanie przerwania przy 250
    TCCR0 |= (1<<CS01) | (1<< CS00);//ustawienie preskalera na 64
	while(1)
    {		
		int y;
		for (y=0;y<MAX_TASKS_NUMBER;y++)
		{
			if (tablica[y].licznik==0)
			{
				tablica[y].ptr;
				tablica[y].licznik=rand()%8;
			}
		}
    }
}
