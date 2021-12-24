#include "stdafx.h"
#include "Mmsystem.h"
#include "dllmain_export.h"
#include "RanGameServerData_export.h"
#include "Monster_export.h"
#include "NetCommand_export.h"
extern MMRESULT (WINAPI *ptimeSetEvent)(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD dwUser, UINT fuEvent);    


//聖門本館1F
//收集煙蒂_聖門 ID 0x0000001e, 0x0006, 0x000a

// 虎令1f 0x0b


// 虎令2f 0x0c


//中洞任 0x001c
// 傷心淚		ID 0x0000086f, 0x0091, 0x0012
// 魔狼的咆哮	ID 0x0000084a, 0x02c3, 0x000b
// 高手授藝

//本洞任 0x001d
// 屎來運轉		ID 0x00000850, 0x01f4, 0x0003
// 保護綠地     ID 0x00000862, 01f1 , 000007







// 異界虎令本館B3 0xf5
// 變異感染 000000876 01a7 000c


// 異界虎令本館B2 0xf6
// 蟲之血 00000878, 0, 04 ?
// 無孔不入 00000879, 0000, 0007

// 異界虎令本館B1 0xf7
// 重度變異 0000087b, 000000d6 00000004 ?
// 復仇之戰 0000087e, 000000d8 00000005 ?
// 蟲蟲危機 0000087c, 000000d7 00000006 ?
// 垃圾危機 0x00087d, 000d7, 000a

// 異界虎令本館 0xf8
// 尋找醫療用品 00000886 0003 0006
// 戰備儲糧 ID 0x00000884, 0x01, 0x19

// 異界虎令操場 0xf9
// 東勤的憤怒  ID 0x00000888 00000001 00000006
// 東勤的無奈  ID 0x00000889 00000001 00000009

// 介 id 0x 0x00000871, 000008, 000002, 000004 回


static const data32 Mission_ID[]    = {0x25, 0x0000001e, 0x0000084a, 0x0000086f, 0x00000850, 0x00000862, 0x00000886, 0x00000888, 0x00000889, 0x0000087b, 0x00000884};
static const data32 Mission_Data1[] = {0x07, 0x00000006, 0x000002c3, 0x00000091, 0x000001f4, 0x000001f1, 0x00000003, 0x00000001, 0x00000001, 0x000000d6, 0x00000001};
static const data32 Mission_Data2[] = {0x0c, 0x0000000a, 0x0000000b, 0x00000011, 0x00000003, 0x00000007, 0x00000006, 0x00000006, 0x00000009, 0x00000004, 0x00000017};
static const data32 Mission_Data3[] = {0x0d, 0x0000000b, 0x0000000c, 0x00000012, 0x00000004, 0x00000008, 0x0000000c, 0x00000007, 0x0000000b, 0x0000000a, 0x00000019};
static const short Mission_TotalMission [] = {1, 1, 1, 2, 1, 2, 1, 4, 4, 6, 1};
static const short Mission_Area[]  =  {0x0000, 0x0000, 0x001c, 0x001c, 0x001d, 0x001d, 0x00f8, 0x00f9, 0x00f9, 0x00f7, 0x00f8};
#define MAXMISSION (sizeof (Mission_ID) / sizeof (data32))
static short Mission_Status[MAXMISSION]; // -1 = NO Mission, 0 = Start, 1 = Finish


void Mission_SetStatus (data32 inMissionId, short inStatus)
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		if (Mission_ID[cnt] == inMissionId)
		{
			Mission_Status[cnt] = inStatus;
			break;
		}
}

void Mission_Renew (data32 inMissionId)
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		if (Mission_ID[cnt] == inMissionId)
		{
			Mission_Status[cnt] = -1;

			if ((Mission_Area[cnt] < 0 || Mission_Area[cnt] == gCharLocArea))
				RAN_Mission_Take (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data2[cnt]);

			break;
		}
}

void Mission_Finish (data32 inMissionId, data16 inFinishMission)
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		if (Mission_ID[cnt] == inMissionId)
		{
			Mission_Status[cnt] = (short)inFinishMission;

			if ((Mission_Area[cnt] < 0 || Mission_Area[cnt] == gCharLocArea) && inFinishMission == Mission_TotalMission[cnt])
				RAN_Mission_Finish (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data3[cnt]);

			break;
		}
}

