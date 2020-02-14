#bankdef "prog_bank" { #addr 0x0,  #size 0x200, #outp 0x0   }
#bankdef "data_bank" { #addr 0x200,#size 0x400, #outp 0x200 }

#bank "prog_bank"

; =============================  Variables  =============================

; Memory address
DRAW_COUNT		= 0x2000					; Main draw counter
PLAYER_Y_POS	= 0x2003					; PLAYERsaur Y start position
PLAYER_BYTES	= 0x2004					; PLAYERsaur byte count
PLAYER_SPRITE	= 0x2005					; Current PLAYERsaur sprite
JUMP_STATE		= 0x2006					; 
JUMP_TIME		= 0x2007					;  
ENEMY_X_POS     = 0x2008					;  
ENEMY_Y_POS     = 0x2009					;  
ENEMY_BYTES		= 0x2010					; PLAYERsaur byte count
ENEMY_SPRITE	= 0x2011					; Current PLAYERsaur sprite 
ENEMY_SHIFT		= 0x2013					;  
ENEMY_SPEED		= 0x2014					;    
GAME_SPEED      = 0x2015

; Values
stm DRAW_COUNT,   #0						; Glovbal draw counter
stm PLAYER_BYTES, #16						; Draw byte counter for player
stm PLAYER_Y_POS, #143						; Initial player Y position
stm JUMP_STATE,   #0						; Jump state: 0 = no, 1 = yes
stm ENEMY_X_POS,  #143						; 
stm ENEMY_Y_POS,  #143						; 
stm ENEMY_BYTES,  #16						; 
stm ENEMY_SPRITE, #ENEMY_f - 1				; Enemy sprite address
stm ENEMY_SHIFT,  #1						; Enemy frame shift or not


stm JUMP_TIME,    #40						;
stm ENEMY_SPEED,  #10						; 10 -> 0; slow -> fast
stm GAME_SPEED,   #10


; =============================  Inicialize  =============================

init:										; Init display in graphic mode
	out i, #0x30							; Basic instruction set
	jsr init_delay
	out i, #0x0c							; Display on, cursor off, char blink off
	jsr init_delay
	out i, #0x01							; Clear Screen
	jsr init_delay
	out i, #0x06							; Entry mode set
	jsr init_delay
	out i, #0x34							; Extended instruction set
	jsr init_delay
	out i, #0x36							; Graphic display on
	
	jsr clear_screen						; Clear the screen


; ==========================  Game loop start ==========================

game_loop:									; Game main loop begin
	
; ===========================  Jump function ============================

jump:
	lda JUMP_STATE							; Jump in progress?
	cmp #1
	beq .jump_progress						; No: go jump progress
	jmp .check_state						; Else: check button press
	.check_state:
		lja a								; Load JoyA port value to A register
		cmp #0x01							; Top button
		beq .start_jump
		jmp .continue
		.start_jump:
			stm JUMP_STATE, #1
			stm PLAYER_Y_POS, #127
			
			ldi #159						; Clear previous frame
			.clear_1:
				out i, a, #136
				out c, #0x00, x3
				sbc #1
				cmp #143
				beq .jump_progress
				jmp .clear_1
			
	.jump_progress:
		dec JUMP_TIME
		beq .jump_end
		jmp .continue
		 
	.jump_end:
		stm JUMP_STATE,   #0
		stm PLAYER_Y_POS, #143
		
		stm JUMP_TIME,    #40
		
		ldi #143							; Clear previous frame
		.clear_2:							; Elozovel eggyesiteni kellene!
			out i, a, #136
			out c, #0x00, x3
			sbc #1
			cmp #127
			beq .continue
			jmp .clear_2
	
	.continue:


; ===========================  Draw Enemy =============================


dec ENEMY_SPEED
beq move_enemy 
jmp move_enemy_end

move_enemy:

	lda GAME_SPEED
	sta ENEMY_SPEED

	dec ENEMY_SHIFT
	beq .loop
	jmp .dec_position

	.dec_position:
		stm ENEMY_SHIFT, #1
		dec ENEMY_X_POS
		
		
		cmp #136						; Auto jump
		beq .auto_jump
		jmp .no_auto_jump
		.auto_jump:
			stm JUMP_STATE, #1
			stm PLAYER_Y_POS, #127
		.no_auto_jump:
		
		
		cmp #135
		beq .check_collision
		jmp .loop
		
	.check_collision:
		lda PLAYER_Y_POS
		
		cmp #143
		beq game_over
			
	.reset_position:
		stm ENEMY_X_POS, #143
		
		ldi #144	
		.hide_loop:							; Hide enemy
			inc a
			out i, a, #136
			out c, #0x00, x2
			cmp #159
			beq move_enemy_end
		 	jmp .hide_loop
		
	.loop:
		inc ENEMY_Y_POS						; Inc Y position
		out i, ENEMY_Y_POS
		out i, ENEMY_X_POS
		
		lda ENEMY_SHIFT						; Shift enemy frame?
		cmp #1
		beq .shift_yes						; Yes
		jmp .shift_no						; No
		
		.shift_yes:
			out c, #0x00
			
		.shift_no:
		
		inc ENEMY_SPRITE
		out c, a, 0x2						; Low addr in A reg, upper immediate
		inc ENEMY_SPRITE
		out c, a, 0x2
		out c, #0x00						; Hide prev frame
		dec ENEMY_BYTES						; Inc byte counter
		beq .end
		jmp .loop
	
	.end:
		lda ENEMY_SPRITE					; Reset sprite begin address
		sbc #32
		sta ENEMY_SPRITE
		lda ENEMY_Y_POS						; Reset sprite Y position
		sbc #16
		sta ENEMY_Y_POS
		stm ENEMY_BYTES, #16				; Reset sprite byte lenght
	
