out text


; min=127, max=136

stm 4000, #136				; Y position
stm 5000, #0				; Jump direction: 0 = None; 1 = Up; 2 = Down 
stm 6000, #0				; Button address

loop:




jump_init:
	lda 5000
	adc #0
	beq jump_check
	jmp jump_state

jump_check:
	sja 6000

jump_state:
	lda 6000
	cmp #1
	beq jump_start
	jmp jump_progress

jump_start:
	stm 5000, #1			; Jump up
	
jump_progress:
	lda 5000
	cmp #1
	beq up
	cmp #2
	beq down
	jmp prog_main
	
	up:
		dec 4000
		cmp #127
		beq up_end
		jmp prog_main
	down:
		inc 4000
		cmp #136
		beq down_end
		jmp prog_main
	
	up_end:
		stm 5000, #2		; Jump down
		jmp jump_end
	down_end:
		stm 5000, #0
	jump_end:
		stm 6000, #0
	
	
	
	
prog_main:

out i, #0x02
out d, 4000
out c, #0x20
out c, #0x20

jmp loop
