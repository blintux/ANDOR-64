
display_init:                   ; Init LCD display in text mode
	out i, #0x30			    ; Basic instruction set
	out i, #0x0c			    ; Display on, cursor off, char blink off
	out i, #0x01			    ; Clear Screen
	out i, #0x06			    ; Entry mode set


; Read and display array 2th element
ldi #array
adc #1
lda array[15:8], a
out d, a

; Overwrite array 2th element
ldi #array
adc #1
stm array[15:8], a, #100
out c, #0x20

; Read and display array 2th element
ldi #array
adc #1
lda array[15:8], a
out d, a

hlt

; The array
array:
#d8 0x02, 0x04, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00