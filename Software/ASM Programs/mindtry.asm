#bankdef "prog_bank" { #addr 0x0,  #size 0x600, #outp 0x0   }		; 1.5 Kb
#bankdef "data_bank" { #addr 0x600,#size 0x100, #outp 0x600 }		; 250 Byte
#bankdef "symbols"   { #addr 0x700,#size 0x100, #outp 0x700 }		; 250 Byte
#bankdef "splash"    { #addr 0x800,#size 0x400, #outp 0x800 }		; 1.0 Kb

TILES		=	12000
TILE_X		=	12001						; Tile X position
TILE_Y		=	12002						; Tile Y position
TILE_SPRITE	=	12003						; Tile sprite start address
TILE_Y_SIZE	=	12004						; Tile height (8 pixel) 
TEMP_1		=   12005						; Temporary varible address 1
TEMP_2		=	12006						; Temporary varible address 2
CURRENT		=	12007						; Current tile 
PREV		=	12008						; Previous tile  
SELECTED_1	=	12009						; Selected tile 1
SELECTED_2	=	12010						; Selected tile 2
SELECT		=	12011						; We have selected tile or not
TILES_LEFT	=	12012						; How many tile remaining left
TILE_HADDR	=	12013						; Tile sprite upper address
DELAY_CONT	=	12014						; Delay countdown
DELAY_MLT	=	12015						; Delay countdown multiplier
TRIES		=	12016						; Number of tries (score) 
DIFFICUULTY	=	12017

#bank "prog_bank"

START:

init_display:								; Initialize display in graphic mode
	stm DELAY_CONT, #20
	stm DELAY_MLT, #1
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
	out i, #0x36							; Graphic display on
	
init_sound:									; Initialize sound (mute all channels)
	jsr mute

draw_slaph_screen:
	jsr draw_splash

select_difficulty:
	stm DIFFICUULTY, #1						; Initial game difficulty
	stm TILES,       #8						; Initial tiles (8 for difficulty 1)
	stm TILES_LEFT,  #4 
	stm TILE_Y,      #139
	stm TILE_X,      #137

	jmp .draw
	
	.select:
		lja a								; Read JoyA port to A register
		cmp #1								; Button: Top
		beq .prev
		cmp #2								; Button: Down
		beq .next		
		cmp #10		
		beq .start
		jmp .select
			
	.prev:
		lda DIFFICUULTY
		cmp #1
		beq .select
		dec DIFFICUULTY
		jmp .draw_arrow
	.next:
		lda DIFFICUULTY
		cmp #4
		beq .select
		inc DIFFICUULTY
		jmp .draw_arrow
	.draw_arrow:
		out i, TILE_Y
		out i, TILE_X
		out c, #0x00
		dec TILE_Y
		out i, TILE_Y
		out i, TILE_X
		out c, #0x00
		dec TILE_Y
		out i, TILE_Y
		out i, TILE_X
		out c, #0x00
		
		lda DIFFICUULTY
		cmp #1
		beq .arrow_pos_1
		cmp #2
		beq .arrow_pos_2
		cmp #3
		beq .arrow_pos_3
		cmp #4
		beq .arrow_pos_4
		
		.arrow_pos_1:
			stm TILES, #8
			stm TILES_LEFT,	#4
			stm TILE_Y, #139
			stm TILE_X, #137
			jmp .draw
		.arrow_pos_2:
			stm TILES, #16
			stm TILES_LEFT,	#8
			stm TILE_Y, #150
			stm TILE_X, #137
			jmp .draw
		.arrow_pos_3:
			stm TILES, #24
			stm TILES_LEFT,	#12
			stm TILE_Y, #139
			stm TILE_X, #140
			jmp .draw
		.arrow_pos_4:
			stm TILES, #32
			stm TILES_LEFT,	#16
			stm TILE_Y, #150
			stm TILE_X, #140
		
		.draw:
			out i, TILE_Y
			out i, TILE_X
			out c, #7
			inc TILE_Y
			out i, TILE_Y
			out i, TILE_X
			out c, #7
			inc TILE_Y
			out i, TILE_Y
			out i, TILE_X
			out c, #7
			
			lja a
			
		jmp .select
	.start:



