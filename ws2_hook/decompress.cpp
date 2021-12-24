#include "stdafx.h"

extern "C" __declspec(naked) long __stdcall lzo_decompress (data8 *inbuf, data32 inlen, data8 *outbuf, data32 *outlen, data32 mode)
{
__asm {
	MOV       EAX,[ESP+0x08]; // len
	PUSH      EBX;
	MOV       EBX,[ESP+0x14]; // outlen
	PUSH      EBP;
	PUSH      ESI;
	MOV       ESI,[ESP+0x10]; // inbuf
	MOV       DWORD PTR [EBX],0x00000000;
	MOV       DL,[ESI];
	CMP       DL,0x11;
	LEA       EBP,[EAX+ESI];
	MOV       EAX,[ESP+0x18]; // outbuf
	PUSH      EDI;
	MOV       ECX,ESI;
	JBE      _0070D993;

	MOVZX     EDI,DL;
	SUB       EDI,0x11;
	CMP       EDI,0x04;
	LEA       ECX,[ESI+0x01];
	JB       _0070DA4D;

_0070D988:
	MOV       DL,[ECX];
	MOV       [EAX],DL;
	INC       EAX;
	INC       ECX;
	DEC       EDI;
	JNZ      _0070D988;

	JMP      _0070DA09;

_0070D993:
	MOVZX     ESI,BYTE PTR [ECX];
	INC       ECX;
	CMP       ESI,0x10;
	JAE      _0070DA6D;

	TEST      ESI,ESI;
	JNZ      _0070D9C6;

	CMP       BYTE PTR [ECX],0x00;
	JNZ      _0070D9BE;

	LEA       ESP,[ESP+0x00000000];

_0070D9B0:
	MOV       DL,[ECX+0x01];
	ADD       ESI,0x000000FF;
	INC       ECX;
	TEST      DL,DL;
	JZ       _0070D9B0;

_0070D9BE:
	MOVZX     EDX,BYTE PTR [ECX];
	LEA       ESI,[EDX+ESI+0x0F];
	INC       ECX;

_0070D9C6:
	MOV       EDX,[ECX];
	MOV       [EAX],EDX;
	ADD       EAX,0x04;
	ADD       ECX,0x04;
	DEC       ESI;
	JZ       _0070DA09;

	CMP       ESI,0x04;
	JB       _0070DA00;

_0070D9D8:
	MOV       EDX,[ECX];
	MOV       [EAX],EDX;
	SUB       ESI,0x04;
	ADD       EAX,0x04;
	ADD       ECX,0x04;
	CMP       ESI,0x04;
	JAE      _0070D9D8;

	TEST      ESI,ESI;
	JBE      _0070DA09;

	MOV       EDI,EDI;

_0070D9F0:
	MOV       DL,[ECX];
	MOV       [EAX],DL;
	INC       EAX;
	INC       ECX;
	DEC       ESI;
	JNZ      _0070D9F0;

	JMP      _0070DA09;

	JMP      _0070DA00;

	LEA       ECX,[ECX+0x00];

_0070DA00:
	MOV       DL,[ECX];
	MOV       [EAX],DL;
	INC       EAX;
	INC       ECX;
	DEC       ESI;
	JNZ      _0070DA00;

_0070DA09:
	MOVZX     ESI,BYTE PTR [ECX];
	INC       ECX;
	CMP       ESI,0x10;
	JAE      _0070DA6D;

	MOVZX     EDX,BYTE PTR [ECX];
	SHR       ESI,0x02;
	SHL       EDX,0x02;
	MOV       EDI,EAX;
	SUB       EDI,ESI;
	SUB       EDI,EDX;
	MOV       DL,[EDI+0xFFFFF7FF];
	SUB       EDI,0x00000801;
	INC       ECX;
	MOV       [EAX],DL;
	INC       EAX;
	INC       EDI;

_0070DA32:
	MOV       DL,[EDI];
	MOV       [EAX],DL;
	MOV       DL,[EDI+0x01];
	INC       EAX;
	MOV       [EAX],DL;
	INC       EAX;

_0070DA3D:
	XOR       EDX,EDX;
	MOV       DL,[ECX-0x02];
	AND       EDX,0x03;
	MOV       EDI,EDX;
	JZ       _0070D993;

_0070DA4D:
	MOV       DL,[ECX];
	MOV       [EAX],DL;
	INC       EAX;
	INC       ECX;
	CMP       EDI,0x01;
	JBE      _0070DA69;

	MOV       DL,[ECX];
	MOV       [EAX],DL;
	INC       EAX;
	INC       ECX;
	CMP       EDI,0x02;
	JBE      _0070DA69;
	
	MOV       DL,[ECX];
	MOV       [EAX],DL;
	INC       EAX;
	INC       ECX;

_0070DA69:
	MOVZX     ESI,BYTE PTR [ECX];
	INC       ECX;

_0070DA6D:
	CMP       ESI,0x40;
	JB       _0070DAAB;

	MOV       EDX,ESI;
	SHR       EDX,0x02;
	AND       EDX,0x07;
	MOV       EDI,EAX;
	SUB       EDI,EDX;
	MOVZX     EDX,BYTE PTR [ECX];
	SHL       EDX,0x03;
	SUB       EDI,EDX;
	DEC       EDI;
	SHR       ESI,0x05;
	INC       ECX;
	DEC       ESI;

_0070DA8C:
	MOV       DL,[EDI];
	MOV       [EAX],DL;
	MOV       DL,[EDI+0x01];
	INC       EAX;
	INC       EDI;
	MOV       [EAX],DL;
	INC       EAX;
	INC       EDI;
	LEA       ESP,[ESP+0x00000000];

_0070DAA0:
	MOV       DL,[EDI];
	MOV       [EAX],DL;
	INC       EAX;
	INC       EDI;
	DEC       ESI;
	JNZ      _0070DAA0;

	JMP      _0070DA3D;

_0070DAAB:
	CMP       ESI,0x20;
	JB       _0070DAE6;

	AND       ESI,0x1F;
	JNZ      _0070DAD6;

	CMP       BYTE PTR [ECX],0x00;
	JNZ      _0070DACE;

	LEA       EBX,[EBX+0x00000000];

_0070DAC0:
	MOV       DL,[ECX+0x01];
	ADD       ESI,0x000000FF;
	INC       ECX;
	TEST      DL,DL;
	JZ       _0070DAC0;

_0070DACE:
	MOVZX     EDX,BYTE PTR [ECX];
	LEA       ESI,[EDX+ESI+0x1F];
	INC       ECX;

_0070DAD6:
	MOVZX     EDX,WORD PTR [ECX];
	SHR       EDX,0x02;
	MOV       EDI,EAX;
	SUB       EDI,EDX;
	DEC       EDI;
	ADD       ECX,0x02;
	JMP      _0070DB30;

_0070DAE6:
	CMP       ESI,0x10;
	MOV       EDI,EAX;
	JB       _0070DB7E;

	MOV       EDX,ESI;
	AND       EDX,0x08;
	SHL       EDX,0x0B;
	SUB       EDI,EDX;
	AND       ESI,0x07;
	JNZ      _0070DB1B;

	CMP       BYTE PTR [ECX],0x00;
	JNZ      _0070DB13;

_0070DB05:
	MOV       DL,[ECX+0x01];
	ADD       ESI,0x000000FF;
	INC       ECX;
	TEST      DL,DL;
	JZ       _0070DB05;

_0070DB13:
	MOVZX     EDX,BYTE PTR [ECX];
	LEA       ESI,[EDX+ESI+0x07];
	INC       ECX;

_0070DB1B:
	MOVZX     EDX,WORD PTR [ECX];
	SHR       EDX,0x02;
	SUB       EDI,EDX;
	ADD       ECX,0x02;
	CMP       EDI,EAX;
	JZ       _0070DB92;

	SUB       EDI,0x00004000;

_0070DB30:
	CMP       ESI,0x06;
	JB       _0070DA8C;

	MOV       EDX,EAX;
	SUB       EDX,EDI;
	CMP       EDX,0x04;
	JL       _0070DA8C;

	MOV       EDX,[EDI];
	MOV       [EAX],EDX;
	ADD       EAX,0x04;
	ADD       EDI,0x04;
	SUB       ESI,0x02;

_0070DB53:
	MOV       EDX,[EDI];
	MOV       [EAX],EDX;
	SUB       ESI,0x04;
	ADD       EAX,0x04;
	ADD       EDI,0x04;
	CMP       ESI,0x04;
	JAE      _0070DB53;

	TEST      ESI,ESI;
	JBE      _0070DA3D;

	LEA       ECX,[ECX+0x00];

_0070DB70:
	MOV       DL,[EDI];
	MOV       [EAX],DL;
	INC       EAX;
	INC       EDI;
	DEC       ESI;
	JNZ      _0070DB70;

	JMP      _0070DA3D;

_0070DB7E:
	MOVZX     EDX,BYTE PTR [ECX];
	SHR       ESI,0x02;
	SUB       EDI,ESI;
	SHL       EDX,0x02;
	SUB       EDI,EDX;
	DEC       EDI;
	INC       ECX;
	JMP      _0070DA32;

_0070DB92:
	SUB       EAX,[ESP+0x1C];  // mode?
	CMP       ECX,EBP;
	MOV       [EBX],EAX;
	JNZ      _0070DBA3;

	POP       EDI;
	POP       ESI;
	POP       EBP;
	XOR       EAX,EAX;
	POP       EBX;
	RET 0x14;

_0070DBA3:
	POP       EDI;
	SBB       EAX,EAX;
	POP       ESI;
	AND       EAX,-04;
	POP       EBP;
	ADD       EAX,-04;
	POP       EBX;
	RET 0x14;
}
}
