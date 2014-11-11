#define MAX_TASKS_NUMBER  8
typedef struct TabEl {
	void *ptr;
	int licznik;
	int numer;
};

struct TabEl tablica[MAX_TASKS_NUMBER];	
