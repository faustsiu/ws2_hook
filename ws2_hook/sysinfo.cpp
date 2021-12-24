#include "stdafx.h"

#define MESSAGEMAXBUFSIZE 10
char gMessHeader[MESSAGEMAXBUFSIZE][0x22];
char gMessBody[MESSAGEMAXBUFSIZE][0x66];
int gMessageStart = 0;
int gMessageEnd = 0;


int Message_BufferFull ()
{
	int NextMessage = gMessageEnd + 1;
 
	if (NextMessage == MESSAGEMAXBUFSIZE)
		NextMessage = 0;
 
	if (NextMessage == gMessageStart)
		return true;
 
	return false;
}
 
int sysinfo_Message (char *inHeader, char *inMessage)
{
	if (Message_BufferFull () == true)
		return false;
 
	strcpy_s (gMessHeader[gMessageEnd], 0x22, inHeader);
	strcpy_s (gMessBody[gMessageEnd], 0x66, inMessage);

	if (++gMessageEnd == MESSAGEMAXBUFSIZE)
		gMessageEnd = 0;
 
	return true;
}

int sysyinfo_Display (char *Buffer, int inBufLen)
{
	int pos = 0;

	while (gMessageStart != gMessageEnd)
	{
		if (inBufLen < 160)
			break;

		*(data32 *)(&Buffer [pos + 0]) = 0x000000a0;
		*(data32 *)(&Buffer [pos + 4]) = 0x000000aa;
		*(data32 *)(&Buffer [pos + 8]) = 0x00000000;
		*(data32 *)(&Buffer [pos + 12]) = 0x00000094;
		*(data32 *)(&Buffer [pos + 16]) = 0x00000b6c;
		*(data32 *)(&Buffer [pos + 20]) = 0x00000004;

		strcpy_s (&Buffer[pos + 24], 0x22, gMessHeader[gMessageStart]);
		strcpy_s (&Buffer[pos + 58], 0x66, gMessBody[gMessageStart]);

		if (++gMessageStart == MESSAGEMAXBUFSIZE)
			gMessageStart = 0;

		pos += 160;
		inBufLen -=160;
	}

	return pos;

}

//0000   20 00 00 00 aa 00 00 00 00 00 00 00 14 00 00 00
//0010   e4 0b 00 00 f7 01 00 00 00 00 00 00 24 00 00 00
int sysyinfo_AddMessage (char *Buffer, int inBufLen)
{
	data8 command[] = {0x20, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0xe4, 0x0b, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

	memcpy (Buffer, command, sizeof (command));
	return sizeof (command);

	return 0;
}
