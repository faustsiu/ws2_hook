#include "stdafx.h"

// Character Information
data32 gCharGlobalID = 0x0017248d, gCharLoginID;

data32 gCharCurExp;

data32 gCharLocX, gCharLocY, gCharLocZ;

data16 gCharFullHP, gCharFullMP, gCharFullSP;
data16 gCharCurrHP, gCharCurrMP, gCharCurrSP;

data16 gStatus, gStatusTimer;
data16 gSpeedWalk, gSpeedRun;

data16 gBackLock = 0;

#define STATUSSPEED	01
#define STATUSCRAZY	02
#define STATUSPOWER	03
#define STATUSEXP	04
#define STATUSLUCKY	06
#define STATUSBOOM	07

#pragma data_seg (".ranhookdata")
data32 gCharLocArea = 0;
#pragma data_seg()

signed long long Character_CurrExp (data32 inExp)
{
	signed long long diff = inExp - gCharCurExp;

	gCharCurExp = inExp;

	return diff;
}

void Character_Location (data32 inCharLocArea, data32 inCharLocX, data32 inCharLocY, data32 inCharLocZ)
{
	gCharLocArea = inCharLocArea;
	if (gCharLocArea == 0)
		gBackLock = 0;
	gCharLocX = inCharLocX;
	gCharLocY = inCharLocY;
	if (inCharLocZ != 0)
		gCharLocZ = inCharLocZ;
}

void Character_SetFullPP (data16 inHP, data16 inMP, data16 inSP)
{
	gCharFullHP = inHP;
	gCharFullMP = inMP;
	gCharFullSP = inSP;
}

void Character_SetCurrPP (data16 inHP, data16 inMP, data16 inSP)
{
	gCharCurrHP = inHP;
	gCharCurrMP = inMP;
	gCharCurrSP = inSP;
}

void Character_SetSpeed (data16 inWalk, data16 inRun)
{
	gSpeedWalk = inWalk;
	gSpeedRun = inRun;
}

void Character_SetStatus (data16 inStatus)
{
	gStatus = inStatus;
	// gStatusTimer
}

void Character_HpPlus (data16 inHP)
{
	gCharCurrHP += inHP;

	if (gCharCurrHP > gCharFullHP)
		gCharCurrHP = gCharFullHP;
}

void Character_HpMinus (data16 inHP)
{
	if (gCharCurrHP > inHP)
		gCharCurrHP -= inHP;
	else
		gCharCurrHP = 0;
}
