extern data32 gtestSkill;
extern data32 Mission5ID;

void Mission_SetStatus (data32 inMissionId, short inStatus);
void Mission_Renew (data32 inMissionId);
void Mission_Finish (data32 inMissionId, data16 inFinishMission);
void Mission_RenewAll (void);
void Mission_Init ();

void MissionEvent ();
void CALLBACK Attack_test (UINT uID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2);
