start:
	out text
	stm 256, #0xff
	stm 255, #1					; Lower counter
	
	;stm wupper, #250

loop:
	inc 255
	sta wlower
	
	out i, #0x02
	out d, wupper
	out c, #0x20
	out d, wlower
	
	
	
	bcs next_bank
	jsr write_byte
	jsr read_byte
	jmp loop

next_bank:
	inc wupper
	jmp loop
	
write_byte:
	stm 256, #0xff
	wupper = pc - 1			; Felso
	wlower = pc - 2			; Also
	rts
	
read_byte:
	lda 256
	rupper = pc - 1			; Felso
	rlower = pc - 2			; Also
	rts
	
end_check:
	

stop:
	hlt
	