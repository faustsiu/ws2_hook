#include "stdafx.h"
#include "RanGameServerData_export.h"
#include "Inventory_export.h"
#include "NetCommand_export.h"


//IN_004_009	�~�G�B�N�O



// HP items list
//IN_004_000	�ѥ] + 60HP    04 00 00 00
//IN_004_001	HP��_��(�p) +110HP   04 00 01 00
//IN_004_002	HP��_��(��) +230HP   04 00 02 00
//IN_004_007	HP��_��(�j��) +350HP 04 00 07 00
//IN_004_006	���B�N�O +400HP   04 00 06 00
//IN_008_001	��X�^�_��
//IN_008_000	����^�_��
static const data32 HPItem[] = {0x00000004, 0x00010004, 0x00020004, 0x00070004, 0x00060004, 0x00010008, 0x00000008};
static const data16 HPValue[] = {60, 110, 230, 350, 400, 250, 150};
#define MAXHPITEM (sizeof (HPItem) / sizeof (data32))
static data32 HPItemSele = 0;

// MP items list
//IN_005_000	MP��_��(�p)
//IN_005_001	MP��_��(��)
//IN_005_002	MP��_��(�j)
//IN_005_005	MP��_��(�j��)
//IN_005_004	���J�O�B�N�O
//IN_008_000	����^�_��
//IN_008_001	��X�^�_��
static const data32 MPItem[] = {0x00000005, 0x00010005, 0x00020005, 0x00050005, 0x00040005, 0x00000008, 0x00010008};
#define MAXMPITEM (sizeof (MPItem) / sizeof (data32))

// SP items list
//IN_006_000	SP��_��(�p)
//IN_006_001	SP��_��(��)
//IN_006_002	SP��_��(�j)
//IN_006_004	�����B�N�O
//IN_006_005	SP��_��(�j��)
static const data32 SPItem[] = {0x00000006, 0x00010006, 0x00020006, 0x00040006, 0x00050006};
#define MAXSPITEM (sizeof (SPItem) / sizeof (data32))


void AutoRegenPP()
{
	if (gCharCurrHP == 0)
		return;

	if (gCharFullHP > RegenHPLv && gCharCurrHP < RegenHPLv)
	{
		data16 cnt;

		for (cnt = 0; cnt < MAXHPITEM; cnt++)
			if (Inventory_QuickUseHP (HPItem[cnt], HPValue[cnt]) != false)
				break;

		if (cnt == MAXHPITEM)
			Ran_BacktoSchool ();
	}

	if (gCharCurrMP < RegenMPLv)
	{
		for (data16 cnt = 0; cnt < MAXMPITEM; cnt++)
		{
			data16 l_Row, l_Col;

			if (Inventory_Search (MPItem[cnt], &l_Row, &l_Col))
			{
				Ran_UseItem (l_Row, l_Col);
				break;
			}
		}

		gCharCurrMP = RegenMPLv;
	}
		
	if (gCharCurrSP < RegenSPLv)
	{
		for (data16 cnt = 0; cnt < MAXSPITEM; cnt++)
		{
			data16 l_Row, l_Col;

			if (Inventory_Search (SPItem[cnt], &l_Row, &l_Col))
			{
				Ran_UseItem (l_Row, l_Col);
				break;
			}
		}

		gCharCurrSP = RegenSPLv;
	}
}
