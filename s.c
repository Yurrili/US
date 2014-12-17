/*
 * GccApplication1.c
 *
 * Created: 2014-12-17 16:05:57
 *  Author: student
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define USART_BAUDRATE 38400
char Zdanie[20]; 
int counter = 0;
int bool = 0;



int main (void)
   {
	  // char ReceivedByte;

	   UCSRB |= (1 << RXEN) | (1 << TXEN) ;  
	   UCSRC |= (0 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); 
	   
	   UBRRH = (25 >> 8); 
	   UBRRL = 25; 
	   UCSRB |= (1 << RXCIE) | (1 << UDRIE);
	   sei();
	   int i;
	   while(1)
	   {

		   /*
	   while ((UCSRA & (1 << RXC)) == 0) {}; 
	   ReceivedByte = UDR; 
	   
		 while ((UCSRA & (1 << UDRE)) == 0) {}; 
		 UDR = ReceivedByte; */
	 }    
} 

ISR(USART_UDRE_vect)
{
	if (bool ==1)
	{
	int i;
	 for (i=0;i!=counter;i++)
	 {
		 UDR = Zdanie[i]	;
		 Zdanie[i]="";
	 }
	 counter = 0;
	}	 
}

ISR(USART_RXC_vect)
{
	
	char ReceivedByte;
	ReceivedByte = UDR;
	
	if (ReceivedByte == 13)
	{
		bool =1;
	}
	else
	{		
	Zdanie[counter++] = ReceivedByte;
	bool = 0;
	//UDR = ReceivedByte;
	}	
}
