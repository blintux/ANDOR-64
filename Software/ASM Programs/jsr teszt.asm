#bankdef "prog_bank"
{
	#addr 0
	#size 0x100
	#outp 0x0
}

#bankdef "data_bank"
{
	#addr 0x100
	#size 0x400
	#outp 0x100
}

#bank "prog_bank"


jsr func2
;nop

stop:
out c, #0x66
hlt



#bank "data_bank"

func2:
	out text

	out d, 65533
	out c, #0x20
	out d, 65534
	out c, #0x20
	out d, 65535

	jmp 65533
	;rts
	
	