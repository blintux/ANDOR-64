#bankdef "prog_bank" { #addr 0x0,  #size 0x400, #outp 0x0   }
#bankdef "data_bank" { #addr 0x400,#size 0x400, #outp 0x400 }

#bank "prog_bank"


init_display:								; Initialize display in graphic mode
	out i, #0x30							; Basic instruction set
	jsr delay
	out i, #0x0c							; Display on, cursor off, char blink off
	jsr delay
	out i, #0x01							; Clear Screen
	jsr delay
	out i, #0x06							; Entry mode set
	jsr delay
	out i, #0x34							; Extended instruction set
	jsr delay
	out i, #0x01
	
	
	ldi #237
	out d, a
	nop
	
	nop


hlt
	
	
	
	
	
	
delay:				    					; Delay function ( DELAY_CONT / DELAY_MLT ) 
	ldi #2
	sta 2000
	ldi #1
	sta 2001
	.main:
			lda 2000
	.loop:	sbc #1
			beq .end
			jmp .loop
	.end:
			dec 2001
			beq .done
			jmp .main 
	.done:
			rts