void Mission_RenewAll (void)
{
// only for mission 0x0000087b
	if (gCharLocArea == 0x00f8 && Mission_Status[8] == 0)
		RAN_Mission_Finish (0x0000087b, 0x00000005, 0x00000001);				
	else if (gCharLocArea == 0x00f7 && Mission_Status[8] == 1)
		RAN_Mission_Finish (0x0000087b, 0x000000d6, 0x00000002);				

	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		if (Mission_Area[cnt] < 0 || Mission_Area[cnt] == gCharLocArea)
			if (Mission_Status[cnt] == -1) // 未接, 需要接任
				RAN_Mission_Take (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data2[cnt]);
			else if (Mission_Status[cnt] == Mission_TotalMission[cnt]) // 完成, 需要回報
				RAN_Mission_Finish (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data3[cnt]);
}

void Mission_Init ()
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		Mission_Status[cnt] = -1; // not start
}




/*


//聖門洞 0x0003
// 阿，聖門洞	ID 0x00000843, 0x244, 0x14

//商洞 0x0010
// 凱特的冰淇淋	ID ?

//中洞任 0x001c
// 魔狼的咆哮	ID 0x0000084a, 0x02c3, 000b
// 高手授藝

//本洞任 0x001d
// 屎來運轉		ID 0x00000850, 0x01f4, 0x0003
// 消除隱患     ID 0x00000861, 00000100, 0000000a   
// 保護綠地     ID 0x00000862, 01f1 , 000007

//死牢任
// 研究用標本	ID ?

//static const data32 Mission_ID[]    = {0x00000843, 0x0000084a, 0x00000850, 0x00000861, 0x00000862, 0x0000001e};
//static const data32 Mission_Data1[] = {0x00000244, 0x000002c3, 0x000001f4, 0x00000100, 0x000001f1, 0x00000006};
//static const data32 Mission_Data2[] = {0x00000014, 0x0000000b, 0x00000003, 0x0000000a, 0x00000007, 0x0000000a};
//static const short Mission_Area[]  =  {0x00000003, 0x0000001c, 0x0000001d, 0x0000001d, 0x0000001d, 0x00000000};
static const data32 Mission_ID[]    = {0x00000850, 0x00000861, 0x00000862};
static const data32 Mission_Data1[] = {0x000001f4, 0x00000100, 0x000001f1};
static const data32 Mission_Data2[] = {0x00000003, 0x0000000a, 0x00000007};
static const short Mission_Area[]  =  {0x001d,     0x001d,     0x001d};
#define MAXMISSION (sizeof (Mission_ID) / sizeof (data32))
static data16 Mission_Status[MAXMISSION];


void Mission_SetStatus (data32 inMissionId, data32 inStatus)
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		if (Mission_ID[cnt] == inMissionId)
		{
			Mission_Status[cnt] = inStatus;
			break;
		}
}

void Mission_Renew (data32 inMissionId)
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
		if (Mission_ID[cnt] == inMissionId)
		{
			if ((Mission_Area[cnt] < 0 || Mission_Area[cnt] == gCharLocArea))
				RAN_Mission_Take (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data2[cnt]);
			break;
		}
}

void Mission_RenewAll (void)
{
	for (int cnt = 0; cnt < MAXMISSION; cnt ++)
	{
		if (Mission_Area[cnt] <= 0 || Mission_Area[cnt] == gCharLocArea)
		{
			if (Mission_Status[cnt] == 10)
			{
				RAN_Mission_Take (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data2[cnt]);
				Mission_Status[cnt] = 0;
			}

			if (Mission_Status[cnt] == 0)
				RAN_Mission_Take (Mission_ID[cnt], Mission_Data1[cnt], Mission_Data2[cnt]);
		}
	}
}

*/





data16 gState;











// AutoAttack 
// for General;

//SN_010_001	血箭 2 min (120) 29
//SN_008_003	迴避驟升2 min (120) 10

//SN_008_009	精靈之舞 45 7.0 10 7
//SN_008_013	炎流之鷹 85 7.2 10 15
//SN_008_006	流星隕擊 75 5.7 10 12
//SN_008_007	穴道制壓 46 7.0 9 15
//SN_008_014	水晶狂嵐 85 7.3 10 21
//SN_009_005	固定射擊 65 8.1 10 27
//SN_008_012	冰封流星擊 85 9.2 4 17

//SN_010_008	哀嘆之韻 75 6.2 11 
//SN_010_007	暴風射擊 75 6.2 11
//SN_010_004	波動蓮華 70 6.2 11
//SN_008_007	穴道制壓 46 7.0 9  15
//SN_009_005	固定射擊 65 8.1 10 27