new_game:
	stm TRIES,			#0					; Number of tires 
	stm TEMP_1, 		#0					; Temporary varaible 1
	stm TEMP_2,			#0					; Temporary varaible 2
	stm SELECT, 		#0					; Any tile is selected? (0: No; 1: Yes)
	stm SELECTED_1, 	#100				; Selected tile 1 (Default 100: No valid tile)
	stm SELECTED_2, 	#100				; Selected tile 1 (Default 100: No valid tile)
		
	ldi #progress							; Reset progress array (fill with zero values)
	.loop:
		stm progress[15:8], a, #0
		inc a
		cmp #progress + 16
		beq .done 
		jmp .loop
	.done:
	
	jsr clear_screen						; Clear screen
	lja a									; Reset joystick port A
	
prepare_tiles:	
	.loop:
		lda TEMP_1
		sta TILE_X
		stm TILE_HADDR, #hidden[15:8]
		stm TILE_SPRITE, #hidden
		jsr draw_tile
		inc TEMP_1
		cmp TILES
		beq .done	
		jmp .loop
	.done:
		stm CURRENT, #0						; Current tile is first
		stm TILE_X,  #0						; Set cursor to first tile
		stm TILE_SPRITE, #cursor
		stm TILE_HADDR, #cursor[15:8]
		jsr draw_tile


game_loop:
	.check_win:
		lda TILES_LEFT
		cmp #0
		beq victory
	
	.check_button:
		lja a								; Read JoyA port to A register
		cmp #1								; Button: Top
		beq .top
		cmp #2								; Button: Down
		beq .down		
		cmp #4								; Button: Left
		beq .left
		cmp #8								; Button: Right
		beq .right
		cmp #10								; Button: Select
		beq .select
		jmp .check_button_end				; No button press? Jump to procedure end

	.right:
		lda CURRENT
		sta PREV
		adc #1
		cmp TILES							; Cursor on last tile?
		beq .check_button_end				; Yes: Skip moving
		inc CURRENT							; No: Increase cursor position
		jmp .move_cursor					; and move cursor
	.left:
		lda CURRENT
		sta PREV
		cmp #0								; Cursor on first tile?
		beq .check_button_end				; Yes: Skip moving
		dec CURRENT							; No: Decrease cursor position
		jmp .move_cursor					; and move cursor
	.down:
		lda CURRENT
		sta PREV
		sbc TILES
		adc #8
		bmi .cont_down
		jmp .check_button_end
		.cont_down:
		lda CURRENT
		adc #8
		sta CURRENT
		jmp .move_cursor
	.top:
		lda CURRENT
		sta PREV
		sbc #8
		bmi .check_button_end
		lda CURRENT
		sbc #8
		sta CURRENT
		jmp .move_cursor
	.select:
		ldi #progress
		adc CURRENT
		lda progress[15:8], a
		cmp #1
		beq .check_button_end
		
		lda SELECTED_1						; Selection 1 is default value?
		cmp #100
		beq .select_1						; Yes: Save selection to selection 1
		jmp .select_2						; No: Save selection to selection 2
		
		.select_1:
		lda CURRENT
		sta SELECTED_1
		sta TILE_X
		stm SELECT, #1
		jsr draw_tile
		jmp .check_button_end
		
		.select_2:
		lda CURRENT
		sta SELECTED_2
		sta TILE_X
		stm SELECT, #1
		jsr draw_tile
		jmp .check_button_end

	.move_cursor:
		lda SELECTED_1
		cmp PREV							; Prev is selected?
		beq .keep							; Yes: Keep this selected
		jmp .continue1						; No: Continue
		.keep:
			stm SELECT, #1
		.continue1:
		
		lda PREV
		sta TILE_X
		stm TILE_SPRITE, #hidden
		stm TILE_HADDR, #hidden[15:8]
		
		ldi #progress
		adc TILE_X
		lda progress[15:8], a
		cmp #1
		beq .skipp
		jmp .continue2
		.skipp:
			stm TILE_SPRITE, #free
			stm TILE_HADDR, #free[15:8]
		.continue2:
		
		jsr draw_tile
		lda CURRENT
		sta TILE_X
		stm TILE_SPRITE, #cursor
		stm TILE_HADDR, #cursor[15:8]
		jsr draw_tile
	
	.check_button_end:
	
	
	.check_selected:
		lda SELECTED_1
		sbc SELECTED_2						; Two selected tile is same?
		beq game_loop						; Yes: Return to game loop
		
		lda SELECTED_1		
		cmp #100							; Selected 1 is default?
		beq game_loop						; Yes: Return to game loop
		
		lda SELECTED_2
		cmp #100							; Selexcted 2 is default?
		beq game_loop						; Yes: Return to game loop
		
		ldi #pairs
		adc SELECTED_1
		lda pairs[15:8], a
		sta TEMP_1
		
		ldi #pairs
		adc SELECTED_2
		lda pairs[15:8], a
		
		stm DELAY_CONT, #255
		stm DELAY_MLT, #3
		
		cmp TEMP_1
		beq .hit
		jmp .no_hit
		
		.hit:
			jsr beep
			jsr delay
			jsr mute
			
			stm SELECT, #0
			
			; Overwrite array 2th element
			ldi #progress
			adc SELECTED_1
			stm progress[15:8], a, #1
			
			ldi #progress
			adc SELECTED_2
			stm progress[15:8], a, #1
			
			lda SELECTED_1
			sta TILE_X
			stm TILE_SPRITE, #free
			stm TILE_HADDR, #free[15:8]
			jsr draw_tile
			
			lda SELECTED_2
			sta TILE_X
			stm TILE_SPRITE, #cursor
			stm TILE_HADDR, #cursor[15:8]
			jsr draw_tile
					
			dec TILES_LEFT
			
			jmp .reset_selection
		
		.no_hit:
			jsr delay
			stm SELECT, 	#0
						
			lda SELECTED_1
			sta TILE_X
			stm TILE_SPRITE, #hidden
			stm TILE_HADDR, #hidden[15:8]
			jsr draw_tile
			
			lda SELECTED_2
			sta TILE_X
			stm TILE_SPRITE, #cursor
			stm TILE_HADDR, #cursor[15:8]
			jsr draw_tile
		
		.reset_selection:
			inc TRIES						; 
			stm SELECTED_1, #100
			stm SELECTED_2, #100
			lja a

	jmp game_loop

	
