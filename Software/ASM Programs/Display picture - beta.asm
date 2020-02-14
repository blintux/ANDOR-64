#bankdef "prog_bank" {	#addr 0			#size 0x100		#outp 0x0	}
#bankdef "data_bank" {	#addr 0x100		#size 0x400		#outp 0x100 }

#bank "prog_bank"

display_init:
	out i, #0x30			; Basic instruction set
	jsr delay
	out i, #0x0c			; Display on, cursor off, char blink off
	jsr delay
	out i, #0x01			; Clear Screen
	jsr delay
	out i, #0x06			; Entry mode set
	jsr delay
	out i, #0x34			; Extended instruction set
	jsr delay
	out i, #0x36			; Graphic display on


jsr display_picture
hlt


display_picture:
	stm 3000, #graph[7:0] 		; Picture data start location - lower
	stm 3001, #graph[15:8]  	; Picture data start location - upper
	stm 3002, #128				; Starting Y coord
	stm 3003, #128				; Starting X coord

	.set_row:
		out i, 3002		    	; Set Y coordinate
		out i, 3003		    	; Set X coordinate
		inc 3002		    	; Increase Y coordinate
		stm 3004, #17			; Bytes in row

	.draw_loop:
		dec 3004
		beq .set_row
		lda 3000				; Load lower address 
		out c, aa, 3001			; Display data from lower and upper address
		inc 3000
		bcs .next_bank
		jmp .draw_loop

	.next_bank:
		inc 3001
		cmp #graph[15:8] + 2
		beq .bottom_half		; Switch screen bottom half
		cmp #graph[15:8] + 4
		beq .end 
		jmp .set_row

	.bottom_half:
		stm 3003, #136			; X coord (display bottom half)
		stm 3002, #128			; Y coord
		jmp .set_row			; Continue drawing
		
	.end:
		rts
	
	
	

delay:				    	; Delay function
		ldi #50
	.loop:	sbc #1
		beq .end
		jmp .loop
	.end: 	rts



#bank "data_bank"

graph:
;#inchexstr "pictures/splash_mindtrial.bin" 
#inchexstr "pictures/lcd_test.bin"
;#inchexstr "pictures/andor.bin"
;#inchexstr "pictures/pic_boat.bin"
;#inchexstr "pictures/pong.bin"
;#inchexstr "pictures/dino.bin"