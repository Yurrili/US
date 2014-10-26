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
	LDI XL , low(tab1)				;wskaŸnik na pierwszy element
	LDI XH , high(tab1)				;bardziej znaczace bity
	;tab2 
	LDI YL , low(tab2)
	LDI YH , high(tab2)

	LDI INDEX , (LEN-1)				;index tablicy ustawiamy na d³ugoœæ -1 i bêdziemy zmniejszaæ do 0 w pêtli LOOP. 0 warunkiem stopu

	ADD XL , INDEX					;przesuwamy wskaŸniki X i Y na koniec tablicy
	ADD YL , INDEX
	INC INDEX						;poprawa indeksu aby wykona³a siê odpowiednia iloœæ iteracji

	LOOP:
		LD TEMP , X					;wczytujemy kolejne elementy rejestru indeksowanego X i Y
		LD TMP , Y

		ADD TMP , TEMP
		st X , TMP					;zapisujemy wartosc dodawania w odpowiedniej komórce rejestru X
		DEC XL						;przesuwamy wskaŸniki w lewo
		DEC YL
		LDI wLEWO , 0				;jezeli pojawi siê przeniesienie bêdzie potrzebne nadmiarowe przejœcie w lewo wiêc trzeba zliczyæ ile tego bêdzie
		BRCS DODAJ					;wyst¹pi³o przeniesienie przechodzimy do poprawienia wartoœci

	PO_DODANIU:
		DEC INDEX					;zmniejszamy iloœæ pozosta³ych elementów

	BRNE LOOP						;jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj pêtle jeszcze raz
	JMP DALEJ						;koniec elementów wiêc koñczymy 

	DODAJ:
		CLC							;trzeba wyczyœciæ flagê C bo tu mo¿e znów wyst¹piæ
		LD R19 , X					;odczytujemy element na lewo od pola gdzie by³o przeniesienie
		LDI R20 , 1					;INC R19
		ADD R19, R20				;dodawanie zamiast INC bo INC nie zmienia flagi C
		st X , R19					;zapis wartoœci poprawionej o przeniesienie
		BRCS LEWO					;by³o przeniesienie
		BRCC POPRAW					;nie by³o przeniesienia, trzeba poprawiæ ewentualne przejœcia w lewo

	LEWO:
		DEC XL						;wskaŸnik X w lewo
		INC wLEWO					;zliczamy przesuniêcie
		JMP DODAJ					;bezwarunkowo skaczemy do poprawy wartoœci

	POPRAW:
		ADD XL , wLEWO				;przesuwamy wskaŸnik X w miejsce ostatniej operacji
		JMP PO_DODANIU				;wracamy do g³ównej pêtli bo wskaŸniki s¹ poprawne
		
	DALEJ:
		NOP							;aby nie robic BREAK 
