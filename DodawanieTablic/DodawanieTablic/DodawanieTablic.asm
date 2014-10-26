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
	LDI XL , low(tab1)				;wskaŸnik na pierwszy element
	LDI XH , high(tab1)				;bardziej znaczace bity
	;tab2 
	LDI YL , low(tab2)
	LDI YH , high(tab2)
	
	LDI INDEX , LEN 				;index tablicy ustawiamy na d³ugoœæ i bêdziemy zmniejszaæ do 0 w pêtli LOOP aby przesun¹æ indeksy na koniec. 0 warunkiem stopu

	MOVE_LAST:
		LD TEMP , X+				;przesuwamy w pêtli wskaŸniki na koniec tablic
		LD TEMP , Y+
		DEC INDEX
		BRNE MOVE_LAST				; index != 0 go to MOVE_LAST
					
	LDI INDEX , LEN					;ponowna inicjalizacja licznika pêtli

	LOOP:
		LD TEMP , -X				;wczytujemy kolejne elementy rejestru indeksowanego X i Y- (X zostaje a Y siê przesuwa)
		LD TMP , -Y

		ADD TMP , TEMP
		st X , TMP					;zapisujemy wartosc dodawania w odpowiedniej komórce rejestru X i przesuwamy go w prawo
		LDI wLEWO , 0				;jezeli pojawi siê przeniesienie bêdzie potrzebne nadmiarowe przejœcie w lewo wiêc trzeba zliczyæ ile tego bêdzie
		BRCS DODAJ					;wyst¹pi³o przeniesienie przechodzimy do poprawienia wartoœci

	PO_DODANIU:		
		DEC INDEX					;zmniejszamy iloœæ pozosta³ych elementów
		BRNE LOOP					;jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj pêtle jeszcze raz
		JMP KONIEC					;koniec elementów wiêc koñczymy 

	DODAJ:
		CLC							;trzeba wyczyœciæ flagê C bo tu mo¿e znów wyst¹piæ przeniesienie
		INC wLEWO					;zliczamy przesuniêcie
		LD TMP , -X					;odczytujemy element na prawo od pola gdzie by³o przeniesienie
		LDI TEMP , 1					;< == INC R19
		ADD TMP, TEMP				;dodawanie zamiast INC bo INC nie zmienia flagi C
		st X , TMP					;zapis wartoœci poprawionej o przeniesienie
		BRCS DODAJ					;by³o przeniesienie, trzeba przeskoczyæ do nastêpnej komórki
		LD TMP , X+
		BRCC POPRAW					;nie by³o przeniesienia, trzeba poprawiæ ewentualne przejœcia w lewo

	POPRAW:					
		DEC wLEWO					
		BRNE PRZESUN				;w pêtli przesuwamy wskaŸnik X w miejsce ostatniej operacji
		JMP PO_DODANIU				;wracamy do g³ównej pêtli bo wskaŸniki s¹ poprawne

	PRZESUN:
		LD TMP , X+					;cofamy X o jedn¹ komórkê w prawo
		JMP POPRAW

	KONIEC:
		NOP							;aby nie robic BREAK 
