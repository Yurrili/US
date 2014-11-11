#define MAX_TASKS_NUMBER  8 // ilosc elementów
typedef struct TabEl {    //struktura
	void *ptr;      // wskaźnik do funk
	int licznik;    // po ilu przejściach timera będzie wywolana funkcja 
	int numer;      // numer struktury dla robienia czegoś w funkcji (opc)
};

struct TabEl tablica[MAX_TASKS_NUMBER];	
