#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "cooperative_scheduler.h"


static uint8_t segment[] = {0,0,0,0}; // kolejne wyœwietlacze
static uint8_t kolumn = 1; //kolumna wyswietlaczy
static uint8_t pokaz_segment = 0; //aktualnie pokazywany segment
uint8_t wiersz;
uint8_t kolumna;
uint8_t number =0;
uint8_t first =0;
uint8_t second =0;
uint8_t active = 0;

ISR(TIMER0_COMP_vect){
	schedule();
}


void init_timer_irq()
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;		//tryb CTC
	sei();										//wlacza przerwania
	TIMSK |= (1 << OCIE0);						// Output Compare Match Interrupt Enable
	TCCR0 |= (1<<CS01) | (1<< CS00);			//ustawienie preskalera na 64
	//DDRA |= 0xFF;
	//DDRD |= 0xFF;
}


#define SEG_ALL 0x7F
#define SEG_A 1
#define SEG_B 2
#define SEG_C 4
#define SEG_D 8
#define SEG_E 16
#define SEG_F 32
#define SEG_G 64

static uint8_t display[] = {
	~( SEG_ALL - SEG_G ),
	~( SEG_B + SEG_C ),
	~( SEG_ALL - SEG_F - SEG_C ),
	~( SEG_ALL - SEG_F - SEG_E ),
	~( SEG_F + SEG_G + SEG_B + SEG_C ),
	~( SEG_ALL - SEG_B - SEG_E ),
	~( SEG_ALL - SEG_B ),
	~( SEG_A + SEG_B+SEG_C ),
	~( SEG_ALL ),
	~( SEG_ALL - SEG_E ),
	   SEG_ALL
};


uint8_t getRowNumber(uint8_t val)
{
	uint8_t tmp;
	
	for (tmp = 0; tmp < 8; tmp++)
	{
		if (val & 1)
			
			{
				return tmp;
			}					
		val = val >> 1;
	}
		

	return 0;
}

uint8_t getColNumber(uint8_t val)
{
	uint8_t tmp;
	
	for (tmp = 0; tmp < 8; tmp++)
	{
		if (val & 1)
		{
			return tmp+1;

		}
		val = val >> 1;
	}
		

	return 0;
}

void keyboard(void* param)
{
	PORTC = 0x00;//wiersz
	DDRC= 0x0F;
	PORTC = 0xF0;
	int i=0;
	for (i;i<10;i++);
	wiersz = (~PINC >> 4) & 0x0f;

	
	PORTC = 0x00;//kolumna
	DDRC= 0xF0;
	PORTC = 0x0F;
	int j=0;
	for (j;j<10;j++);
	kolumna = (~PINC) & 0x0f;
	
	number = getRowNumber(wiersz) * 4 + (getColNumber(kolumna));
	first = number % 10;
	second = number / 10;
}

void screen(void* param)
{
	DDRD = 0xff;
	PORTD = 0xFF;
	if(active == 0)
	{
		if(first >= 0 && first < 10)
			PORTA = display[first];
		else
			PORTA = display[10];
		PORTD = 0b11110111;
	}
	else if(active == 1)
	{
		
		if(second >= 0 && second < 10)
			PORTA = display[second];
		else
			PORTA = display[10];
		PORTD = 0b11111011;
	}
	active = (active + 1) % 2;
}

int main(void)
{
	AddTask(1, 8, screen, 0);
	AddTask(2, 50, keyboard, 0);
	init_timer_irq();
     PORTD = 0xFF;
    execute();
}