; 0->7; 8->15; 16->23; 24->31

draw_tile:
	lda TILE_X
	sta TEMP_1
	sbc #8
	bmi row_1
	sbc #8
	bmi row_2
	sbc #8
	bmi row_3
	jmp row_4
	
	row_1:
		lda TILE_X
		adc #128
		sta TILE_X
		stm TILE_Y, #128
		jmp draw
	row_2:
		lda TILE_X
		adc #136
		sta TILE_X
		stm TILE_Y, #144
		jmp draw
	row_3:		
		lda TILE_X
		adc #136
		sta TILE_X
		stm TILE_Y, #128		
		jmp draw
	row_4:
		lda TILE_X
		adc #128
		sta TILE_X
		stm TILE_Y, #144		

	draw:
		stm TILE_Y_SIZE, #16
		
		lda SELECT
		cmp #1
		beq .set_symbol_sprite
		jmp .loop
		
		.set_symbol_sprite:
			ldi #pairs
			adc TEMP_1
			lda pairs[15:8], a
			

			sta TEMP_1
			stm TEMP_2, #0
			.cacl:
				dec TEMP_1
				beq .tov
				lda TEMP_2
				adc #32
				sta TEMP_2
				jmp .cacl
			.tov:
			
			ldi #symbols
			adc TEMP_2
			

			sta TILE_SPRITE
			stm TILE_HADDR, #symbols[15:8]
			
			.continue:
				
			stm SELECT, #0
		
	.loop:
		out i, TILE_Y						; Set sprite Y position
		out i, TILE_X						; Set sprite X position
		lda TILE_SPRITE						; Tile sprite data start address
		out c, aa, TILE_HADDR
		inc TILE_SPRITE
		out c, aa, TILE_HADDR
		inc TILE_SPRITE
		inc TILE_Y							; Increase Y position
		dec TILE_Y_SIZE						; Decrease number of lines
		beq .end							; Last line? Yes: jump end
		jmp .loop							; No: Draw next line 
	.end:
		rts


