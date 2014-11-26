#ifndef MAX_NUMBER_OF_TASKS
#define MAX_NUMBER_OF_TASKS  8 // ilosc element?w
#endif

typedef void (*task_ptr)(void *);
static int counter1=0;

typedef struct  TabEl{    //struktura
task_ptr func_ptr;      // wska?nik do funk
int counter;        // po ilu przej?ciach timera b?dzie wywolana funkcja
int priorytet;
int period;
int ready;
void * params;
} ;
 
struct TabEl tablica[MAX_NUMBER_OF_TASKS];
 
void keypak()
{
    counter1++;
}  
void keypak1()
{
    counter1--;
}  
 
void init_scheduler()
{
    int index;
    for(index = 0; index < MAX_NUMBER_OF_TASKS ;index++)
    {
        tablica[index].ready = -1;
    }
}
void init_timer_irq()
{
    OCR0 = 250;//
    TCCR0 |= (1 << WGM01)  | (0 << WGM00) ;
    sei();  //wlacza przerwania
    TIMSK |= (1 << OCIE0); // generowanie przerwania przy 250
    TCCR0 |= (1<<CS01) | (1<< CS00);//ustawienie preskalera na 64
}  
 
void shedule(){
    int index;
    for(index = 0; index < MAX_NUMBER_OF_TASKS ;index++)
    {
        if(tablica[index].ready != -1){
            if(tablica[index].counter == 0){//zeby nam nie przyspieszalo liczenia ; )
                tablica[index].counter = tablica[index].period;
            } else {
                tablica[index].counter--;
             
                if(tablica[index].counter == 0){
                    tablica[index].ready = 1;
                }
            }
        }      
         
    }
}
 
void AddTask(int priority,int period, task_ptr func_ptr, void *params){// to chyba tak ptr * func_ptr
    tablica[priority].func_ptr = func_ptr;
    tablica[priority].period = period;
    tablica[priority].counter = period;
    tablica[priority].ready = 0;
    tablica[priority].params = params;
}
 
void execute(){
     
    while(1){
        int k;
        for(k = 0; k < MAX_NUMBER_OF_TASKS; k++){
            cli();  //blokuj przerwanie
            if(tablica[k].ready == 1){//kiedy ready to wywoluje funkcje i ustawia conter znowu na okres odliczania             
                tablica[k].func_ptr(tablica[k].params);//tutaj powinno wywo?a? funkcje, ale raczej tego nie robi ^.^
                tablica[k].ready = 0;
                sei();
                k=0;   
            }
             
    }
}