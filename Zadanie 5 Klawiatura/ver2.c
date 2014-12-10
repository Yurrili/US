#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>


void init_timer_irq()
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;		//tryb CTC
	sei();										//wlacza przerwania
	TIMSK |= (1 << OCIE0);						// Output Compare Match Interrupt Enable
	TCCR0 |= (1<<CS01) | (1<< CS00);			//ustawienie preskalera na 64
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
	~( SEG_ALL - SEG_E )
};


static uint8_t display1[] = {
	0b10000001,//1
	0b01000001,//2
	0b00100001,//3
	0b00010001,//4
	0b10000010,//5
	0b01000010,//6
	0b00100010,//7
	0b00010010,//8
	0b10000100,//9
	0b01000100,//10
	0b00100100,//11
	0b00010100,//12
	0b10001000,//13
	0b01001000,//14
	0b00101000,//15
	0b00011000//16
};



uint8_t getRowNumber(uint8_t val)
{
	uint8_t tmp;
	
	for (tmp = 0; tmp < 8; tmp++)
	{
		if (val & 1)
			break;
		val = val >> 1;
	}	
if (tmp==4)
{
return 0;
}
else 
{
if (tmp ==5)
{
return 1;
}
else
{
if (tmp == 6)
{
return 2;
}
else
{
if (tmp ==8)
{
retunr 3;
}
}
}
}
	return 0;
}

uint8_t getColNumber(uint8_t val)
{
	uint8_t tmp;
	
	for (tmp = 0; tmp < 8; tmp++)
	{
		if (val & 1)
			break;
		val = val >> 1;
	}	
if (tmp==4)
{
return 1;
}
else 
{
if (tmp ==5)
{
return 2;
}
else
{
if (tmp == 6)
{
return 3;
}
else
{
if (tmp ==8)
{
retunr 4;
}
}
}
}
	return 0;
}

int main(void)
{
	init_timer_irq();
	uint8_t wiersz;
	uint8_t kolumna;
	DDRA = 0xFF;
	/*key1= PIN;
	PORTA = 0x00;
	DDRA = 0xF0;
	
	PORTA = 0x0F;*/
    while(1)
    {
        PORTC = 0x00;//wiersz
        DDRC= 0x0F;
        PORTC = 0xF0;
        int i=0;
        for (i;i<10;i++);
		wiersz= PINC;
		//0b11010000
		// key1 = (~key1) >> 4;
		//0b00000010;
		//PORTA = key1;
		
		PORTC = 0x00;//kolumna
		DDRC= 0xF0;
		PORTC = 0x0F;
		int j=0;
		for (j;j<10;j++);
		
		kolumna= PINC;
		//key2 = (~key2) & 0x0f;
		// key2 = 0b00000100
		PORTA =~(kolumna+wiersz);
		
		uint8_t number = getRowNumber(wiersz) * 4 + getColNumber(kolumna);
		
		
		PORTA = ~wiersz;
    }
}

