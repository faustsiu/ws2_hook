#include "stdafx.h"
#include "Character_export.h"
#include "RanGameServerData_export.h"
#include "NetCommand_export.h"

#define MAXROW 13
#define MAXCOL 6

static data32 g_InventoryItem[MAXROW][MAXCOL];
static data16 g_Quantity[MAXROW][MAXCOL];

void Inventory_Init (void)
{
	for (data16 cnt = 0; cnt < MAXROW; cnt++)
		for (data16 cnt1 = 0; cnt1 < MAXCOL; cnt1++)
			g_InventoryItem[cnt][cnt1] = 0;
}

void Inventory_Insert (data32 inItem, data16 inRow, data16 inCol, data16 inQuantity)
{
	g_InventoryItem[inRow][inCol] = inItem;
	g_Quantity[inRow][inCol] = inQuantity;
}

void Inventory_ChangeQuantity (data16 inRow, data16 inCol, data16 inQuantity)
{
	g_Quantity[inRow][inCol] = inQuantity;
}

bool Inventory_Search (data32 inItem, data16 *inRow, data16 *inCol)
{
	for (data16 cnt = 0; cnt < MAXROW; cnt++)
		for (data16 cnt1 = 0; cnt1 < MAXCOL; cnt1++)
			if (g_InventoryItem[cnt][cnt1] == inItem && g_Quantity[cnt][cnt1] > 0)
			{
				*inRow = cnt;
				*inCol = cnt1;
				return true;
			}

	return false;
}

bool Inventory_QuickUseHP (data32 inItem, data16 inValue)
{
	for (int cnt1 = MAXCOL - 1; cnt1 >= 0; cnt1--)
		for (int cnt = MAXROW - 1; cnt >= 0; cnt--)
			if (g_InventoryItem[cnt][cnt1] == inItem)
				for (; g_Quantity[cnt][cnt1] > 0; g_Quantity[cnt][cnt1] --)
				{
					Ran_UseItem ((data16)cnt, (data16)cnt1);
					Character_HpPlus (inValue);

					if (gCharCurrHP >= RegenHPLv)
						return true;
				}

	return false;
}
