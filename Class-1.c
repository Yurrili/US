/*
 * GccApplication1.c
 *
 * Created: 2014-10-15 16:20:46
 *  Author:  Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
 */ 
 

//Stopniowe przygasanie i zapalanie diod

#define F_CPU 16000000UL // definicja szybkości taktowania procesora potrzebna funkcji dely do poprawnej pracy
#define swieci 0b11111111 //ustawienie bitów na 1 powoduje zapalenie diód
#define ciemne 0b00000000 //- = - na 0 powoduje wygaszenie diód
#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

int main(void)
{
    DDRA = 0xFF; // ustawienie portu A na output
    int i ;
    int temp = 100;
    int j = 1;
    while(1)
    {
           
        PORTA = ciemne;  // manipulacja wyjściem portu A 
        
        for (i=1;i<1000;i++) //petla zapalaj¹ca stopniowo diody
        {
            
            PORTA = swieci;
            _delay_ms(i/100); //opóŸnienie zegara   

        
            PORTA = ciemne;
            _delay_ms(10-i/100);
        }
        
        PORTA = swieci;
        
        for (i=1;i<1000;i++) // petla gaszaca stopniowo diody
        {
            
            PORTA = ciemne;
            _delay_ms(i/100);

            
            PORTA = swieci;
            _delay_ms(10-i/100);
        }
   
    }
}