victory:
	stm DELAY_CONT, #20
	stm DELAY_MLT, #1
	out i, #0x30			   				; Basic instruction set
	jsr delay
	out i, #0x0c			   				; Display on, cursor off, char blink off
	jsr delay
	out i, #0x01							; Clear Screen
	jsr delay
	out i, #0x06			   				; Entry mode set
    jsr delay
    
    ldi #text_game_end
    .loop:
		out c, a, text_game_end[15:8]
		adc #1
		cmp #win_text_len + text_game_end
		beq .show_tries
		jmp .loop
    .show_tries:
    	out i, #0x96
    	out d, TRIES
    .wait:
    	lja a
    	cmp #0
    	beq .wait
    	jmp START

draw_splash:
	stm 13000, #splash[7:0] 				; Picture data start location - lower
	stm 13001, #splash[15:8]  				; Picture data start location - upper
	stm 13002, #128							; Starting Y coord
	stm 13003, #128							; Starting X coord

	.set_row:
		out i, 13002		    			; Set Y coordinate
		out i, 13003		    			; Set X coordinate
		inc 13002		    				; Increase Y coordinate
		stm 13004, #17						; Bytes in row

	.draw_loop:
		dec 13004
		beq .set_row
		lda 13000							; Load lower address 
		out c, aa, 13001					; Display data from lower and upper address
		inc 13000
		bcs .next_bank
		jmp .draw_loop

	.next_bank:
		inc 13001
		cmp #splash[15:8] + 2
		beq .bottom_half					; Switch screen bottom half
		cmp #splash[15:8] + 4
		beq .end 
		jmp .set_row

	.bottom_half:
		stm 13003, #136						; X coord (display bottom half)
		stm 13002, #128						; Y coord
		jmp .set_row						; Continue drawing
		
	.end:
		rts
		
clear_screen:
	stm 12800, #128
	out i, #128, #128						; Y, X Coordinate

	.start:
		ldi #8
	.loop:
		out c, #0b0, x4
		sbc #1
		beq .over
		jmp .loop
	.over:
		inc 12800
		out i, a, #128						; Y, X Coordinate (Y from A reg)
		
		cmp #160
		beq .end
		jmp .start
	.end:
		out c, #0x00						; !
		rts

beep:
	snd #0x83	; sets channel 0 tone to 0x123
	snd #0x12	
	snd #0x90	; sets channel 0 to loudest possible
	rts	
		
mute:
	snd #0x9f
	snd #0xbf
	snd #0xdf
	snd #0xff
	rts
	

delay:				    					; Delay function ( DELAY_CONT / DELAY_MLT ) 
	lda DELAY_MLT
	sta TEMP_1
	.main:
			lda DELAY_CONT
	.loop:	sbc #1
			beq .end
			jmp .loop
	.end:
			dec TEMP_1
			beq .done
			jmp .main 
	.done:
			rts
	
	
#bank "data_bank"

progress:									; Released tiles array
	#res 32									; Reserve 32 bytes with 0x00

free:										; Released tile sprite (empty area)
	#res 32									; Reserve 32 bytes with 0x00
	
pairs:
	#d8 1,2,3,4,1,2,3,4,5,6,7,8,5,6,
		7,8,9,9,10,10,11,11,12,12,5,
		5,4,4,3,3,2,2

cursor:										; Tile cursor
#d8 0xff, 0xfe, 0x80, 0x02, 0x80, 0x02, 
	0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 
	0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 
	0x80, 0x02, 0x80, 0x02, 0x80, 0x02, 
	0x80, 0x02, 0x80, 0x02, 0xff, 0xfe, 
	0x00, 0x00

hidden:										; Hidden tile
#d8 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 
	0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 
	0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 
	0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 
	0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 
	0x00, 0x00

arrow:										; Arrow cursor
#d8	252, 254, 255, 254, 252

text_game_end:								; End game text
	#str "CONGRATULATIONS!"
	#str "Press a button  "
	#str "Your tries:     "
	#str "to main menu."
	
	win_text_len = pc - text_game_end 

; Symbol graphics

#bank "symbols"
symbols:
	#inchexstr "pictures/mindtrial_symbols.bin"

#bank "splash"
splash:
	#inchexstr "pictures/mindtrial_splash.bin"