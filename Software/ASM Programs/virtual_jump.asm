
;vjsr teszt

rcall teszt, #0



nop
nop
hlt


teszt:
	nop
	nop
	adc #1
	
	rcall teszt, #1
;	vrts
