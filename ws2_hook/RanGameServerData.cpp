#include "stdafx.h"
#include "stdio.h"
#include "dllmain_export.h"
#include "Mmsystem.h"
#include "sysinfo_export.h"
#include "NetCommand_export.h"
#include "Character_export.h"
#include "PPRegen_export.h"
#include "Monster_export.h"
#include "Pickup.h"
#include "Mission_export.h"
#include "decompress_export.h"
#include "Inventory_export.h"

// Ran Setting
#pragma data_seg (".ranhookdata")
data8 gAutoPicking = 1;

data8 gAutoRegenPP = 1;
data16 RegenHPLv = 1800, RegenMPLv = 10, RegenSPLv = 50;

short gGateID = -1;

short gMission = 0x00;

data32	atttest_Delay = 0;
data32	atttest_Skill = 0;

data32 getsysinfo = 1;

data32	gHook_Status = 0;
data32	gHook_Status1 = 0;
data32	gHook_Status2 = 0;
data32	gHook_Status3 = 0;
data32	gHook_Status4 = 0;

data32 gPlace = 0;
#pragma data_seg()

 
SOCKET g_hRanHostServer = NULL;
SOCKET g_hRanGameServer = NULL;

MMRESULT (WINAPI *ptimeSetEvent)(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD dwUser, UINT fuEvent);    

static void Ran_Action(void)
{
// Auto regen PP
// Entry gate area
// Auto do mission
// Skill test

	if (gAutoRegenPP)
		AutoRegenPP ();

	if (gGateID >= 0x00)
		Ran_Entry ();

	if (gMission <= 0)
		MissionEvent ();

	if (atttest_Skill)
	{
		Ran_Attack (atttest_Skill, 10, 100, 0);
		if (atttest_Delay > 100)
		{
			gtestSkill = atttest_Skill;
			atttest_Delay -= 100;
		}
		ptimeSetEvent (atttest_Delay * 100, 0, Attack_test, 0, TIME_ONESHOT); // wait
		atttest_Skill = 0;
	}

	if (gPlace)
	{
		Ran_Skill ();
		gPlace = 0;
	}
}

// RAN Packet Parser
void RanParser_SendData(data8 *Data, data32 Len)
{
	data32 ProcLen = 0;
    data32 DataLen;

	while (ProcLen < Len)
    {
		DataLen = *((data32*)&Data[ProcLen]);
		if (DataLen == 00 || (ProcLen + DataLen) > Len)
			break;
	
		switch (*((data32*)&Data[ProcLen + 4]))
		{
			case 0x0000091a: // Init Command
				gCharLoginID = *((data16*)&Data[ProcLen + 12]);
				break;

			case 0x00000b95: // Location
				gCharLocZ = *((data32 *)&Data[ProcLen + 16]);
				break;
		}

		ProcLen += DataLen;
	}
}

