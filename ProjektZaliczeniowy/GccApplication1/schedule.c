#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "schedule.h"

static int sei_on = 1; 

void execute(void)
{
	
	int i;
		
		for(i = 0 ; i < MAX_NUMBER_OF_TASKS; i++)
		{
					if(tasks[i].task == 0)
					{
						tasks[i].param = 0;
						tasks[i].period = -1; 
						tasks[i].ready = 0; 
						tasks[i].task = 0;
						tasks[i].togo = -1;
					}
	}
	
	int table_counter = 0;
	
			while(1)
			{
				
				if(tasks[table_counter].ready > 0)
				{
					if(sei_on)
					{
						//cli();
						sei_on = 0;
					}
					
							tasks[table_counter].ready--;
							
								tasks[table_counter].task(tasks[table_counter].param);
							table_counter++;
				}
		
		else
		{
		
			if(table_counter + 1 < MAX_NUMBER_OF_TASKS)
				table_counter++;
			
			else
			{
				if(sei_on == 0)
				{
					sei_on = 1;
					//sei();
				}
				table_counter = 0;
			}
		}
	}
}

void schedule(void)
{
		int i;
		for(i = 0; i < MAX_NUMBER_OF_TASKS; i++)
		{
			//task ready
				if(tasks[i].togo == 0)
				{
					tasks[i].ready++;
					if(tasks[i].ready >= 1000)
						tasks[i].ready = 2;
					tasks[i].togo = tasks[i].period - 1;
				}else{
					tasks[i].togo--;
				}
		}
}

void AddTask(int priority, int period, TASK_PTR func_ptr, void * params)
{
		tasks[priority-1].period = period;
		tasks[priority-1].task = func_ptr;
		tasks[priority-1].param = params;
		tasks[priority-1].togo = period;
		tasks[priority-1].ready = 0;
}

void USART_Init()
{
		UBRRH = (unsigned char)(25>>8);
		UBRRL = (unsigned char)25;
		
		UCSRB = (1<<RXEN)|(1<<TXEN);	//receivetransmit enable
		UCSRC = (1<<URSEL)|(0<<USBS)|(0<<UCSZ2)|(1<<UCSZ1)|(1<<UCSZ0); // 8-bit frame, 1-bit-stop, 
}


unsigned char USART_Receive(void)
{
	while(!(UCSRA & (1<<RXC)) );
	return UDR;
}

void USART_Transmit(unsigned char data)
{
	while( !(UCSRA & (1<<UDRE)) );
	UDR=data;
}

int foo(void)
{
	DDRA|=0xFF;
	USART_Init(25);
	unsigned char date;
	
		while(1)
		{
			PORTA=0x00;
			date=USART_Receive();
			PORTA=0xff;
			USART_Transmit(date);
		}
}