// 測試
//const data32 SkillID[]			= {0x00030008, 0x0001000a, 0x000d0008, 0x00060008, 0x00090008, 0x00070008, 0x000e0008, 0x00050009, 0x000c0008};
//const data16 SkillRefresh[]		= {1200,       1200,       72,         57,         70,         70,         73,         81,          92};
//const data16 SkillDelay[]       = {29,         10,         15,         12,         7,          15,         21,         27,          17};
//const data16 SkillDistant[]		= {0,          0,          85,         75,         45,         46,         85,         65,          85};
//const data16 SkillTargetNumber[]= {0,          0,          10,         10,         10,         9,          10,         10,          4};

// 敏弓
//const data32 SkillID[]			= {0x00030008, 0x0001000a, 0x00090008, 0x000d0008, 0x00060008, 0x00070008, 0x000e0008, 0x00050009, 0x000c0008};
//const data16 SkillRefresh[]		= {1200,       1200,       70,         72,         57,         70,         73,         81,          92};
//const data16 SkillDelay[]      	= {29,         10,         7,          15,         12,         15,         21,         27,          17};
//const data16 SkillDistant[]		= {0,          0,          45,         85,         75,         46,         85,         65,          85};
//const data16 SkillTargetNumber[]= {0,          0,          10,         10,         10,         9,          10,         10,          4};

// 精敏力弓
const data32 SkillID[]			= {0x00030008, 0x0001000a, 0x0008000a, 0x0007000a, 0x0004000a, 0x00070008, 0x00050009};
const data16 SkillRefresh[]		= {1200,       1200,       62,         62,         62,         70,         81};
const data16 SkillDelay[]      	= {29,         10,         9,          18,         15,         15,         27};
const data16 SkillDistant[]		= {0,          0,          75,         75,         70,         46,         65};
const data16 SkillTargetNumber[]= {0,          0,          11,         11,         11,         9,          10};


#define MAXSkill (sizeof (SkillID) / sizeof (data32))
data16 SkillTimer[MAXSkill];

data32 gtestSkill = 0;
void CALLBACK Attack_test (UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	if (gtestSkill)
	{
		Ran_Attack (gtestSkill, 10, 100, 0);
		gtestSkill = 0;
	}
	else
		Ran_Protect (SkillID[1]);
}


static data32 gCircle_X, gCircle_Y;
static data16 gCircle_gStep;

void Mission_Circle_Init (data16 inStart)
{
	gCircle_X = gCharLocX;
	gCircle_Y = gCharLocY;
	gCircle_gStep = inStart;
}


data16 Mission_Round1 ()
{
	data16 delay = 20;
	float tmp;

	if (gCircle_gStep == 0)
	{
		gCircle_X = 0xc4826400;
		gCircle_Y = 0x45b08a60;
		data16 delay = 50;
	} // UP
	else if ((gCircle_gStep >= 34 &&
			gCircle_gStep <= 39) || 
			gCircle_gStep == 44)
	{
		tmp = *(float *)&gCircle_Y + 20.0f;
		gCircle_Y = *(data32 *)&tmp;
		data16 delay = 1;
	}
	else if (gCircle_gStep >= 26 &&
			gCircle_gStep <= 33)
	{
		tmp = *(float *)&gCircle_Y + 20.0f;
		gCircle_Y = *(data32 *)&tmp;
		tmp = *(float *)&gCircle_X + 20.0f;
		gCircle_X = *(data32 *)&tmp;
		data16 delay = 2;
	} // Right
	else if (gCircle_gStep == 1 ||
			gCircle_gStep == 2)
	{
		tmp = *(float *)&gCircle_X + 20.0f;
		gCircle_X = *(data32 *)&tmp;
		data16 delay = 1;
	}
	else if (gCircle_gStep >= 3 &&
			gCircle_gStep <= 4)
	{
		tmp = *(float *)&gCircle_Y - 20.0f;
		gCircle_Y = *(data32 *)&tmp;
		tmp = *(float *)&gCircle_X + 20.0f;
		gCircle_X = *(data32 *)&tmp;
		data16 delay = 2;
	} // Down
	else if (gCircle_gStep >= 5 &&
			gCircle_gStep <= 25)
	{
		tmp = *(float *)&gCircle_Y - 20.0f;
		gCircle_Y = *(data32 *)&tmp;
		data16 delay = 1;
	}
	else if (gCircle_gStep >= 40 &&
			gCircle_gStep <= 43)
	{
		tmp = *(float *)&gCircle_Y + 20.0f;
		gCircle_Y = *(data32 *)&tmp;
		tmp = *(float *)&gCircle_X - 20.0f;
		gCircle_X = *(data32 *)&tmp;
		data16 delay = 2;
	}

	Ran_Move (gCircle_X, gCircle_Y, gCharLocZ);
	gCharLocX = gCircle_X;
	gCharLocY = gCircle_Y;

	if (++gCircle_gStep == 45)
		gCircle_gStep = 0;

	return delay;
}

