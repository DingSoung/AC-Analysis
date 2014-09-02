crc8TableInit:	;output crc table start from sram 0x10, sram 0x0C wil over write
	PUSH	R3
	PUSH	R2
	MOV	R3,	#0x00		;for (i = 0; i<256 ;) {
	fori:
	LDR	R0,	R3
	JP	B8,	=foriend
		
		MOV	R1,	#0x0C	;crc = i
		LDR	[R1],	R3
		
		MOV	R2,	#0x00	;for (j = 0; j<8 ;) {
		forj:
		LDR	R0,	R2
		JP	B3,	=forjend
			PUSH	R3
			PUSH	R2
			MOV	R1,	#0x0C	;C1 = (crc << 1) => R2
			LDR	R0,	[R1]
			RSL	R0,	#1
			AND	R0,	#0xFF
			LDR	R2,	R0
			MOV	R1,	#0x0C	;C2 = ((crc & 0x80) ? Poly : 0) => R3
			LDR	R0,	[R1]
			JP	B7,	=Poly
			MOV	R3,	#0x00
			JP	A1,	=afterPoly
			Poly:
			MOV	R3,	#0x07
			afterPoly:		;crc = (C1 ^ C2) & 0xFF
			LDR	R0,	R2
			XOR	R0,	R3
			AND	R0,	#0xFF
			MOV	R1,	#0x0C
			LDR	[R1],	R0
			POP	R2
			POP	R3
		LDR	R0,	R2	;j++
		ADD	R0,	#0x01
		LDR	R2,	R0
		JP	A1,	=forj
		forjend:
		
		MOV	R1,	#0x0C	;crc8table[i] = crc
		LDR	R0,	[R1]
		PUSH	R0
			MOV	R0,	#0x10
			ADD	R0,	R3
			LDR	R1,	R0
		POP	R0
		LDR	[R1],	R0
		
	LDR	R0,	R3	;i++
	ADD	R0,	#0x01
	LDR	R3,	R0
	JP	A1,	=fori
	foriend:
	POP	R2
	POP	R3
	RET
crc8Init:	;output sram 0x0C
	PUSH	R1
	MOV	R1,	#0x0C
	MOV	[R1],	#0x00
	POP	R1
	RET
crc8Update:	;input 1 byte date R0, output sram 0x0B
	PUSH	R0
	PUSH	R1
	MOV	R1,	#0x0C	;*pCrc8 = crc8table[pData[i] ^ *pCrc8];
	XOR	R0,	[R1]
	ADD	R0,	#0x10
	LDR	R1,	R0
	LDR	R0,	[R1]
	MOV	R1,	#0x0C
	LDR	[R1],	R0
	POP	R1
	POP	R0
	RET
crc8Fihshed:	;output SRAM 0x0C
	PUSH	R1
	PUSH	R0
	MOV	R0,	#0x00
	MOV	R1,	#0x0C
	XOR	R0,	[R1]
	LDR	[R1],	R0
	POP	R0
	POP	R1
	RET
