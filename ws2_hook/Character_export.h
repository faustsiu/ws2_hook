extern data32 gCharGlobalID, gCharLoginID;

//extern data32 gCharLocArea;
extern data32 gCharLocX, gCharLocY, gCharLocZ;

//extern data16 gCharFullHP, gCharFullMP, gCharFullSP;
//extern data16 gCharCurrHP, gCharCurrMP, gCharCurrSP;

//extern data16 gStatus, gStatusTimer;

extern data16 gBackLock;

signed long long Character_CurrExp (data32 inExp);
void Character_Location (data32 inCharLocArea, data32 inCharLocX, data32 inCharLocY, data32 inCharLocZ);
void Character_SetFullPP (data16 inHP, data16 inMP, data16 inSP);
void Character_SetCurrPP (data16 inHP, data16 inMP, data16 inSP);
void Character_SetSpeed (data16 inWalk, data16 inRun);
void Character_SetStatus (data16 inStatus);
void Character_HpPlus (data16 inHP);
void Character_HpMinus (data16 inHP);