data16 Mission_Circle40 ()
{
	float tmp;
	data16 delay = 5;

	switch (gCircle_gStep)
	{
		case 0: // UP
			tmp = *(float *)&gCircle_Y + 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 1: // UP
			tmp = *(float *)&gCircle_Y + 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 2: // Right
			tmp = *(float *)&gCircle_Y + 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 3: // Right
			tmp = *(float *)&gCircle_Y + 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 4: // down
			tmp = *(float *)&gCircle_Y + 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 5: // down
			tmp = *(float *)&gCircle_Y + 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 6: // Left
			tmp = *(float *)&gCircle_X + 20.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 7: // Left
			tmp = *(float *)&gCircle_X + 20.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 8: // Left
			tmp = *(float *)&gCircle_Y - 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 9: // Left
			tmp = *(float *)&gCircle_Y - 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 10: // Left
			tmp = *(float *)&gCircle_Y - 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 11: // Left
			tmp = *(float *)&gCircle_Y - 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 12: // Left
			tmp = *(float *)&gCircle_Y - 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 13: // Left
			tmp = *(float *)&gCircle_Y - 20.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 20.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 7;
			break;
		case 14: // Left
			tmp = *(float *)&gCircle_X - 20.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 15: // Left
			tmp = *(float *)&gCircle_X - 20.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
	}

	Ran_Move (gCircle_X, gCircle_Y, gCharLocZ);
	gCharLocX = gCircle_X;
	gCharLocY = gCircle_Y;

	if (++gCircle_gStep == 16)
		gCircle_gStep = 0;

	return delay; //delay 0.5s
}

// 財團1F
data16 Mission_Circle2 ()
{
	float tmp;
	data16 delay = 7;

    if (gCircle_gStep == 0 || gCircle_gStep == 79)
	{
		tmp = *(float *)&gCircle_X - 242.0f;
		gCircle_X = *(data32 *)&tmp;
		delay = 58;
	}else if (gCircle_gStep == 39 || gCircle_gStep == 40)
	{
		tmp = *(float *)&gCircle_X + 242.0f;
		gCircle_X = *(data32 *)&tmp;
		delay =  58;
	}
	else if ((gCircle_gStep >=1 && gCircle_gStep <= 16) || (gCircle_gStep >=64 && gCircle_gStep <= 78))
	{
		tmp = *(float *)&gCircle_X - 21.0f;
		gCircle_X = *(data32 *)&tmp;
		delay = 5;
	}
	else if ((gCircle_gStep >=24 && gCircle_gStep <= 38) || (gCircle_gStep >=41 && gCircle_gStep <= 56))
	{
		tmp = *(float *)&gCircle_X + 21.0f;
		gCircle_X = *(data32 *)&tmp;
		delay = 5;
	}
	else 
	{
		switch (gCircle_gStep)
		{
			case 59:
			case 17:
				tmp = *(float *)&gCircle_Y + 29.7f;
				gCircle_Y = *(data32 *)&tmp;
				tmp = *(float *)&gCircle_X - 29.7f;
				gCircle_X = *(data32 *)&tmp;
				delay = 10;
				break;

			case 58:
			case 18:
				tmp = *(float *)&gCircle_Y + 21.0f;
				gCircle_Y = *(data32 *)&tmp;
				delay = 5;
				break;

			case 57:
			case 19:
				tmp = *(float *)&gCircle_Y + 29.7f;
				gCircle_Y = *(data32 *)&tmp;
				tmp = *(float *)&gCircle_X + 29.7f;
				gCircle_X = *(data32 *)&tmp;
				delay = 10;
				break;
			case 20:
				tmp = *(float *)&gCircle_X + 21.0f;
				gCircle_X = *(data32 *)&tmp;
				delay = 5;
				break;
			case 60:
				tmp = *(float *)&gCircle_X - 21.0f;
				gCircle_X = *(data32 *)&tmp;
				delay = 5;
				break;
			case 63:
			case 21:
				tmp = *(float *)&gCircle_Y - 29.7f;
				gCircle_Y = *(data32 *)&tmp;
				tmp = *(float *)&gCircle_X + 29.7f;
				gCircle_X = *(data32 *)&tmp;
				delay = 10;
				break;
			case 62:
			case 22:
				tmp = *(float *)&gCircle_Y - 21.0f;
				gCircle_Y = *(data32 *)&tmp;
				delay = 5;
				break;
			case 61:
			case 23:
				tmp = *(float *)&gCircle_Y - 29.7f;
				gCircle_Y = *(data32 *)&tmp;
				tmp = *(float *)&gCircle_X - 29.7f;
				gCircle_X = *(data32 *)&tmp;
				delay = 10;
				break;
		}
	}

	Ran_Move (gCircle_X, gCircle_Y, gCharLocZ);
	gCharLocX = gCircle_X;
	gCharLocY = gCircle_Y;

	if (++gCircle_gStep == 80)
		gCircle_gStep = 0;

	return delay;
}

