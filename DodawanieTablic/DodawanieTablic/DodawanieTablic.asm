/*
 * DodawanieTablic.asm
 *
 *   Author:  Mateusz Utkala, Oleksandr Kuzhel, Klaudia Olejniczak
 */ 


 .include  "m32def.inc"; wczytuje sterowanie, nazwy portow etc. kontrolera 
.EQU LEN = 16 ;dlugosc tablic
.DSEG
.org 0x01FF;
tab1: .BYTE LEN
tab2: .BYTE LEN

.CSEG ;ustalamy segment kodu
.org 0; zaczynamy od adresu zero
 
;tab1
LDI XL , low(tab1);mniej znaczacy bit
LDI XH , high(tab1);bardziej znaczacy bit
;tab2 
LDI ZL , low(tab2);mniej znaczacy bit
LDI ZH , high(tab2);bardziej znaczacy bit 

CLC
ADC ZL,XL
ADC ZH,XH