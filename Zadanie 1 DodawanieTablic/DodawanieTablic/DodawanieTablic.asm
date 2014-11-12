/*
 * DodawanieTablic.asm
 *
 *   Author:  Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
 *	 Program czyta mlodsze bity tablic od lewej do prawej
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
	.DEF wPRAWO = R21				;ilosc przesuniec sie w prawo przy przenoszeniu Carry

	;tab1	
	LDI XL , low(tab1)				;wskaŸnik na pierwszy element
	LDI XH , high(tab1)				;bardziej znaczace bity
	;tab2 
	LDI YL , low(tab2)
	LDI YH , high(tab2)
	
	LDI INDEX , LEN 				;index tablicy ustawiamy na d³ugoœæ i bêdziemy zmniejszaæ do 0 w pêtli LOOP. 0 warunkiem stopu

	LOOP:
		LD TEMP , X					;wczytujemy kolejne elementy rejestru indeksowanego X i Y+ (X zostaje a Y siê przesuwa)
		LD TMP , Y+

		ADD TMP , TEMP
		st X+ , TMP					;zapisujemy wartosc dodawania w odpowiedniej komórce rejestru X i przesuwamy go w prawo
		LDI wPRAWO , 0				;jezeli pojawi siê przeniesienie bêdzie potrzebne nadmiarowe przejœcie w prawo wiêc trzeba zliczyæ ile tego bêdzie
		BRCS DODAJ					;wyst¹pi³o przeniesienie przechodzimy do poprawienia wartoœci

	PO_DODANIU:
		DEC INDEX					;zmniejszamy iloœæ pozosta³ych elementów

	BRNE LOOP						;jump if not ZERO flag, gdy flaga zero w INDEX != 0 wykonaj pêtle jeszcze raz
	JMP KONIEC						;koniec elementów wiêc koñczymy 

	DODAJ:
		CLC							;trzeba wyczyœciæ flagê C bo tu mo¿e znów wyst¹piæ przeniesienie
		mov TMP , INDEX				;porównujemy czy  (index-1) - iloœæ w prawo > 0 je¿eli tak to mo¿emy przesuwaæ siê dalej
		DEC TMP						; je¿eli jest 0 to znaczy ¿e jesteœmy w ostatnim polu pamiêci zmiennej i nalezy
		sub TMP , wPRAWO			; zakoñczyæ przenoszenie przeniesienia z dodawania i cofn¹æ X o jeden w lewo alby 
		BREQ POPRAW2				; wróci³ w odpowiednie miejsce przed wyst¹pieniem przeniesienia

		INC wPRAWO					;zliczamy przesuniêcie
		LD R19 , X					;odczytujemy element na lewo od pola gdzie by³o przeniesienie
		LDI R20 , 1					;< == INC R19
		ADD R19, R20				;dodawanie zamiast INC bo INC nie zmienia flagi C
		st X , R19					;zapis wartoœci poprawionej o przeniesienie
		BRCS PRAWO					;by³o przeniesienie, trzeba przeskoczyæ do nastêpnej komórki
		BRCC POPRAW					;nie by³o przeniesienia, trzeba poprawiæ ewentualne przejœcia w lewo

	PRAWO:
		LD R19 , X+ 				;wskaŸnik X w prawo
		JMP DODAJ					;bezwarunkowo skaczemy do poprawy wartoœci

	POPRAW:					
		DEC wPRAWO				
		BRNE PRZESUN				;przesuwamy wskaŸnik X w miejsce ostatniej operacji
		JMP PO_DODANIU				;wracamy do g³ównej pêtli bo wskaŸniki s¹ poprawne

	PRZESUN:
		LD R19 , -X					;cofamy X o jedn¹komórkê w lewo
		JMP POPRAW

	POPRAW2:
		LD R19 , -X					;cofamy X o jedn¹komórkê w lewo bo to by³o nadmiarowe przenuniêcie w prawo - wychodz¹ce poza pamiêæ tej zmiennej
		JMP POPRAW					; poprawiamy pozostale przejscia w prawo

	KONIEC:
		NOP							;aby nie robic BREAK 
