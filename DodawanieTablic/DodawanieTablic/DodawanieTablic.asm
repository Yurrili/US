/*
 * DodawanieTablic.asm
 *
 *   Author:  Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
 *	 Program czyta mlodsze bity tablic od Prawej do Lewej
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
	.DEF wLEWO = R21				;ilosc przesuniec sie w prawo przy przenoszeniu Carry

	;tab1	
	LDI XL , low(tab1)				;wska�nik na pierwszy element
	LDI XH , high(tab1)				;bardziej znaczace bity
	;tab2 
	LDI YL , low(tab2)
	LDI YH , high(tab2)
	
	LDI INDEX , LEN 				;index tablicy ustawiamy na d�ugo�� i b�dziemy zmniejsza� do 0 w p�tli LOOP aby przesun�� indeksy na koniec. 0 warunkiem stopu

	MOVE_LAST:
		LD TEMP , X+				;przesuwamy w p�tli wska�niki na koniec tablic
		LD TEMP , Y+
		DEC INDEX
		BRNE MOVE_LAST				; index != 0 go to MOVE_LAST
					
	LDI INDEX , LEN					;ponowna inicjalizacja licznika p�tli

	LOOP:
		LD TEMP , -X				;wczytujemy kolejne elementy rejestru indeksowanego X i Y- (X zostaje a Y si� przesuwa)
		LD TMP , -Y

		ADD TMP , TEMP
		st X , TMP					;zapisujemy wartosc dodawania w odpowiedniej kom�rce rejestru X i przesuwamy go w prawo
		LDI wLEWO , 0				;jezeli pojawi si� przeniesienie b�dzie potrzebne nadmiarowe przej�cie w lewo wi�c trzeba zliczy� ile tego b�dzie
		BRCS DODAJ					;wyst�pi�o przeniesienie przechodzimy do poprawienia warto�ci

	PO_DODANIU:		
		DEC INDEX					;zmniejszamy ilo�� pozosta�ych element�w
		BRNE LOOP					;jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj p�tle jeszcze raz
		JMP KONIEC					;koniec element�w wi�c ko�czymy 

	DODAJ:
		CLC							;trzeba wyczy�ci� flag� C bo tu mo�e zn�w wyst�pi� przeniesienie
		INC wLEWO					;zliczamy przesuni�cie
		LD TMP , -X					;odczytujemy element na prawo od pola gdzie by�o przeniesienie
		LDI TEMP , 1					;< == INC R19
		ADD TMP, TEMP				;dodawanie zamiast INC bo INC nie zmienia flagi C
		st X , TMP					;zapis warto�ci poprawionej o przeniesienie
		BRCS DODAJ					;by�o przeniesienie, trzeba przeskoczy� do nast�pnej kom�rki
		LD TMP , X+
		BRCC POPRAW					;nie by�o przeniesienia, trzeba poprawi� ewentualne przej�cia w lewo

	POPRAW:					
		DEC wLEWO					
		BRNE PRZESUN				;w p�tli przesuwamy wska�nik X w miejsce ostatniej operacji
		JMP PO_DODANIU				;wracamy do g��wnej p�tli bo wska�niki s� poprawne

	PRZESUN:
		LD TMP , X+					;cofamy X o jedn� kom�rk� w prawo
		JMP POPRAW

	KONIEC:
		NOP							;aby nie robic BREAK 
