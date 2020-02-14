;------------------------------------------------------------------------------
;  Definitions
;------------------------------------------------------------------------------

; Address variables
COORD_Y    = 0xff
COORD_X    = 0xfe
LINE_BYTES = 0xfd
DATA_BYTE  = 0xfc
DATA_BANK  = 0xfb
BANK_COUNT = 0xfa
PICTURE_ID = 0xf9

; Display commands
BASIC_INST = 0x30                       ; Basic instruction set
SCR_MODE   = 0x0c                       ; Display on, cursor off, char blink off
SCR_CLEAR  = 0x01                       ; Clear Screen
ENTRY_MODE = 0x06                       ; Entry mode
EX_INST    = 0x34                       ; Extended instruction set
GRAPH_MODE = 0x36                       ; Graphic display on

#bankdef "prog_bank"                    ; Program bank (zeropage)
{
	#addr 0
	#size 0x100
	#outp 0x0
}

#bankdef "data_bank"                    ; Data bank (Upper 2 Kb)
{
	#addr 0x100
	#size 0x800
	#outp 0x100
}

;------------------------------------------------------------------------------
;  Startup
;------------------------------------------------------------------------------

#bank "prog_bank"

display_init:                           ; Init LCD display in graphic mode
	out i, #BASIC_INST
	jsr delay
	out i, #SCR_MODE
	jsr delay
	out i, #SCR_CLEAR
	jsr delay
	out i, #ENTRY_MODE
	jsr delay
	out i, #EX_INST
	jsr delay
	out i, #GRAPH_MODE

picture_1:                              ; Picture 1
	stm DATA_BANK, #1
	stm PICTURE_ID, #1
	jmp main
    
picture_2:                              ; Picture 2
	stm DATA_BANK, #5
	stm PICTURE_ID, #2
	jmp main

;------------------------------------------------------------------------------
;    Main Loop
;------------------------------------------------------------------------------

main:
    stm DATA_BYTE, #0               ; Picture data start address low
    stm COORD_Y, #128	        	; Starting Y coord
    stm COORD_X, #128	        	; Starting X coord (display top)
    stm BANK_COUNT, #1
	
line_loop:
    out i, COORD_Y                  ; Set Y coordinate
    out i, COORD_X		    	    ; Set X coordinate
    inc COORD_Y		    	        ; Increase Y coordinate (1 = 1 line)
    stm LINE_BYTES, #16             ; Bytes in line

    .loop:                          ; Write line loop
        lda DATA_BYTE
        out c, a, DATA_BANK         ; Display byte (Lower addres in A reg, upper in bank)
        inc DATA_BYTE
        bcs .next_bank              ; End of bank? Yes: switch to next bank
        dec LINE_BYTES
        beq line_loop               ; End of line? Yes: jump to new line
        jmp .loop                   ; Else draw more

    .next_bank:
        stm DATA_BYTE, #0 		    ; Picture data lower start address
        inc DATA_BANK
        inc BANK_COUNT
        cmp #3
        beq .bottom_half            ; Switch screen bottom half 
        cmp #5
        beq switch_picture          ; Last bank? Yes: Jump to picture switch
        jmp line_loop                             ; Else draw new line

    .bottom_half:
        stm COORD_X, #136		    ; X coord (display half)
        stm COORD_Y, #128		    ; Y coord
        jmp line_loop   		    ; Continue drawing

switch_picture:
    lda PICTURE_ID
    cmp #1                          ; If this picture 1 jump to picture 2
    beq picture_2
    jmp picture_1                   ; Else jump picture 1

delay:                              ; Delay loop for LCD init
    ldi #50
    .loop:  sbc #1
            beq .end
            jmp .loop
    .end:   rts

;------------------------------------------------------------------------------
;    Data stuff
;------------------------------------------------------------------------------

#bank "data_bank"

graph:                              ; Include picture data file
#inchexstr "pictures/andor_anim.bin"
