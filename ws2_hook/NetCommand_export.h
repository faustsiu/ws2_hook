void Ran_UseItem (data16 inRow, data16 inCol);
void Ran_UseCard (data16 inRow, data16 inCol);
void Ran_Entry (void);
void Ran_PickUp (data32 iItemID, data32 iItemType);
void Ran_Move (data32 X, data32 Y, data32 Z);
void Ran_UseStartPointPass (void);
void Ran_UseBackPointPass (void);
void Ran_BacktoSchool ();
void Ran_Protect (data32 iSkill);
data16 Ran_Attack (data32 iSkill, data16 iTargetNumber, data16 iDistant, data32 iMonsterType);
void RAN_Mission_Take (data32 inID, data32 inData1, data32 inData2);
void RAN_Mission_Finish (data32 inID, data32 inData1, data32 inData2);
void RAN_Mission_Release (data32 inMissionId);

void Ran_Place (void);
void Ran_Skill ();
