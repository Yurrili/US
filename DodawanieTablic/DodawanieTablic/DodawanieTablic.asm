/*
 * DodawanieTablic.asm
 *
 *   Author:  Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
 */ 


.include  "m32def.inc"; wczytuje sterowanie, nazwy portow etc. kontrolera 
.EQU LEN = 16 ;dlugosc tablic
.DSEG ;segment danych, twozymy dwie tablice o zdefiniowanej dlugosci w LEN
.org 0x60 ;ustalenie pierwszego adresu segmentu danych
	tab1: .BYTE LEN
	tab2: .BYTE LEN

.CSEG ;ustalamy segment kodu
.org 0; zaczynamy od adresu zero
	.DEF INDEX = R16 ; definiujemy zmienna index jako R16
	.DEF TEMP = R17

	;tab1
	
	LDI XL , low(tab1) ;mniej znaczacy bit - wskaŸnik
	LDI XH , high(tab1) ;bardziej znaczacy bit
	;tab2 
	LDI YL , low(tab2)
	LDI YH , high(tab2)

	LDI INDEX , LEN ; index tablicy ustawiam na d³ugoœæ i bêdziemy zmniejszaæ do 0 w pêtli LOOP



	LOOP:
		LD TEMP , X
		LD R18 , Y+

		ADC R18 , TEMP
		st X+ , R18
		BRCS DODAJ


	PO_DODANIU:
		DEC INDEX
	BRNE LOOP	; jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj pêtle jeszcze raz
	JMP DALEJ

	DODAJ:
		LD R19 , X
		INC R19
		st X , R19
		BRCS DODAJ
		JMP PO_DODANIU

	DALEJ:
		NOP


	/* opcja 1.0
	

	ADD XL , INDEX	;przesuwamy wskaŸnik na ostatni¹ pozycjê tablicy
	ADD YL , INDEX

	LOOP:
	 LD TEMP , X
	 
	 DEC XL ; przesuwam wskaŸnik o jeden w lewo
	 DEC INDEX ; wartownik dla pêtli
	BRNE LOOP	; jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj pêtle jeszcze raz
	*/
	

/*	opcja 0.5
LDI INDEX , LEN
	ADD_LOOP:
	LD R20 , -X
	LD R21 , -Y

	ADC R26 , R20
	ADC X , R21

	DEC INDEX
	BRNE ADD_LOOP
	*/