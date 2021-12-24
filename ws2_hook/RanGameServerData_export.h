extern SOCKET g_hRanHostServer;
extern SOCKET g_hRanGameServer;

extern data32 gCharGlobalID, gCharLoginID;
extern data32 gCharLocArea;
extern data32 gCharLocX, gCharLocY, gCharLocZ;
extern data16 gCharCurrHP, gCharCurrMP, gCharCurrSP;
extern data16 gCharFullHP, gCharFullMP, gCharFullSP;


extern data8 gAutoPicking;

extern data8 gAutoRegenPP;
extern data16 RegenHPLv, RegenMPLv, RegenSPLv;

extern short gMission;

extern short gGateID;

extern data32 atttest_Delay;
extern data32 atttest_Skill;

extern data32 getsysinfo;

extern data32 gHook_Status;
extern data32 gHook_Status1;
extern data32 gHook_Status2;
extern data32 gHook_Status3;
extern data32 gHook_Status4;

extern data32 gPlace;

void RanParser_SendData(data8 *Data, data32 Len);
void RanParser_RecvGameServerData(data8 *Data, data32 Len);


