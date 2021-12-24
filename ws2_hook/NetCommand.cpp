#include "stdafx.h"
#include "dllmain_export.h"
#include "Character_export.h"
#include "RanGameServerData_export.h"
#include "Monster_export.h"
#include "math.h"

// Action
// iItemType: 3 = Item, 4 = Money
void Ran_PickUp (data32 iItemID, data32 iItemType)
{
	data8 command[16];

	*(data32 *)(&command[0])  = 0x00000010; // Command Length
	*(data32 *)(&command[4])  = 0x00000c34; // Move Command
	*(data32 *)(&command[8])  = iItemType;
	*(data32 *)(&command[12]) = iItemID;

	wf_send (g_hRanGameServer, (const char *)command, 16, 0x00);
}

/*
void Ran_PickUp1 (data16 iItemID)
{
	data8 command[12] = {0x0c, 0x00, 0x00, 0x00, 0x35, 0x0c, 0x00, 0x00, 0xcf, 0x04, 0x00, 0x00};

	*(data16 *)(&command[8]) = iItemID;

	ws2_send (g_hRanGameServer, (const char *)command, 12, 0x00);
}
*/

void Ran_Entry (void)
{
	data8 command[12];
	
	*(data32 *)(&command[0]) =  0x0000000c; // Command KLength
	*(data32 *)(&command[4]) =  0x00000b89; // Move Command
	*(data32 *)(&command[8]) =  gGateID;    // Gate Number
	gGateID = -1;

	ws2_send (g_hRanGameServer, (const char *)command, 12, 0x00);
}

void Ran_Move (data32 inLocX, data32 inLocY, data32 inLocZ)
{
	data8 command[36];

	*(data32 *)(&command[0]) =  0x00000024; // Command KLength
	*(data32 *)(&command[4]) =  0x00000b95; // Move Command
	*(data32 *)(&command[8]) =  0x00000001; // ???
	*(data32 *)(&command[12]) = gCharLocX;
    *(data32 *)(&command[16]) = gCharLocZ;
    *(data32 *)(&command[20]) = gCharLocY;
    *(data32 *)(&command[24]) = inLocX;
    *(data32 *)(&command[28]) = inLocZ;
    *(data32 *)(&command[32]) = inLocY;

	ws2_send (g_hRanGameServer, (const char *)command, 36, 0x00);
}


void Ran_UseItem (data16 inRow, data16 inCol)
{
	data8 command[12];

	*(data32 *)(&command[0]) =  0x0000000c; // Command KLength
	*(data32 *)(&command[4]) =  0x00000c97; // Move Command
	*(data16 *)(&command[8]) = inCol;
	*(data16 *)(&command[10]) = inRow;

//	ws2_send (g_hRanGameServer, (const char *)command, 12, 0x00);
	wf_send (g_hRanGameServer, (const char *)command, 12, 0x00);
}

void Ran_UseCard (data16 inRow, data16 inCol)
{
	data8 command[12];

	*(data32 *)(&command[0]) =  0x0000000c; // Command KLength
	*(data32 *)(&command[4]) =  0x00000c9c; // Move Command
	*(data16 *)(&command[8]) = inCol;
	*(data16 *)(&command[10]) = inRow;

	wf_send (g_hRanGameServer, (const char *)command, 12, 0x00);
}

void Ran_BacktoSchool ()
{
		if (gCharLocArea != 0 && gBackLock == 0)
		{
			Ran_UseCard (0x09, 0x00); // Back to School
			gBackLock = 1;
		}
}


//14 00 00 00 3a 0c 00 00 12 81 d6 c4 88 16 6f c2 3a c2 9d c4
void Ran_Place ()
{
	data8 command[20];

	*(data32 *)(&command[0])  = 0x00000014; // Command KLength
	*(data32 *)(&command[4])  = 0x00000c3a; // Move Command
	*(data32 *)(&command[8])  = 0xc4d68112; // X
	*(data32 *)(&command[12]) = 0xc26f1688; // Z
	*(data32 *)(&command[16]) = 0xc49dc23a; // Y

	ws2_send (g_hRanGameServer, (const char *)command, 20, 0x00);
}

//0000   0c 00 00 00 98 0c 00 00 02 00 02 00
void Ran_Skill ()
{
	data8 command[12];

	*(data32 *)(&command[0])  = 0x0000000c; // Command KLength
	*(data32 *)(&command[4])  = 0x00000c98; // Move Command
	*(data32 *)(&command[8])  = 0x00020002; // X

	ws2_send (g_hRanGameServer, (const char *)command, 12, 0x00);
}



void Ran_Protect (data32 iSkill)
{
	data8 command[42] = {
				0x22 , 0x00 , 0x00 , 0x00 , 
				0xa2 , 0x0c , 0x00 , 0x00 , 
				0x08 , 0x00 , 0x0c , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x01 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 ,
				0x08 , 0x00 , 0x00 , 0x00 ,
				0xa8 , 0x0c , 0x00 , 0x00 };
				
	*(data32 *)(&command[8]) = iSkill;
	*(data32 *)(&command[32]) = gCharLoginID;
	ws2_send (g_hRanGameServer, (const char *)command, 42, 0x00);
}

