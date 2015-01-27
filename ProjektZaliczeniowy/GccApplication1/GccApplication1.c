
#include "HD44780.h"
#include "schedule.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#define USART_BAUDRATE 38400
static int dot = 0b10000000;

//represents active_screen screen at the moment
int active_screen = 0;

typedef struct{ // hour_first hour_second : minuts_first minuts_second
	int hour_first;
	int hour_second;
	int minuts_first;
	int minuts_second;
} ALARM;

//alarms
ALARM alarms_set_list[8];
uint8_t how_many_alarms=0;

//represents clock mode
int mode = 0;
// mode 0 hh:mm
// mode 1 mm:ss

//keyboard
int kolumna = 0;
int wiersz = 0;

//clock
	//seconds
int first = 0;
int second = 0;
	//minutes
int third = 0;
int fourth = 0;
	//hour
int five = 0;
int six = 0;

//change mode
int change = 0;

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


//interruption handling
ISR(TIMER0_COMP_vect)
{
	schedule();
}

void init_timer_irq()
{
	OCR0 = 250;
	TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;		//tryb CTC
	sei();										//wlacza przerwania
	TIMSK |= (1 << OCIE0);						// Output Compare Match Interrupt Enable
	TCCR0 |= (1<<CS01) | (1<< CS00);			//ustawienie preskalera na 64
}

	//DISPLAY
void mode_one(){
	DDRA = 0xff;
	DDRD = 0xff;
	PORTD = 0xFF;
	
	if(active_screen == 0)
	{
		PORTA = display[first];
		PORTD = 0b11110111;
	}
	else if(active_screen == 1)
	{
		PORTA = display[second];
		PORTD = 0b11111011;
	}
	else if(active_screen == 2)
	{
		PORTA = display[third] ; //dot because 00.00
		PORTD = 0b11111101;
	}
	else if(active_screen == 3)
	{
		PORTA = display[fourth];
		PORTD = 0b11111110;
	}
	
	active_screen = (active_screen + 1) % 4; 
}

void mode_zero(){
	DDRA = 0xff;
	DDRD = 0xff;
	PORTD = 0xFF;
	
	if(active_screen == 0)
	{
		PORTA = display[third];
		PORTD = 0b11110111;
	}
	else if(active_screen == 1)
	{
		PORTA = display[fourth] ; 
		PORTD = 0b11111011;
	}
	else if(active_screen == 2)
	{
		PORTA = display[five];//dot because 00.00
		PORTD = 0b11111101;
	}
	else if(active_screen == 3)
	{
		PORTA = display[six];
		PORTD = 0b11111110;
	}
	
	//this maes sure we cycle around 4 screens, not more
	active_screen = (active_screen + 1) % 4; 
}

void display_on(void* param)
{
	//mode mm:ss
	if(mode == 1)
	{
		mode_one();
	}
	
	// mode hh:mm
	if(mode == 0)
	{
		mode_zero();
	}
}

//increases time
void clock_(void* param)
{
	
	if(first  < 9)
	{
		first ++;
	}
	else
	{
		first = 0;
		if(second  < 6)
		{
			second ++;
		}
		else
		{
			second = 0;
			if(third < 9)
			{
				third++;
			}
			else
			{
				third = 0;
				if(fourth  < 6)
				{
					fourth++;
				}
				else
				{
					fourth = 0;	
					if(six  < 2)// earlier than 20
					{
						if(five  < 9 )
						{
							five++;
						}
						else
						{
							five = 0;
							six++;
						}
					}
					
					if(six  == 2)
					{//after 20
						
						if(five  < 4 )// 
						{
							five++;
						
						}
						else//when 24, it start from begining
						{
							six = 0;
							five = 0;
						}
						
					}
				}
			}
		}
	}
}

	//KEYBORD

