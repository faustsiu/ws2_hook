#include "stdafx.h"
#include "decompress_export.h"
#include "Character_export.h"
#include "Inventory_export.h"
#include "Mission_export.h"
#include "RanGameServerData_export.h"


/*
58 00 00 00 
01 09 00 00
00 00 
00 00 
01 00 
00 00 
3e 00 1e 00 
ff ff ff ff
c3 5d 18 48 
00 00 00 00 00 00 00 00 00 00 00 00
91 6a 00 00 00 00 00 00 01 00 05 00 02 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 08 cb d7 2a 00 00 00 00 00 00 00 00
*/

static void RanParser_CommandPacket (data8 *Data, data32 Len)
{
	data32 ProcLen = 0;
    data32 DataLen;


	while (ProcLen < Len)
    {

		DataLen = *((data32 *)&Data[ProcLen]);

        if (DataLen == 00 || ProcLen + DataLen > Len)
            break;

		switch (*((data16 *)&Data[ProcLen + 4]))
        {
			case 0x0900:
				Inventory_Init ();
				Mission_Init ();
				Character_Location (*((data32 *)&Data[ProcLen + 16]), *((data32 *)&Data[ProcLen + 20]), *((data32 *)&Data[ProcLen + 28]), *((data32 *)&Data[ProcLen + 24]));

//				gCharLocArea = (*((long *)&Data[ProcLen + 16]));
//				gCharLocX = (*((data32 *)&Data[ProcLen + 20]));
//				gCharLocY = (*((data32 *)&Data[ProcLen + 28]));
//				gCharLocZ = (*((data32 *)&Data[ProcLen + 24]));
//
//				if (gCharLocArea == 0)
//					gBackLock = 0;
				
				break;

			case 0x0901:
				Inventory_Insert (*((data32 *)&Data[ProcLen + 16]), *((data16 *)&Data[ProcLen + 10]), *((data16 *)&Data[ProcLen + 8]), *((data16 *)&Data[ProcLen + 48]));
				break;

			case 0x0905:
				Mission_SetStatus (*((data32 *)&Data[ProcLen + 16]), *((data16 *)&Data[ProcLen + 0x25]));
				break;

			case 0x090a:
				Mission_RenewAll ();
				break;
				
//			case 0x0b40:
//				gCharGlobalID = (*((data32 *)&Data[ProcLen + 8]));
//				gCharLocX = (*((data32 *)&Data[ProcLen + 16]));
//				gCharLocY = (*((data32 *)&Data[ProcLen + 24]));
//				gCharLocZ = (*((data32 *)&Data[ProcLen + 20]));
				break;

			case 0x0b8b:
				Character_Location (*((data32 *)&Data[ProcLen + 12]), *((data32 *)&Data[ProcLen + 16]), *((data32 *)&Data[ProcLen + 24]), *((data32 *)&Data[ProcLen + 20]));

//				gCharLocArea = (*((long *)&Data[ProcLen + 12]));
//				gCharLocX = (*((data32 *)&Data[ProcLen + 16]));
//				gCharLocY = (*((data32 *)&Data[ProcLen + 24]));
//				gCharLocZ = (*((data32 *)&Data[ProcLen + 20]));
//
//				if (gCharLocArea == 0)
//					gBackLock = 0;

				Mission_RenewAll ();
				break;

			case 0x0c84: // �_А쫆헕
				Character_Location (*((data32 *)&Data[ProcLen + 8]), *((data32 *)&Data[ProcLen + 12]), *((data32 *)&Data[ProcLen + 20]), *((data32 *)&Data[ProcLen + 16]));

//				gCharLocArea = (*((long *)&Data[ProcLen + 8]));
//				gCharLocX = (*((data32 *)&Data[ProcLen + 12]));
//				gCharLocY = (*((data32 *)&Data[ProcLen + 20]));
//				gCharLocZ = (*((data32 *)&Data[ProcLen + 16]));
//
//				if (gCharLocArea == 0)
//					gBackLock = 0;

			case 0x0c9d: // �_А쫆헕
				Character_Location (*((data32 *)&Data[ProcLen + 12]), *((data32 *)&Data[ProcLen + 16]), *((data32 *)&Data[ProcLen + 24]), *((data32 *)&Data[ProcLen + 20]));

//				gCharLocArea = (*((long *)&Data[ProcLen + 12]));
//				gCharLocX = (*((data32 *)&Data[ProcLen + 16]));
//				gCharLocY = (*((data32 *)&Data[ProcLen + 24]));
//				gCharLocZ = (*((data32 *)&Data[ProcLen + 20]));
//
//				if (gCharLocArea == 0)
//					gBackLock = 0;
		}

		ProcLen += DataLen;
	}
}

// Ran Decompress (lzo compression)

static data32 decompressdatalen;
static data8 decompressdata[0x600];
static void RanParser_DataPacket(data8 *Data)
{
	data32 DataLen = *((data32 *)Data);

    if (DataLen == 00 || *((data32 *)&Data[4]) != 0x000000aa)
		return;

	switch (*((data32 *)&Data[8]))
	{
		case 0x00000000: // noncompress command
			RanParser_CommandPacket (&Data[12], DataLen - 12);
			break;

		case 0x00000001: // compress command
			lzo_decompress (&Data[12], DataLen - 12, decompressdata, &decompressdatalen, NULL);
			RanParser_CommandPacket (decompressdata, decompressdatalen);
			break;
	}

	return;
}

static data32 lastbuflen = 0;
static data8 lastbuf[0x300];
void RanParser_RecvHostServerData(data8 *Data, data32 Len)
{
	data32 ProcLen = 0;
	data32 DataLen;

	if (lastbuflen)
	{
		data32 lv_CommandPacketLen = *((data32 *)&lastbuf[0]);
		data32 lv_CopyDataLen = lv_CommandPacketLen - lastbuflen;

		if (Len < lv_CopyDataLen)
			lv_CopyDataLen = Len;
		memcpy (&lastbuf[lastbuflen], Data, lv_CopyDataLen);

		lastbuflen += lv_CopyDataLen;
		ProcLen = lv_CopyDataLen;

		if (lastbuflen == lv_CommandPacketLen)
		{
			RanParser_DataPacket (lastbuf);
			lastbuflen = 0;
		}
	}
	
	while (ProcLen < Len)
	{
	    if (*((data32 *)&Data[ProcLen + 4]) != 0x000000aa)
			return;

		DataLen = *((data32 *)&Data[ProcLen]);
        if (DataLen == 00 || (ProcLen + DataLen) > Len)
			break;
		RanParser_DataPacket (&Data[ProcLen]);
		ProcLen += DataLen;
	}

	if (ProcLen < Len)
	{
		if (DataLen <= sizeof (lastbuf))
		{
			lastbuflen = Len - ProcLen;
			memcpy (lastbuf, &Data[ProcLen], lastbuflen);
		}
		else
			gHook_Status = DataLen;
	}
}