data16 Ran_Attack (data32 iSkill, data16 iTargetNumber, data16 iDistant, data32 iMonsterType)
{
//	40 Base command + 40 monster command
	data8 command[98] = {
				0x22 , 0x00 , 0x00 , 0x00 , 
				0xa2 , 0x0c , 0x00 , 0x00 , 
				0x08 , 0x00 , 0x0c , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x00 , 0x00 , 0x00 , 0x00 , 
				0x0a , 0x00 , // 10 monster
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
				0x02 , 0x00, 0x27, 0x0a,
	};

	data32 CommandLen;

	data16 lMonsterNumber = 0;
	for (int cnt = 0; cnt < MAXMONSTERNUMBER && lMonsterNumber < iTargetNumber; cnt++)
	{
		if (gMonsterID [cnt] && gMonsterAttack[cnt] == 1)
		{
				*(data16 *)(&command[lMonsterNumber * 4 + 32 ]) = gMonsterID [cnt];
				lMonsterNumber ++;
		}
	}

	for (int cnt = 0; cnt < MAXMONSTERNUMBER && lMonsterNumber < iTargetNumber; cnt++)
	{
		if (gMonsterID [cnt] && (!iMonsterType || iMonsterType == gMonsterType [cnt]) && gMonsterAttack[cnt] == 0)
		{
			float X, Y;
			float d;
			data16 id;
			
			X = *(float *)(&gMonsterLocX[cnt]) - *(float *)(&gCharLocX);
			Y = *(float *)(&gMonsterLocY[cnt]) - *(float *)(&gCharLocY);
			d = sqrt((X * X) + (Y * Y)) + 0.5f;

			id = (data16)d;

			if (id < (iDistant + 50))
			{
				*(data16 *)(&command[lMonsterNumber * 4 + 32 ]) = gMonsterID [cnt];
				lMonsterNumber ++;
			}
		}
	}

	for (int cnt = 0; cnt < MAXMONSTERNUMBER && lMonsterNumber < iTargetNumber; cnt++)
	{
		if (gMonsterID [cnt] && (!iMonsterType || iMonsterType == gMonsterType [cnt]) && gMonsterAttack[cnt] == 0)
		{
			float X, Y;
			float d;
			data16 id;
			
			X = *(float *)(&gMonsterLocX[cnt]) - *(float *)(&gCharLocX);
			Y = *(float *)(&gMonsterLocY[cnt]) - *(float *)(&gCharLocY);
			d = sqrt((X * X) + (Y * Y)) + 0.5f;

			id = (data16)d;

			if (id >= (iDistant + 50))
			{
				*(data16 *)(&command[lMonsterNumber * 4 + 32 ]) = gMonsterID [cnt];
				lMonsterNumber ++;
			}
		}
	}

	if (lMonsterNumber)
	{
		*(data32 *)(&command[8]) = iSkill;
		CommandLen = lMonsterNumber * 4 + 30;
		*(data32 *)(&command[0]) = CommandLen;
		*(data16 *)(&command[28]) = lMonsterNumber;
	
		memcpy (&command[CommandLen], "\x08\x00\x00\x00\xa8\x0c\x00\x00", 8);

		ws2_send (g_hRanGameServer, (const char *)command, CommandLen + 8, 0x00);
		return 1;
	}

	return 0;
}

// 阿，聖門洞      ID 0x00000843, 0x244, 0x14
// 收集煙蒂_聖門   ID 0x0000001e, 00000006, 00000a
// 蒐集繃帶_聖門   ID 0x00000021, 00000006, 00000c
// 護士的請託_聖門 ID 0x0000000f, 00000006, 000007
// 中洞魔狼任      ID 0x0000084a, 0x02c3, 000b(次數)
// 消除隱          ID 0x00000861, 00000100, 0000000a           

void RAN_Mission_Take (data32 inID, data32 inData1, data32 inData2)
{
	data8 command[20];

	*(data32 *)(&command[0])  = 0x00000014; // Command Length
	*(data32 *)(&command[4])  = 0x00000d8d; // Move Command
	*(data32 *)(&command[8])  = inData1; //??
	*(data32 *)(&command[12]) = inData2; // ??
	*(data32 *)(&command[16]) = inID; // ID

	ws2_send (g_hRanGameServer, (const char *)command, 20, 0x00);
}

void RAN_Mission_Finish (data32 inID, data32 inData1, data32 inData2)
{
	data8 command[20];

	*(data32 *)(&command[0])  = 0x00000014; // Command Length
	*(data32 *)(&command[4])  = 0x00000d9c; // Move Command
	*(data32 *)(&command[8])  = inData1; //??
	*(data32 *)(&command[12]) = inData2; // ??
	*(data32 *)(&command[16]) = inID; // ID

	ws2_send (g_hRanGameServer, (const char *)command, 20, 0x00);
}

void RAN_Mission_Release (data32 inMissionId)
{
	data8 command[12];

	*(data32 *)(&command[0])  = 0x0000000c; // Command Length
	*(data32 *)(&command[4])  = 0x00000d92; // Move Command
	*(data32 *)(&command[12]) = inMissionId; // ID

	ws2_send (g_hRanGameServer, (const char *)command, 12, 0x00);
}
