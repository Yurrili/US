#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#define kolumna PORTD
#define wyswietlacz PORTA

static uint8_t segment[] = {0,0,0,0};	// kolejne wyświetlacze
static uint8_t kolumn = 1;				//kolumna wyswietlaczy
static uint8_t pokaz_segment = 0;		//aktualnie pokazywany segment

ISR(TIMER0_COMP_vect){
	static delay = 0;

	//shedule();
	if ((++delay % 100) == 0)	//wygaszenie wyświetlacza i zmiana wyswietlanego elementu co 1s
	{
		if( (++segment[0] %10) == 0){			//wyznaczamy kolejne wartości dla poszczególnych segmentow
			if((++segment[1] %10) == 0){
				if((++segment[2] %10) == 0){
					if((++segment[3] %10) == 0){
						segment[0] = segment[1] = segment[2] = segment[3] = 0;	//po osiągnięciu 9999 zerujemy wszystko
					}
				}
			}
		}
		wyswietlacz = 0b11111111;
		
		
	}else{
		if( (delay % 50) == 0){	//zmiana krazacego zera co 0.5s nie jesteśmy w stanie bez płytki dopasować szybkości zmian wyświetlaczy aby zmianialo sie 'plynnie'
			if( (kolumn<<1) <= 8 ){	//przesunięcia bitowe *2 wtedy mamy do dyspozycji tylko PINy 1,2,4,8 aby nie zaswiecac dwoch wyświetlaczy na raz
				kolumn<<1;
				kolumna = ~kolumn;
				pokaz_segment++;
			}
			else{
				kolumn=1;
				pokaz_segment = 0;
			}
		}
	}
}

#define SEG_ALL 0x7F
#define SEG_A 1
#define SEG_B 2
#define SEG_C 4
#define SEG_D 8
#define SEG_E 16
#define SEG_F 32
#define SEG_G 64

void init_timer_irq()
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;		//tryb CTC
	sei();										//wlacza przerwania
	TIMSK |= (1 << OCIE0);						// Output Compare Match Interrupt Enable
	TCCR0 |= (1<<CS01) | (1<< CS00);			//ustawienie preskalera na 64
}

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

/*
void display(uint8_t n) {
	switch(n) {
		case 0: PORTA = ~(SEG_ALL-SEG_G); break;
		case 1: PORTA = ~(SEG_B+SEG_C); break;
		case 2: PORTA = ~(SEG_ALL-SEG_F-SEG_C); break;
		case 3: PORTA = ~(SEG_ALL-SEG_F-SEG_E); break;
		case 4: PORTA = ~(SEG_F+SEG_G+SEG_B+SEG_C); break;
		case 5: PORTA = ~(SEG_ALL-SEG_B-SEG_E); break;
		case 6: PORTA = ~(SEG_ALL-SEG_B); break;
		case 7: PORTA =~( SEG_A+SEG_B+SEG_C); break;
		case 8: PORTA =~( SEG_ALL); break;
		case 9: PORTA = ~(SEG_ALL-SEG_E); break;
		default:PORTA = 0; break;
	}
}
*/
int main(void)
{
	DDRA = 0xFF; // ustawienie porty A na output
	DDRD = 0xFF;
	init_timer_irq();
	//execute();
	while(1)
	{
		wyswietlacz = display[ segment[pokaz_segment] ];
	}
}