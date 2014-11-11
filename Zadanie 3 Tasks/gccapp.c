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
		tablica[index].licznik--; // każde przejście zegara zmniejszamy ilość przejśc w strukturach  
	}
}

void *taskFunction(int numer) // funkcja którą wywolujemy 
{
	PORTA = numer; // zapalenie świateł 
	_delay_ms(30);
	PORTA = ciemne; 
}

int main(void)
{
	DDRA = 0xFF; // ustawienie porty A na output
	int x;
	for (x=0;x<MAX_TASKS_NUMBER;x++)
	{
		tablica[x].ptr = taskFunction(x); // wskaźnik do funkcji do strktury
		tablica[x].numer=x;   // numer struktury
		tablica[x].licznik=rand()%8; // ile przejść timera do wywolania funkcji 
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
			if (tablica[y].licznik==0) // dla każdej struktury sprawdzamy czy wywolujemy 
			{
				tablica[y].ptr; // tu powinno być wywolanie funkcji
				tablica[y].licznik=rand()%8; // nowa liczba przejść dla struktury 
			}
		}
    }
}