static void RanParser_CommandPacket (data8 *Data, data32 Len)
{
	data32 ProcLen = 0;
    data32 DataLen;

	while (ProcLen < Len)
    {
		DataLen = *((data32 *)&Data[ProcLen]);
        if (DataLen == 00 || ProcLen + DataLen > Len)
            break;

		short att;

		//						sysinfo_Message ("Pickup");
		switch (*((data16 *)&Data[ProcLen + 4]))
        {
			case 0x0b7f: // 出現物品
				if (gAutoPicking)
					if (PickUp_CheckNoPickUp (*((data32 *)&Data[ProcLen + 0x8])) == false)
						Ran_PickUp (*((data16 *)&Data[ProcLen + 0x14]), 3);
				break;

			case 0x0b80: // 出現金錢
				if (gAutoPicking)
					Ran_PickUp (*((data16 *)&Data[ProcLen + 12]), 4);
				break;

			case 0x0b82: // 出現人物
				if (getsysinfo)
				{
					char buf [0x66];
					sprintf_s (buf, 0x66, "lv:%u hp:%u", *(data16 *)&Data[ProcLen + 0x48], *(data16 *)&Data[ProcLen + 0x86]);
					sysinfo_Message ((char *)&Data[ProcLen + 8], buf);
				}

				if (gMission)
				{
					Ran_BacktoSchool ();
					gAutoPicking = 0;
					gMission = 0;
				}

				break;


			case 0x0b83: // 怪物出現
				if (*((data16 *)&Data[ProcLen + 48]) == 0x1388 && *((data16 *)&Data[ProcLen + 52]) == 0x1388)
					break;				
				if (*((data16 *)&Data[ProcLen + 48]) == 0x0046 && *((data16 *)&Data[ProcLen + 52]) == 0x0028)
					break;

//				if (*((data32 *)&Data[ProcLen + 12]) != 0x0000001c)
					Monster_New (*((data16 *)&Data[ProcLen + 20]), 
						         *((data32 *)&Data[ProcLen + 8]), 
								 *((data32 *)&Data[ProcLen + 24]), 
								 *((data32 *)&Data[ProcLen + 32]), 
								*((data32 *)&Data[ProcLen + 28]));
				break;

			case 0x0b84: // 怪物消失
				Monster_Erase (*((data16 *)&Data[ProcLen + 12]));
				break;

			case 0x0b9c: // 怪物mission
				Monster_Mission (*((data16 *)&Data[ProcLen + 12]));
				break;

			case 0x0b9e: // 怪物攻擊
				att = *(short*)&Data[ProcLen + 0x10];
                break;

			case 0x0ba0: // 怪物死亡
				Monster_Erase (*((data16 *)&Data[ProcLen + 12]));
				break;

            case 0x0ba1:	// PP Data
				Character_SetFullPP (*(data16 *)&Data[ProcLen + 10], *(data16 *)&Data[ProcLen + 14], *(data16 *)&Data[ProcLen + 18]);
				Character_SetCurrPP (*(data16 *)&Data[ProcLen + 8], *(data16 *)&Data[ProcLen + 12], *(data16 *)&Data[ProcLen + 16]);
/*
				gCharCurrHP = *(data16 *)&Data[ProcLen + 8];
                gCharCurrMP = *(data16 *)&Data[ProcLen + 12];
                gCharCurrSP = *(data16 *)&Data[ProcLen + 16];
				gCharFullHP = *(data16 *)&Data[ProcLen + 10];
                gCharFullMP = *(data16 *)&Data[ProcLen + 14];
                gCharFullSP = *(data16 *)&Data[ProcLen + 18];
*/
				break;

            case 0x0ba2:	// PP Data
				{
					char exp[20];
					sprintf (exp, "%lld", Character_CurrExp (*(data32 *)&Data[ProcLen + 8]));
					sysinfo_Message ("得到經驗", exp);
				}
				break;

			case 0x0bcf: // 加物品入背包
				Inventory_Insert (*((data32 *)&Data[ProcLen + 16]), *((data16 *)&Data[ProcLen + 10]), *((data16 *)&Data[ProcLen + 8]), *((data16 *)&Data[ProcLen + 48]));
				break;

			case 0x0bd0: // 刪掉背包物品
				Inventory_Insert (0, *((data16 *)&Data[ProcLen + 10]), *((data16 *)&Data[ProcLen + 8]), 0);
				break;

			case 0x0bd6: // 改變數量
				Inventory_ChangeQuantity (*((data16 *)&Data[ProcLen + 10]), *((data16 *)&Data[ProcLen + 8]), *((data16 *)&Data[ProcLen + 12]));
				break;

			case 0x0bd8: // 加物品數量入背包
				Inventory_Insert (*((data32 *)&Data[ProcLen + 12]), *((data16 *)&Data[ProcLen + 10]), *((data16 *)&Data[ProcLen + 44]), 0);
				break;

//			case 0x0be4: // 得到金錢
//				*((data32 *)&Data[ProcLen + 16] // 金錢
//				break;

			case 0x0c9d: // 轉地圖
				Character_Location (*((data32 *)&Data[ProcLen + 12]), *((data32 *)&Data[ProcLen + 16]), *((data32 *)&Data[ProcLen + 24]), *((data32 *)&Data[ProcLen + 20]));
				break;

			case 0x0ca5: // 攻擊
                if (*(data16*)&Data[ProcLen + 12] == gCharLoginID)
                {
					Character_HpMinus ((data16)((short)0x00 - *(short*)&Data[ProcLen + 0x16]));

//					att = (short)(0x00 - *(short*)&Data[ProcLen + 0x16]);
//					if (gCharCurrHP > (data32)att)
//						gCharCurrHP -= (data32)att;
//					else
//						gCharCurrHP = 0;

					Monster_Attack (*(data16*)&Data[ProcLen + 18]);
                }
                break;

			case 0x0d28: // Set Status
					Character_SetStatus (*(data16*)&Data[ProcLen + 0x0c]);
				break;

			case 0x0d29: // Cancel Status
					Character_SetStatus (0);
				break;

            case 0x0d6a: // 怪物移動
/*
				ProcLen + 8 // Monster ID
				ProcLen + 16 // Monster Org Location X
				ProcLen + 24 // Monster Org Location Y
				ProcLen + 20 // Monster Org Location Z
*/
				Monster_Move (*((data16 *)&Data[ProcLen + 8]), 
							  *((data32 *)&Data[ProcLen + 16]), 
							  *((data32 *)&Data[ProcLen + 24]), 
							  *((data32 *)&Data[ProcLen + 20]));
				break;

			case 0x0d6d: // 怪物攻擊
                if (*(data16*)&Data[ProcLen + 0x10] == gCharLoginID)
                {
					Character_HpMinus (*(data16*)&Data[ProcLen + 0x14]);

//					att = *(data16*)&Data[ProcLen + 0x14];
//					if (gCharCurrHP > (data32)att)
//						gCharCurrHP -= (data32)att;
//					else
//						gCharCurrHP = 0;

					Monster_Attack (*(data16*)&Data[ProcLen + 8]);
				}
				break;


//			case 0x0d93: // Delete Message
//				Mission_SetStatus (*((data32 *)&Data[ProcLen + 8]), 1);
				
//			case 0x0d94: // 增加任務名字
//				Mission_SetStatus (*((data32 *)&Data[ProcLen + 8]), 1);


			case 0x0d95: // Mission Status
				Mission_Renew (*((data32 *)&Data[ProcLen + 16]));
//				Take Mission
//				Mission_SetStatus (*((data32 *)&Data[ProcLen + 10]), 1);
				break;

			case 0x0d96: // 開始第1次
				Mission_SetStatus (*((data32 *)&Data[ProcLen + 16]), *((data16 *)&Data[ProcLen + 0x25]));
				break;

			case 0x0d97: // 開始回報
				Mission_Finish (*((data32*)&Data[ProcLen + 0x08]), *((data16*)&Data[ProcLen + 0x0c]));
				break;

//			case 0x0d9d: // Display Message
//				Mission_Renew (*((data32 *)&Data[ProcLen + 8]));
//				break;

//			case 0x0d9e: // 保護綠地火魔導士完成
//				Mission_Renew (*((data32 *)&Data[ProcLen + 8]));
//				break;

//			case 0x0d9f: // 完成第1次
//				Mission_SetStatus (*((data32 *)&Data[ProcLen + 8]), 2);
//				break;				
				
//			case 0x0da1: // 觀看任務 完成
//				Mission_SetStatus (*((data32 *)&Data[ProcLen + 8]), 1);
//				break;

//			case 0x0da2: // 已接任務
				// *((data32 *)&Data[ProcLen + 8]) Miassion ID
//				break;


			case 0x0e07: // 自己位置
				for (data16 cnt = 0; cnt < *((data16 *)&Data[ProcLen + 8]); cnt++)
				{
					if (*((data32 *)&Data[ProcLen + 12 + (cnt * 12)]) == gCharGlobalID)
					{
						gCharLocX = (*((data32 *)&Data[ProcLen + 16 + (cnt * 12)]));
						gCharLocY = (*((data32 *)&Data[ProcLen + 20 + (cnt * 12)]));
						break;
					}
				}
				break;

/*
case 0x0d97:
				gHook_Status = 0x0d97;
				if (*((data32 *)&Data[ProcLen + 8]) == Mission5ID)
				{
					RAN_Mission_Finish (Mission5ID, 0x02c3, 0x0b);
					Mission5ID = 0;
				}
				break;
*/

			case 0x0ef9: // 自己位置
				Character_Location (*((data32 *)&Data[ProcLen + 8]), *((data32 *)&Data[ProcLen + 12]), *((data32 *)&Data[ProcLen + 20]), *((data32 *)&Data[ProcLen + 16]));

//				gCharLocArea = (*((long *)&Data[ProcLen + 8]));
//				gCharLocX = (*((data32 *)&Data[ProcLen + 12]));
//				gCharLocY = (*((data32 *)&Data[ProcLen + 20]));
//				gCharLocZ = (*((data32 *)&Data[ProcLen + 16]));
//				
//				if (gCharLocArea == 0)
//					gBackLock = 0;
//				
				Monster_init ();
//				sysinfo_Message ("轉地圖", "0x0ef9");
				break;
/*
login mission info
0x0da2
10 00 00 00 a2 0d 00 00 0f 00 00 00 00 00 00 00

10 00 00 00 a2 0d 00 00 1a 00 00 00 00 00 00 00
0b 00 00 00 d0 0c 00 00 00 00 00 09 00 00 00 d1
0c 00 00 00 09 00 00 00 f0 0e 00 00 00 3c 00 00
00 8e 0b 00 00 00 00 00 00 00 00 00 01 0d 00 00
9b 01 00 00 f6 74 b7 f9 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 09 00 00 00 8f 0b 00
00 01 0c 00 00 00 29 0d 00 00 95 1d 00 00 Press
*/



				/*
			case 0x0d94: // 任務開始
				// 8 = Mission ID
				// 
				break;

			case 0x0d96: // 任務開始

			case 0x0d9d: // 完成任務
				// 8 = Mission ID
				// 
				break;
*/
/* 對方HP
0000   1c 00 00 00 aa 00 00 00 00 00 00 00 10 00 00 00
0010   a8 0b 00 00 9b 17 00 00 36 00 36 00
0x0ba8 Command
0x179b ID
0036   HP
*/
/* 對方移動
0000   38 00 00 00 aa 00 00 00 00 00 00 00 2c 00 00 00
0010   96 0b 00 00 9b 17 00 00 15 00 00 00 1f 04 f2 c4
0020   88 16 6f c2 4e bd 4d c4 04 f4 f1 c4 88 16 6f c2
0030   bf df 50 c4 00 00 00 00
0x0b96 Command
0x169b ID
Location
*/		
/*
WIN?
0000   14 00 00 00 aa 00 00 00 00 00 00 00 08 00 00 00
0010   0c 0d 00 00
*/
				/*
			case 0x0130: // ?????

			case 0x0b29: // ?????
			case 0x0b82: // ????
            case 0x0b94: // 對方攻擊
            case 0x0b96: // 對方移動
			case 0x0bd2: // ?????
			case 0x0bd7: // ?????
			case 0x0be4: // ?????
			case 0x0be5: // ?????

			case 0x0c33: // ?????
			case 0x0ca6: // 對方動作

			case 0x0d6b: // 怪物動作
            case 0x0d6c: // 怪物攻擊missing
			case 0x0da2: // ????
			case 0x0dcc: // ????
				break;

			default:
				gHook_Status = *((data16 *)&Data[ProcLen + 4]);
				break;
*/				
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
void RanParser_RecvGameServerData(data8 *Data, data32 Len)
{
	data32 ProcLen = 0;
	data32 DataLen = 0;


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

	Ran_Action();
}