move_enemy_end:


; ===========================  Draw Player =============================


draw_PLAYER:
	lda DRAW_COUNT
	cmp #14
	beq .sprite_1
	cmp #0
	beq .sprite_2
	jmp .loop
	.sprite_1:
		stm PLAYER_SPRITE, #PLAYER_f1 - 1
		jmp .loop
	.sprite_2:
		stm PLAYER_SPRITE, #PLAYER_f2 - 1
		jmp .loop
		
	.loop:
		inc PLAYER_Y_POS					; Inc Y position
		out i, a, #136						; Y, X Coordinate (Y from A reg)
		inc PLAYER_SPRITE
		out c, a, 0x2						; Low addr in A reg, upper immediate
		inc PLAYER_SPRITE
		out c, a, 0x2
		dec PLAYER_BYTES					; Inc byte counter
		beq .end
		jmp .loop
	.end:
		lda PLAYER_SPRITE					; Reset sprite begin address
		sbc #32
		sta PLAYER_SPRITE
		lda PLAYER_Y_POS					; Reset sprite Y position
		sbc #16
		sta PLAYER_Y_POS
		stm PLAYER_BYTES, #16				; Reset sprite byte lenght
		
		
; ==========================  Game loop end =========================

game_loop_end:								; Gemae loop end
	inc DRAW_COUNT							; Inc draw counter
	cmp #30
	beq .reset
	jmp game_loop
	
	.reset:
		stm DRAW_COUNT, #0					; Reset Draw counter
		
	jmp game_loop
	

; ==============================  DATA ==============================

#bank "data_bank"

game_over:
	;stm 6666, #16
	;stm 6667, #RIP
	;out i, #136, #143
	
	;.loop:
	;	out c, a, 0x2
	;	inc PLAYER_SPRITE
	;	out c, a, 0x2
	;	inc PLAYER_SPRITE
	
	.wait:
		lja a
		cmp #0x0
		beq .wait
		jmp 0x00



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
		out i, a, #128					; Y, X Coordinate (Y from A reg)
		
		cmp #160
		beq .end
		jmp .start
	.end:
		out c, #0x00					; Mivel az utolso egy parancs volt, kell egy kiiras (javitani kell)
		rts


game_delay:				    			; Delay function
			ldi #255
	.loop:	sbc #1

			beq .end
			jmp .loop
	.end: 	rts	

init_delay:				    			; Delay function
			ldi #50
	.loop:	sbc #1
			beq .end
			jmp .loop
	.end: 	rts
	
	
; ===========================  GRAPHICS ===========================

PLAYER_f1:
#d8 0x00, 0x00, 0x03, 0xE0, 0x0F, 0xF8, 0x1F, 0xFC, 0x3D, 0xFE, 0x3F, 
	0xF8, 0x7F, 0xE0, 0x7F, 0x80, 0x7E, 0x00, 0x7F, 0x80, 0x7F, 0xE0, 
	0x3F, 0xF8, 0x3F, 0xFE, 0x1F, 0xFC, 0x0F, 0xF8, 0x03, 0xE0

PLAYER_f2:
#d8 0x00, 0x00, 0x03, 0xE0, 0x0F, 0xF8, 0x1F, 0xFC, 0x3D, 0xFE, 0x3F, 
	0xFE, 0x7F, 0xFF, 0x7F, 0xFF, 0x7F, 0x00, 0x7F, 0xFF, 0x7F, 0xFF, 
	0x3F, 0xFE, 0x3F, 0xFE, 0x1F, 0xFC, 0x0F, 0xF8, 0x03, 0xE0

ENEMY_f:
#d8 0x00, 0x00, 0x03, 0xC0, 0x0F, 0xF0, 0x1F, 0xF8, 0x27, 0x9C, 0x03, 
	0x0C, 0x73, 0xCE, 0x73, 0xCE, 0x67, 0x9E, 0x7F, 0xFE, 0x7F, 0xFE, 
	0x7F, 0xFE, 0x7F, 0xFE, 0x7F, 0xFE, 0x7B, 0xDE, 0x31, 0x8C
	
;RIP:
;#d8 0x07, 0xC0, 0x1F, 0xF0, 0x3E, 0xF8, 0x7C, 0x7C, 0x7E, 0xFC, 0xFE, 
;	0xFE, 0xFE, 0xFE, 0xFF, 0xFE, 0xFF, 0xFE, 0xE2, 0x8E, 0xEA, 0xAE, 
;	0xE2, 0x8E, 0xE6, 0xBE, 0xEA, 0xBE, 0xFF, 0xFE, 0xFF, 0xFE

