/*Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak*/

.include "m32def.inc" 

.EQU LEN = 16
.DSEG ; segment danych
.ORG 0x60
tab1: .BYTE LEN
tab2: .BYTE LEN 
licz: .BYTE LEN

.CSEG	; segment kodu
.ORG 0 

START:
.DEF SaveSreg = R1 ; tu beziemy przechowywać SREG 
LDI R16 , low(RAMEND)
LDI R17 , high(RAMEND) ; inicjalizacja stosu na koncu pamieci
OUT SPL , R16 ; 
OUT SPH , R17 ; przekazanie wskaznikow dla wskaznika stosu

LDI XL , low(tab1)
LDI XH , high(tab1)

LDI YL , low(tab2)
LDI YH , high(tab2)

LDI ZL , low(LEN)
LDI ZH , high(LEN)

CALL DODAJ

END:
	JMP END

DODAJ:
	PUSH R19 ; wrzucamy na stos
	PUSH R20
	CLC ; zerujemy flagi 
OK:
	LD R19 , X+ ; wczytujemy X i przechodzimy dalej
	LD R20 , Y  ; wczytujemy Y
	ADC R19, R20 ; ADD z uwzględnieniem C
	ST Y+, R19 ; do Y to co otrzymaliśmy i przechodzimy dalej
	BRCS LS ; sprawdzamy czy mamy Carry ,jeśli tak to zapisujemy 
	SBIW Z ,1 ;zmniejszamy długość o 1 
	KK:
	BRNE OK 
        END1:
	POP R20 ; POP w odwrotnej kolejności 
	POP R19
	RET; powrót z procedury
	
	LS:
           IN SaveSreg,sreg ; SBIW zeruje flagi , więc zapisujemy 
	   SBIW Z ,1 ; zmniejszamy długość (nasz licznkik)
	   BREQ END1 ; jak mamy ostanie bity i przepełnienie 
	   OUT sreg,SaveSreg ; 
	   JMP KK ; wracamy