data16 Mission_Circle1 ()
{
	float tmp;
	data16 delay = 5;

	switch (gCircle_gStep)
	{
		case 0: // UP
			tmp = *(float *)&gCircle_Y + 30.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 30.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 10;
			break;
		case 1: // Right
			tmp = *(float *)&gCircle_Y + 21.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 2: // down
			tmp = *(float *)&gCircle_Y + 30.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 30.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 10;
			break;
		case 3: // Left
			tmp = *(float *)&gCircle_X + 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 4: // Left
			tmp = *(float *)&gCircle_X + 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 5: // Left
			tmp = *(float *)&gCircle_X + 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 6: // Left
			tmp = *(float *)&gCircle_X + 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 7: // Left
			tmp = *(float *)&gCircle_X + 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 8: // Left
			tmp = *(float *)&gCircle_Y - 30.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 30.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 10;
			break;
		case 9: // Left
			tmp = *(float *)&gCircle_Y - 21.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 10: // Left
			tmp = *(float *)&gCircle_Y - 30.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 30.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 10;
			break;
		case 11: // Left
			tmp = *(float *)&gCircle_X - 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 12: // Left
			tmp = *(float *)&gCircle_X - 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 13: // Left
			tmp = *(float *)&gCircle_X - 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 14: // Left
			tmp = *(float *)&gCircle_X - 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 15: // Left
			tmp = *(float *)&gCircle_X - 21.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
	}

	Ran_Move (gCircle_X, gCircle_Y, gCharLocZ);
	gCharLocX = gCircle_X;
	gCharLocY = gCircle_Y;

	if (++gCircle_gStep == 16)
		gCircle_gStep = 0;

	return delay;
}

data16 Mission_Circle ()
{
	float tmp;
	data16 delay = 10;

	switch (gCircle_gStep)
	{
		case 0: // UP
			tmp = *(float *)&gCircle_Y + 40.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 40.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 14;
			break;
		case 1: // Right
			tmp = *(float *)&gCircle_Y + 40.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 2: // down
			tmp = *(float *)&gCircle_Y + 40.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 40.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 14;
			break;
		case 3: // Left
			tmp = *(float *)&gCircle_X + 40.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
		case 4: // Left
			tmp = *(float *)&gCircle_Y - 40.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X + 40.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 14;
			break;
		case 5: // Left
			tmp = *(float *)&gCircle_Y - 40.0f;
			gCircle_Y = *(data32 *)&tmp;
			break;
		case 6: // Left
			tmp = *(float *)&gCircle_Y - 40.0f;
			gCircle_Y = *(data32 *)&tmp;
			tmp = *(float *)&gCircle_X - 40.0f;
			gCircle_X = *(data32 *)&tmp;
			delay = 14;
			break;
		case 7: // Left
			tmp = *(float *)&gCircle_X - 40.0f;
			gCircle_X = *(data32 *)&tmp;
			break;
	}

	Ran_Move (gCircle_X, gCircle_Y, gCharLocZ);
	gCharLocX = gCircle_X;
	gCharLocY = gCircle_Y;

	if (++gCircle_gStep == 8)
		gCircle_gStep = 0;

	return delay; //delay 0.5s
}

void Mission_Attack_Init (void)
{
	for (data16 cnt = 0; cnt < MAXSkill; cnt++)
		SkillTimer[cnt] = 0;
}

