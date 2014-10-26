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

	LDI INDEX , (LEN-1) ; index tablicy ustawiam na d³ugoœæ i bêdziemy zmniejszaæ do 0 w pêtli LOOP

	ADD XL , INDEX
	ADD YL , INDEX
	INC INDEX

	LOOP:
		LD TEMP , X
		LD R18 , Y

		ADD R18 , TEMP
		st X , R18
		DEC XL
		DEC YL
		BRCS DODAJ

	PO_DODANIU:
		DEC INDEX
	BRNE LOOP	; jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj pêtle jeszcze raz
	JMP DALEJ

	DODAJ:
		CLC
		LD R19 , X
		INC R19
		st X , R19
		BRCS DODAJ
		JMP PO_DODANIU

	DALEJ:
		NOP
