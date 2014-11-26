#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include "tablica.h"

static int licz  = 0;

ISR(TIMER0_COMP_vect){
	static delay = 0;
	//shedule();
	if ((++delay % 100) == 0)
	{licz = ++licz % 10;
	PORTA=0b11111111;}
}
#define SEG_ALL 0x7F
#define SEG_A 1
#define SEG_B 2
#define SEG_C 4
#define SEG_D 8
#define SEG_E 16
#define SEG_F 32
#define SEG_G 64


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
int main(void)
{
	DDRA = 0xFF; // ustawienie porty A na output
	init_scheduler();
	
	init_timer_irq();
	//execute();
	while(1)
	{
		display(licz);
	}
}