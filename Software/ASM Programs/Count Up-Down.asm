from = 1
to   = 20

COUNT = 0xff				; Counter memory address

display_init:
	out i, #0x30			; Basic instruction set
	jsr delay
	out i, #0x0c			; Display on, cursor off, char blink off
	jsr delay
	out i, #0x01			; Clear Screen
	jsr delay
	out i, #0x06			; Entry mode set

start:
	stm COUNT, #from

up:
	inc COUNT
	jsr display
	cmp #to
	beq down
	jmp up
	
down:
	dec COUNT
	jsr display
	cmp #from
	beq up
	jmp down

display:
	out i, #0x02
	out d, COUNT
	out c, #0x20
	out c, #0x20			
	rts
	
delay:
			ldi #3
	.loop:	sbc #1
			beq .end
			jmp .loop
	.end: 	rts
