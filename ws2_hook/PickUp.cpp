#include "stdafx.h"
#include "RanGameServerData_export.h"

// Items Data

//0x003B0009 Bomb 9999HP Box
//0x00070009 Bomb 300HP Box
//0x00080009 Bomb 200HP Box
//0x00090009 Bomb 100HP Box
//0x00270009 Speed Up Box
//0x00280009 Crazy Time Box
//0x002a0009 Lucky Box
//0x00290009 Power UP
//0x00040009 ©R¹B½c¤l
static const data32 NoPickItem [] = {0x003B0009, 0x00070009, 0x00080009, 0x00090009, 0x00270009, 0x00280009, 0x002a0009, 0x00040009};
#define MAXNOPICKITEM (sizeof (NoPickItem) / sizeof (data32))

//IN_005_000	MP«ì´_¾¯(¤p)
//IN_005_001	MP«ì´_¾¯(¤¤)
//IN_005_002	MP«ì´_¾¯(¤j)
//IN_005_005	MP«ì´_¾¯(±j¤Æ)
//IN_005_004	¥©§J¤O¦B²N²O
//IN_006_000	SP«ì´_¾¯(¤p)
//IN_006_001	SP«ì´_¾¯(¤¤)
//IN_006_002	SP«ì´_¾¯(¤j)
//IN_006_004	§ö¤¯¦B²N²O
//IN_006_005	SP«ì´_¾¯(±j¤Æ)
static const data32 NoPPItem[] = {0x00000005, 0x00010005, 0x00020005, 0x00050005, 0x00040005, 0x00000006, 0x00010006, 0x00020006, 0x00040006, 0x00050006};
#define MAXNOPPITEM (sizeof (NoPPItem) / sizeof (data32))

//
// Don't Pick up items
static const data16 NoItem[] = {4, 5, 6, 8, 19, 23, 29, 32, 38, 41, 44, 50, 53, 56, 59, 60, 61, 62, 85, 86, 87, 88, 89, 90, 91, 92, 71, 72, 73, 80, 155, 9, 111, 13, 14, 74, 150};
#define MAXNOITEM (sizeof (NoItem) / sizeof (data16))

static const data16 NoSItem[] = {150, 0, 3, 27, 28, 30, 31, 117, 127, 63};
static const data16 NoSItemS[] = {126, 0, 0, 0, 0, 0, 0, 0, 217, 19};
static const data16 NoSItemE[] = {147, 9, 8, 7, 10, 3, 3, 4, 217, 22};
#define MAXNOSITEM (sizeof (NoSItem) / sizeof (data16))


bool PickUp_CheckNoPickUp (data32 inType)
{
	switch (gAutoPicking)
	{
		case 5:
			if ((data16)(inType & 0xffff) == 131)
				return false;
			else if ((data16)(inType & 0xffff) == 150)
			{
				data16 itemnumber = (data16)(inType >> 16);
					
				if (itemnumber >= 1 && itemnumber <= 22)
					return false;

				if (itemnumber >= 78 && itemnumber <= 93)
					return false;
			}
			else if ((data16)(inType & 0xffff) == 142)
			{
				data16 itemnumber = (data16)(inType >> 16);
					
				if (itemnumber >= 18 && itemnumber <= 49)
					return false;
			}
			else if ((data16)(inType & 0xffff) == 153)
			{
				data16 itemnumber = (data16)(inType >> 16);
					
				if (itemnumber >= 111 && itemnumber <= 123)
					return false;
			}

			return true;
		
/*
IN_011_006	°ª¯ÅÅ@¨©¾¯
IN_011_014	²§¬É¤ô´¹

IN_111_035	²×·¥¸É¥R¾¯(100²~¸Ë)

IN_133_006	±jªZ¶uª÷
IN_133_010	±jªZ°©¯»

IN_150_164	°ª¯ÅÅ@¨©¾¯
IN_150_168	²§¬É¤ô´¹
IN_150_173	±jªZ¶uª÷
IN_150_175	±jªZ°©¯»

IN_156_033	²×·¥¸É¥R¾¯(100²~¸Ë)
IN_156_034	°ª¯ÅÅ@¨©¾¯
IN_156_038	²§¬É¤ô´¹
IN_156_043	±jªZ¶uª÷
IN_156_045	±jªZ°©¯»
		case 4: // For 4/17
            switch (inType)
			{
			case 0x0006000b:
			case 0x000e000b:

			case 0x0023006f:

			case 0x00060085:
			case 0x000a0085:

			case 0x00a40096:
			case 0x00a80096:
			case 0x00ad0096:
			case 0x00af0096:

			case 0x0021009c:
			case 0x0022009c:
			case 0x0026009c:
			case 0x002b009c:
			case 0x002d009c:
				return true;
			}
*/


		case 4: // MAX Only
			if (inType != 0x001b006f)
				return true;
			return false;


		case 3:

            if (inType == 0x00290009) // POWER
				return true;
			for (data16 cnt = 0; cnt < MAXNOITEM; cnt++)
				if ((data16)(inType & 0xffff) == NoItem[cnt])
					return true;
			for (data16 cnt = 0; cnt < MAXNOSITEM; cnt++)
				if ((data16)(inType & 0xffff) == NoSItem[cnt])
				{
					data16 itemnumber = (data16)(inType >> 16);
					
					if (itemnumber >= NoSItemS[cnt] && itemnumber <= NoSItemE[cnt])
						return true;
				}

		case 2:
			for (data16 cnt = 0; cnt < MAXNOPPITEM; cnt++)
				if (inType == NoPPItem[cnt])
					return true;
		case 1:
			for (data16 cnt = 0; cnt < MAXNOPICKITEM; cnt++)
				if (inType == NoPickItem[cnt])
					return true;
	}

	return false;
}
