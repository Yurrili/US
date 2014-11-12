#define MAX_NUMBER_OF_TASKS  8 // ilosc elementów

typedef struct TabEl {    //struktura
	void *func_ptr;      // wskaŸnik do funk
	int counter;		// po ilu przejœciach timera bêdzie wywolana funkcja
	int priorytet; 
	int period;
	int ready;     
};

struct TabEl tablica[MAX_NUMBER_OF_TASKS];

void shedule(){
	int index;
	for(index = 0; index < MAX_NUMBER_OF_TASKS ;index++)
	{
		if(tablica[index].counter == 0){//zeby nam nie przyspieszalo liczenia ; )
			tablica[index].counter = tablica[index].period;
		} else {
			tablica[index].counter--;
			
			if(tablica[index].counter == 0){
				tablica[index].ready = 1;
			}
		}
		
		
		//tu zostalo napisac funkcje sprawdzajac¹
	}
}

void AddTask(int priority,int period, void *func_ptr, void *params){// to chyba tak ptr * func_ptr
	tablica[priority].func_ptr = func_ptr(params);// Z TYM MAM PROBLEM
	tablica[priority].period = period;
	tablica[priority].counter = period;
	tablica[priority].ready = 0;
	
}

void execute(){
	
	while(1){
		int k;
		for(k = 0; k < MAX_NUMBER_OF_TASKS; k++){
			if(tablica[k].ready == 1){//kiedy ready to wywoluje funkcje i ustawia conter znowu na okres odliczania
				tablica[k].func_ptr;//tutaj powinno wywo³aæ funkcje, ale raczej tego nie robi ^.^
				tablica[k].ready = 0;
				k = 0;	
			}
		}
	}
}


