/*
 * DodawanieTablic.asm
 *
 *   Author:  Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
 */ 


.include  "m32def.inc"				; wczytuje sterowanie, nazwy portow etc. kontrolera 
.EQU LEN = 16						;dlugosc tablic
.DSEG								;segment danych, twozymy dwie tablice o zdefiniowanej dlugosci w LEN
.org 0x60							;ustalenie pierwszego adresu segmentu danych
	tab1: .BYTE LEN
	tab2: .BYTE LEN

.CSEG								;segment kodu
.org 0								;zaczynamy od adresu zero
	.DEF INDEX = R16				;definiujemy zmienna index jako R16
	.DEF TEMP = R17	
	.DEF TMP = R18
	.DEF wLEWO = R21				;ilosc przesuniec sie w lewo przy przenoszeniu Carry

	;tab1	
	LDI XL , low(tab1)				;wska�nik na pierwszy element
	LDI XH , high(tab1)				;bardziej znaczace bity
	;tab2 
	LDI YL , low(tab2)
	LDI YH , high(tab2)

	LDI INDEX , (LEN-1)				;index tablicy ustawiamy na d�ugo�� -1 i b�dziemy zmniejsza� do 0 w p�tli LOOP. 0 warunkiem stopu

	ADD XL , INDEX					;przesuwamy wska�niki X i Y na koniec tablicy
	ADD YL , INDEX
	INC INDEX						;poprawa indeksu aby wykona�a si� odpowiednia ilo�� iteracji

	LOOP:
		LD TEMP , X					;wczytujemy kolejne elementy rejestru indeksowanego X i Y
		LD TMP , Y

		ADD TMP , TEMP
		st X , TMP					;zapisujemy wartosc dodawania w odpowiedniej kom�rce rejestru X
		DEC XL						;przesuwamy wska�niki w lewo
		DEC YL
		LDI wLEWO , 0				;jezeli pojawi si� przeniesienie b�dzie potrzebne nadmiarowe przej�cie w lewo wi�c trzeba zliczy� ile tego b�dzie
		BRCS DODAJ					;wyst�pi�o przeniesienie przechodzimy do poprawienia warto�ci

	PO_DODANIU:
		DEC INDEX					;zmniejszamy ilo�� pozosta�ych element�w

	BRNE LOOP						;jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj p�tle jeszcze raz
	JMP DALEJ						;koniec element�w wi�c ko�czymy 

	DODAJ:
		CLC							;trzeba wyczy�ci� flag� C bo tu mo�e zn�w wyst�pi�
		LD R19 , X					;odczytujemy element na lewo od pola gdzie by�o przeniesienie
		LDI R20 , 1					;INC R19
		ADD R19, R20				;dodawanie zamiast INC bo INC nie zmienia flagi C
		st X , R19					;zapis warto�ci poprawionej o przeniesienie
		BRCS LEWO					;by�o przeniesienie
		BRCC POPRAW					;nie by�o przeniesienia, trzeba poprawi� ewentualne przej�cia w lewo

	LEWO:
		DEC XL						;wska�nik X w lewo
		INC wLEWO					;zliczamy przesuni�cie
		JMP DODAJ					;bezwarunkowo skaczemy do poprawy warto�ci

	POPRAW:
		ADD XL , wLEWO				;przesuwamy wska�nik X w miejsce ostatniej operacji
		JMP PO_DODANIU				;wracamy do g��wnej p�tli bo wska�niki s� poprawne
		
	DALEJ:
		NOP							;aby nie robic BREAK 
