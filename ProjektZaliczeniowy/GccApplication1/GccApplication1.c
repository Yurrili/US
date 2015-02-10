//kurkadfdsdfsdf
#include "HD44780.h"
#include "schedule.h"
#include <avr/io.h>
#include <string.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#define USART_BAUDRATE 38400


static int dot = 0b10000000;
int watch_mode = 0;
int watch_mode_q = 0;

//represents active_screen screen at the moment
int active_screen = 0;
int display_mode = 0;//  0 - mode of setting alarm or  1 - clock
int display_mode_q = 0;
int access = 0; // access to changing time or alarm
// hour_first hour_second : minuts_first minuts_second
typedef struct{ 
int hour_first;
int hour_second;
int minuts_first;
int minuts_second;
} ALARM;

//alarms
ALARM alarms_set_list[8];
uint8_t how_many_alarms=0;

int hour_first = 1;
int hour_second = 2;
int minuts_first = 0;
int minuts_second = 0;

int runtime = 1;
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
int five = 2;
int six = 1;


#define SEG_ALL 0x7F
#define SEG_A 1
#define SEG_B 2
#define SEG_C 4
#define SEG_D 8
#define SEG_E 16
#define SEG_F 32
#define SEG_G 64

//7-segment display
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
	if(display_mode == 0)
		display_on();
	else
		display_alarm_setting();
		
	schedule();
}

void display_alarm_setting(){
	
	DDRA = 0xff;
	DDRD = 0xff;
	PORTD = 0xFF;
	
	if(active_screen == 0)
	{
		PORTA = display[minuts_second];
		PORTD = 0b11110111;
	}
	else if(active_screen == 1)
	{
		PORTA = display[minuts_first];
		PORTD = 0b11111011;
	}
	else if(active_screen == 2)
	{
		PORTA = display[hour_second] ; //dot because 00.00
		PORTD = 0b11111101;
	}
	else if(active_screen == 3)
	{
		PORTA = display[hour_first];
		PORTD = 0b11111110;
	}
	
	active_screen = (active_screen + 1) % 4; 
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
void mode_one(){//mm:ss
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
//hh:mm
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
if(runtime == 1)
	{
	if(how_many_alarms > 0){
		check_alarms();
	}
	
	if(first  < 9)
	{
		first ++;
	}
	else
	{
		first = 0;
		if(second  < 5)
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
				if(fourth  < 5)
				{
					fourth++;
				}
				else
				{
					fourth = 0;	
						if((six*10 + five) <= 18  ) 
							five++;
							
						if((six*10 + five) == 19  ){
							six = 2;
							five = 0;
						} 
						
						if((six*10 + five) >= 20 && (six*10 + five) < 23)
							five++;
						 
						if((six*10 + five) == 23  ){
							six = 0;
							five = 0;
						}
				}
			}
		}
	}
	}
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
	}

void set_time()
{ 
			if(mode == 1){
				mode = 0;
			}
			
				switch(watch_mode_q)
				{
					case 0:  // _ _ : _ 0
							if(third <= 8  ) 
								third++;
							else
								third=0;
						break;
					case 1: //_ _ : 0 _
							if(fourth <= 4  ) 
								fourth++;
							else
								fourth=0;
						break;
					case 2: // 0 0 : _ _
							if((six*10 + five) <= 18  ) 
								five++;
								
							if((six*10 + five) == 19  ){
								six = 2;
								five = 0;
							} 
							
							if((six*10 + five) >= 20 && (six*10 + five) <= 23)
								five++;
							 
							
							if((six*10 + five) == 24  ){
								six = 0;
								five = 0;
							}
						break;
						
				
	
				}
}
		
void set_alarm()
{ 
				switch(display_mode_q)
				{
					case 0:  // _ _ : _ 0
					
							if(minuts_second <= 8  ) 
								minuts_second++;
							else
								minuts_second=0;
						break;
					case 1: //_ _ : 0 _
							if(minuts_first <= 4  ) 
								minuts_first++;
							else
								minuts_first=0;
						break;
					case 2: // 0 0 : _ _
							if((hour_first*10 + hour_second) <= 18  ) 
								hour_second++;
								
							if((hour_first*10 + hour_second) == 19  ){
								hour_first = 2;
								hour_second = 0;
							} 
							
							if(((hour_first*10 + hour_second) >= 20) && ((hour_first*10 + hour_second) <= 23))
								hour_second++;
							 
							
							if((hour_first*10 + hour_second) == 24  ){
								hour_first = 0;
								hour_second = 0;
							}
						break;
	
				}
}

void alarm_RING(){
		int i = 0;
		int j = 0;
		
		while(i!=800)
		{
			i++;
			LCD_WriteText("BIP BIP");
			for (j;j<200;j++);	
			LCD_Clear();		
		}
}

