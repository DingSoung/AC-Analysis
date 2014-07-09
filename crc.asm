
crc32TableInit:	;output crc table start from sram 0x10, sram 0x0F wiil over write
	MOV	R3,	#0x00		;for (i = 0; ;) {
	gennumber:
		MOV	R1,	#0x0F	;c = (unsigned int)i
		LDR	[R1],	R3
		MOV	R2,	#0x00	;for (j = 0; ;) {
		genbit:
			PUSH	R2
			MOV	R1,	#0x0F	;c' = c
			LDR	R0,	[R1]
			LDR	R2,	R0	;temp of c'
			AND	R0,	#0x01	;if ((c' & 1) == 0)
			JP	Z,	=genbit0
			JP	A1,	=genbit1
			genbit1:		;c' = 0xedb88320L ^ (c' >> 1)
				LDR	R0,	R2
				RSR	R0,	#0x01
				LDR	R2,	R0
				MOV	R0,	#0x7FFF ;bit0 not save
				RSL	R0,	#0x10
				OR	R0,	#0xFFFF
				AND	R0,	R2
				LDR	R2,	R0
				MOV	R0,	#0xEDB8
				RSL	R0,	#0x10
				OR	R0,	#0x8320
				XOR	R0,	R2
				JP	A1,	=genbitdone
			genbit0:		;c' = (c' >> 1)
				LDR	R0,	R2
				RSR	R0,	#0x01
				JP	A1,	=genbitdone
			genbitdone:
			MOV	R1,	#0x0F	;c = c'
			LDR	[R1],	R0
			POP	R2
		LDR	R0,	R2	;j < 8
		XOR	R0,	#0x07
		JP	Z,	=genNext
		LDR	R0,	R2	; j++
		ADD	R0,	#0x01
		LDR	R2,	R0
		JP	A1,	=genbit
		genNext:
		MOV	R1,	#0x0F	;crc_table[i] = c
		LDR	R2,	[R1]
		LDR	R0,	R3	;offset at 0x10
		ADD	R0,	#0x10
		LDR	R1,	R0
		LDR	[R1],	R2
	LDR	R0,	R3		;i < 256;
	XOR	R0,	#0xFF
	JP	Z,	=crc32TableInitDone
	LDR	R0,	R3		; i++
	ADD	R0,	#0x01
	LDR	R3,	R0
	JP	A1,	=gennumber
	crc32TableInitDone:
	RET
crc32Init:	;output sram 0x0F
	PUSH	R1
	PUSH	R0
	MOV	R0,	#0xFFFF
	RSL	R0,	#0x08
	OR	R0,	#0xFFFF
	MOV	R1,	#0x0F
	LDR	[R1],	R0
	POP	R0
	POP	R1
	RET
crc32Update:	;input byte date R0, output sram 0x0F
	PUSH	R3
	PUSH	R2
	PUSH	R1
	LDR	R2,	R0	;argument R0 1byte
	;R3 = crc32tab[ (pData[i]) ^ ((*pCrc32) & 0x000000FF) ]
	MOV	R1,	#0x0F
	LDR	R0,	[R1]
	AND	R0,	#0x000000FF	;(pData[i]) = R2
	XOR	R0,	R2
	LDR	R1,	R0
	LDR	R3,	[R1]
	;R0 = (*pCrc32) >> 8)
	MOV	R1,	#0x0F
	LDR	R0,	[R1]
	RSR	R0,	#0x08
	;crc = R0 ^ R3
	XOR	R0,	R3
	;Store CRC
	MOV	R1,	#0x0F
	LDR	[R1],	R0
	POP	R1
	POP	R2
	POP	R3
	RET
crc32Fihshed:	;output SRAM 0x0F
	PUSH	R1
	PUSH	R0
	MOV	R0,	#0xFFFF
	RSL	R0,	#0x08
	OR	R0,	#0xFFFF
	MOV	R1,	#0x0F
	XOR	R0,	[R1]
	LDR	[R1],	R0
	POP	R0
	POP	R1
	RET