data16 Mission_Attack (data32 inMonsterType)
{
	data16 DelayTime = 1;

	for (data16 cnt = 0; cnt < MAXSkill; cnt++)
	{
		if (cnt == 2 && SkillTimer[6] == 0 && Monster_GetAttack() == false)
			cnt = 6;

		if (SkillTimer[cnt] == 00)
		{

			if (SkillDistant[cnt])
			{
				if (Ran_Attack (SkillID[cnt], SkillTargetNumber[cnt], SkillDistant[cnt], inMonsterType) == 0)
					return 1;
			}
			else
				Ran_Protect (SkillID[cnt]);

			DelayTime = SkillDelay[cnt];
			SkillTimer[cnt] = SkillRefresh[cnt];
			break;
		}
	}

	return (DelayTime);
}

data32 Mission5ID = 0;
data16 Mission6 (void)
{
	if (gState == 0)
	{
		Mission_Attack_Init ();
		gState = 1;
	}
		
	if (Mission5ID == 0)
	{
		Mission5ID = 0x0000084a;
		RAN_Mission_Take (Mission5ID, 0x02c3, 0x0b);
		return 1;
	}

	return Mission_Attack (0x00040028);
}



data16 Mission4 (void)
{
	return 0;
}

// 走打
data16 Mission3 (void)
{
	if (gState == 0)
	{
		Mission_Circle_Init (0);
		Mission_Attack_Init ();
	}

	if (++gState == 3)
		gState = 1;

	if (gState & 0x01)
		return Mission_Attack (0);

	return Mission_Circle1 ();
}

// 原地打怪
data16 Mission2 (void)
{
	if (gState == 0)
	{
		Mission_Attack_Init ();
		gState = 1;
	}
		
	return Mission_Attack (0);
}

// 四方行
data16 Mission1 (void)
{
	if (gState == 0)
	{
		Mission_Circle_Init (0);
		gState = 1;
	}

	return Mission_Circle ();
}


// 屎來運轉
//CN_040_022	光魔導士

//CN_040_024	冰魔導士


// 保護緣地
//CN_040_022	光魔導士
//CN_040_021	火魔導士

// 走打
data16 Mission11 (void)
{
	if (gState == 0)
	{
		Mission_Circle_Init (0);
		Mission_Attack_Init ();
	}

	if (++gState == 3)
		gState = 1;

	if (gState & 0x01)
		return Mission_Attack (0x00160028);//CN_040_022	光魔導士

	return Mission_Circle ();
}

data16 Mission12 (void)
{
	if (gState == 0)
	{
		Mission_Circle_Init (0);
		Mission_Attack_Init ();
	}

	if (++gState == 3)
		gState = 1;

	if (gState & 0x01)
		return Mission_Attack (0x00150028);//CN_040_021	火魔導士

	return Mission_Circle ();
}

// CN_040_004 狂冷魔狼
// 魔狼的咆哮      ID 0x0000084a, 0x02c3, 000b
data16 Mission_0x0000084a (void)
{
	if (gState == 0)
	{
		Mission_Circle_Init (0);
		Mission_Attack_Init ();
	}

	if (gState == 1)
	{
		gState++;
		return Mission_Round1 ();
	}

	gState = 1;
	return Mission_Attack (0x00040028);
}

static void CALLBACK MissionProc (UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	data16 delay;

	if (gMission == 0)
		return ;

	switch (gMission)
	{
		case 1:
			delay = Mission1 ();
			break;
		case 2:
			delay = Mission2 ();
			break;
		case 3:
			delay = Mission3 ();
			break;
		case 4:
			delay = Mission4 ();
			break;
		case 5:
//			delay = Mission5 ();
			break;
		case 6:
			delay = Mission6 ();
			break;
		case 11:
			delay = Mission11 ();
			break;
		case 12:
			delay = Mission12 ();
			break;
		default:
			delay = 1;
	}

	ptimeSetEvent (delay * 100, 0, MissionProc, 0, TIME_ONESHOT);

	for (data16 cnt = 0; cnt < MAXSkill; cnt++)
		if (SkillTimer[cnt])
		{
			if (SkillTimer[cnt] > delay)
				SkillTimer[cnt] -= delay;
			else
				SkillTimer[cnt] = 0;
		}
}

void MissionEvent ()
{
	gState = 0;
	ptimeSetEvent (1, 0, MissionProc, 0, TIME_ONESHOT);
	gMission &= 0x7fff;
}
