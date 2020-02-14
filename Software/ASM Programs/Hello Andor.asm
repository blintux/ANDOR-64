display_init:                   ; Init LCD display in text mode
	out i, #0x30			    ; Basic instruction set
	;jsr delay
	out i, #0x0c			    ; Display on, cursor off, char blink off
	;jsr delay
	out i, #0x01			    ; Clear Screen
	;jsr delay
	out i, #0x06			    ; Entry mode set
    ;jsr delay

start:
	out i, #0x01
	;jsr delay
    stm 0xff, #0                ; String upper mem address (0 = string in zeropage)
    ldi #hello                  ; Load first byte of string
    
main:
	out c, a, 0xff              ; Display current byte
	inc a                       ; Count to next byte
	cmp #len + hello            ; Is the last byte?
	beq stop                    ; Yes: Stop the program
	jmp main                    ; No: return to main loop

stop:
	;hlt                         ; Stop the program
	jmp start

delay:				    	    ; Delay function for display init
            ldi #5
	.loop:	sbc #1
    		beq .end
    		jmp .loop
	.end: 	rts

hello:                          ; Displayable string
	#str "Hello! I'm ANDOR"
	len = pc - hello