void keybord_read_mode(){

	PORTC = 0x00;//wiersz
	DDRC= 0x0F;
	PORTC = 0xF0;
	
	int i=0;
	for (i;i<10;i++);
	
	wiersz= PINC;
	wiersz = (~wiersz >> 4) & 0x0f;
	
	PORTC = 0x00;//kolumna
	
	DDRC= 0xF0;
	PORTC = 0x0F;
	
	int j=0;
	for (j;j<10;j++);
	
	kolumna = PINC;
	kolumna = (~kolumna) & 0x0f;
	
}


 		// calculate
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

	//ALARM
	
	void AddAlarm(int h_first, int h_second, int m_first, int m_second)
	{
		alarms_set_list[how_many_alarms].hour_first = h_first;
		alarms_set_list[how_many_alarms].hour_second = h_second;
		alarms_set_list[how_many_alarms].minuts_first = m_first;
		alarms_set_list[how_many_alarms].minuts_second = m_second;
		
		how_many_alarms++;
	}

	void keybord_set_alarm ()
	{ // TODO show alarm setting at lcd

		LCD_WriteText("Set Alarm");
		_delay_ms(400);
		LCD_Clear();
		
		while(change!=0){// do until change last position
			
			int h_first = 0;
			int h_second = 0;
			int m_first = 0;
			int m_second = 0;
			
			//display setting time
			LCD_WriteText(h_first + h_second + ":" + m_first + m_second);
			_delay_ms(200);
			LCD_Clear();
			
			//start reading information from keyboard
			keybord_read_mode();
			
			int number = getRowNumber(wiersz) * 4 + (getColNumber(kolumna));
			
			if( number - 1 < 10 ){
				
				if(change == 1)
				{
					m_second == number - 1 ;
					change++;
					
				}
				
				if(change == 2 && number - 1 < 6)
				{//hh:*m
					m_first = number - 1;
					change++;
				}
				
				if(change == 3)
				{
					h_second == number - 1;
					change++;
				}
				
				if(change == 4 && number - 1 < 3){
					h_first = number - 1;
					change = 0;
				}
				
				AddAlarm(h_first,h_second, m_first, m_second);
			}
		}
	}

	void alarm (){
		int i =0;
		
		while(i!=800)
		{
			i++;
			LCD_WriteText("BIP BIP");
			_delay_ms(150);
			LCD_Clear();
			_delay_ms(100);
		}

	}

	void check_alarms()
	{
		int i=0;
		for (i =0;i<how_many_alarms;i++)
		{

			if (  third == alarms_set_list[how_many_alarms].minuts_second &&
			fourth == alarms_set_list[how_many_alarms].minuts_first &&
			five == alarms_set_list[how_many_alarms].hour_second &&
			six == alarms_set_list[how_many_alarms].hour_first )
			{
				alarm();
			}
			
		}
		
	}


//keybord

void keybord_change_time (){

	while(change!=0){// do until change last position
		
		keybord_read_mode();
		
		int number = getRowNumber(wiersz) * 4 + (getColNumber(kolumna));
		
		if( number - 1 < 10 ){
			
			if(change == 1)
			{
				
				if( mode = 0){//mode 0 hh:m*
					third == number - 1 ;
					} else {//mode 1 mm:s*
					first == number - 1;
				}
				change++;
			}
			
			if(change == 2)
			{//hh:*m
				if( number - 1 < 6)
				{
					if( mode = 0)
					{//mode 0 hh:*m
						fourth = number - 1;
					}
					else
					{//mode 1 mm:*s
						second = number - 1;
					}
					change++;
				}
			}
			
			if(change == 3)
			{
				if( mode = 0){//mode 0 h*:mm
					five == number - 1;
					} else {//mode 1 m*:ss
					third == number - 1;
				}
				change++;
			}
			
			if(change == 4){
				if( mode = 0){//mode 0 *h:mm
					if(number - 1 < 3){
						six = number - 1;
						change = 0;
					}
					}	else {
					if( number - 1 < 6){
						fourth = number - 1;
						change = 0;
					}
				}
			}
			
		}
	}
}
	
		// read menu
void keybord_menu (void* param)
{

	PORTC = 0x00;//wiersz
        
	DDRC= 0x0F;
	PORTC = 0xF0;
       
   int i=0;
   
   for (i;i<10;i++);
        
   wiersz = PINC;
   wiersz = (~wiersz >> 4) & 0x0f;
      
   PORTC = 0x00;//kolumna
        
   DDRC= 0xF0;
   PORTC = 0x0F;
        
   int j=0;
   for (j;j<10;j++);
         
   kolumna = PINC;
   kolumna = (~kolumna) & 0x0f;
     
         
   int number = getRowNumber(wiersz) * 4 + (getColNumber(kolumna));
         
   if( number == 16 )  //changing time, when press number "16"
   	{
		   change = 1;
		   keybord_change_time();	
	}
	
	if (number == 15)
	{
			change = 1;
			keybord_set_alarm();
	}				
			   
   	
    
  // PORTA = display[number];
}

		// changing time function <-- we need to stop intertupts here

void setHour(int first, int second){
	six = first;
	five = second;
}
void setMinute(int first, int second){
	fourth = first;
	third = second;
}

void initRS232(){
	UCSRB |= (1 << RXEN) | (1 << TXEN) ;	//transmit / receive ON
	UCSRC |= (0 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);
	
	UBRRH = (25 >> 8);
	UBRRL = 25;
	UCSRB |= (1 << RXCIE) | (1 << UDRIE);
	sei();
}

void sendCharRS(char zn){
	while(!(UCSRA & (1<<UDRE))); //wait for finish sending
	UDR = zn;	//send char
}

char getCharRS(){
	while(!(UCSRA & (1<<RXC))); //wait for finish receiving
	return UDR;	//return char
}

int main(void)
{
		LCD_Initalize();
		
		AddTask(1, 3, display_on, 0); // task to show
		AddTask(2, 100, clock_ , 0); //task to clock
		AddTask(3, 10, keybord_menu, 0); // task to type
		AddTask(4, 5, check_alarms, 0); // task to check alarm    // and task to rule them all
	
		init_timer_irq();
		sei();
		//make sure we use correct ports
		//PORTD = ~1;
		//PORTA = digit[1];
		//start task checking
		execute();
    
}

