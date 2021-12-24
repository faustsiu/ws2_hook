#include "stdafx.h"

#define MAXMONSTERNUMBER 256
data16 gMonsterID[MAXMONSTERNUMBER];
data32 gMonsterType[MAXMONSTERNUMBER], gMonsterLocX[MAXMONSTERNUMBER], gMonsterLocY[MAXMONSTERNUMBER], gMonsterLocZ[MAXMONSTERNUMBER];
data8 gMonsterAttack[MAXMONSTERNUMBER];

// Monster Action
void Monster_init ()
{
	for (data16 cnt = 0; cnt < MAXMONSTERNUMBER; cnt++)
		gMonsterID[cnt] = 0;
}

void Monster_New (data16 inID, data32 inType, data32 inX, data32 inY, data32 inZ)
{
	data16 cnt;
	short lEmptySpace = -1;

	for (cnt = 0; cnt < MAXMONSTERNUMBER; cnt ++)
	{
		if (gMonsterID[cnt] == inID)
			break;

		if (lEmptySpace < 0x00 && gMonsterID[cnt] == 0x00)
			lEmptySpace = cnt;
	}

	if (cnt == MAXMONSTERNUMBER)
	{
		if (lEmptySpace < 0x00)
			return ;
		else
			cnt = lEmptySpace;
	}

	gMonsterID[cnt] = inID;
	gMonsterType[cnt] = inType;
	gMonsterLocX[cnt] = inX;
	gMonsterLocY[cnt] = inY;
	gMonsterLocZ[cnt] = inZ;
	gMonsterAttack[cnt] = 0x00;
}

void Monster_Erase (data16 iMonsterID)
{
	for (int cnt = 0; cnt < MAXMONSTERNUMBER; cnt ++)
		if (gMonsterID[cnt] == iMonsterID)
		{
			gMonsterID[cnt] = 0;
			return;
		}
}

void Monster_Move (data16 inID, data32 inX, data32 inY, data32 inZ)
{
	for (int cnt = 0; cnt < MAXMONSTERNUMBER; cnt ++)
	{
		if (gMonsterID[cnt] == inID)
		{
			gMonsterLocX[cnt] = inX;
			gMonsterLocY[cnt] = inY;
			gMonsterLocZ[cnt] = inZ;
			if (gMonsterAttack[cnt] == 2)
				gMonsterAttack[cnt] = 0;
			return;
		}
	}

	Monster_New (inID, 0, inX, inY, inZ);
}

void Monster_Attack (data16 inID)
{
	for (int cnt = 0; cnt < MAXMONSTERNUMBER; cnt ++)
		if (gMonsterID[cnt] == inID)
		{
			gMonsterAttack[cnt] = 1;
			return;
		}
}

void Monster_Mission (data16 inID)
{
	for (int cnt = 0; cnt < MAXMONSTERNUMBER; cnt ++)
		if (gMonsterID[cnt] == inID)
		{
			if (gMonsterAttack[cnt] == 0)
				gMonsterAttack[cnt] = 2;
			return;
		}
}

bool Monster_GetAttack (void)
{
	for (int cnt = 0; cnt < MAXMONSTERNUMBER; cnt ++)
		if (gMonsterID[cnt] && gMonsterAttack[cnt] == 1)
			return true;

	return false;
}