void check_alarms()
{
	LCD_Clear();
	LCD_WriteText("ALLARMS");
		int i = 0 ;
		int number_first = 0 ;
		int number_second = 0 ;
		int number_clock_first = six*10 + five;
		int number_clock_second = fourth*10 + third;
		
		for (i = 0; i < how_many_alarms; i++)
		{
			LCD_Clear();
			LCD_WriteText("AA");
			number_first = (alarms_set_list[how_many_alarms].hour_first)*10 + alarms_set_list[how_many_alarms].hour_second;
			number_second = (alarms_set_list[how_many_alarms].minuts_first)*10 + alarms_set_list[how_many_alarms].minuts_second;
		
			if ( number_clock_first == number_first)
			{
				if (number_clock_second == number_second )
				{
								//LCD_WriteText("BBBBB");
								alarm_RING();	
				}
			}
			
		}		
}

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
           
   switch(number)
	{
		case 1: //changing mode
				if(watch_mode != 1)
			   	mode = (mode+1) % 2;
			   else
			   	mode = 1;
			break;
			
		case 2://set time / set changing time mode
			switch(watch_mode)
			{
				case 1:
					
						if(watch_mode_q == 2){
							watch_mode = 0;
							watch_mode_q = 0;
							access = 0;
							LCD_Clear();
							LCD_WriteText("Time Setted");
						}							
						if(watch_mode_q < 2)
							watch_mode_q = (watch_mode_q+1) % 3;
							
					
							
							if(how_many_alarms > 0){
									check_alarms();
								}
						
					break;			
				
				case 0:
						watch_mode = 1;
						access = 1;
						watch_mode_q = 0;
						LCD_WriteText("Set TIME");
					break;	
			}
				break;
				
		case 3:// ++ time
		if (access==1)
		{
				if(display_mode == 1)
					set_alarm();
				if (display_mode == 0)
					set_time();	
		}
			
			break;
			
		case 4:// set alarm
				switch(display_mode)
				{
					case 1:
						
							if(display_mode_q == 2){
								display_mode = 0;
								display_mode_q = 0;
								
								AddAlarm(hour_first, hour_second, minuts_first, minuts_second);
								how_many_alarms = (how_many_alarms+1) % 9;
								LCD_Clear();
								LCD_WriteText(" Alarm ADDED ");
								access =0;
								hour_first = 1;
								hour_second = 2;
								minuts_first = 0;
								minuts_second = 0;
							}								
							if(display_mode_q < 2)
								display_mode_q = (display_mode_q+1) % 3;
								
						
								
								
					
						break;			
					
					case 0:
							display_mode = 1;
							display_mode_q = 0;
							access = 1;
							LCD_WriteText("Set Alarm");
						break;	
				}
			break;
			
			case 5: //usart
				
						cli();
						runtime = 0;
						second = usarthandle();
						sei();
						runtime = 1;
						break;
				
		
	}	//switch end
}




void usarthandle() //GG:MM:ss -> OK
{
	
	unsigned char dane[3];
	//godziny
	PORTA=0x00;
	dane[0] = USART_Receive();
	PORTA = 0xFF;
	USART_Transmit(dane[0]);
	PORTA = 0x00;
	dane[1] = USART_Receive();
	PORTA = 0xFF;
	USART_Transmit(dane[1]);
	
	int hour = atoi(dane);	//GG
	if(hour < 0 || hour > 23)
		hour = 0;
	else{
		six = hour/10;	//lub dane[0]
		five = hour%10;// dane [1] :)
		}
		
	PORTA = 0xFF;
	USART_Transmit('\;');	//hour set confirm via "\;"
	
	PORTA=0x00;
	dane[0] = USART_Receive();	// RECIVE : CHAR
	
	PORTA = 0xFF;
	USART_Transmit('\;');	//confirm recive :
	
	
	//minuty
	PORTA=0x00;
	dane[0] = USART_Receive();
	PORTA = 0xFF;
	USART_Transmit(dane[0]);
	PORTA = 0x00;
	dane[1] = USART_Receive();
	PORTA = 0xFF;
	USART_Transmit(dane[1]);
	int minute = atoi(dane);	// MM
	
	if(minute < 0 || minute > 59)
		minute = 0;
	else{
		fourth = minute/10;	// dane[0]
		 third = minute%10;// dane[1]
		}
	
	PORTA = 0xFF;
	USART_Transmit('\;');
	
	PORTA=0x00;
	dane[0] = USART_Receive(); // RECIVE ':' CHAR
	
	PORTA = 0xFF;
	USART_Transmit('\;');	//confirm recive :
	
	
	//sekundy
	PORTA=0x00;
	dane[0] = USART_Receive();
	PORTA = 0xFF;
	USART_Transmit(dane[0]);
	PORTA = 0x00;
	dane[1] = USART_Receive();
	PORTA = 0xFF;
	USART_Transmit(dane[1]);
	int secondd = atoi(dane);
	
	if(secondd < 0 || secondd > 59)
		secondd = 0;	
	else{
		second = secondd/10;	//dane[0]
		first = secondd%10;		//dane[1]
	}
	
		PORTA = 0xFF;
		USART_Transmit('OK'); // finish USART time setup
}


int main(void)
{
		LCD_Initalize();
		LCD_Clear();
		USART_Init();	// speed 38400 , 1bit stop, no parity
		AddTask(1, 1000, clock_ , 0); //task to clock
		AddTask(2, 100, keybord_menu, 0); // task to type
	//AddTask(4, , check_alarms, 0); // task to check alarm    // and task to rule them all
	
		init_timer_irq();
		sei();
		execute();
    
}

