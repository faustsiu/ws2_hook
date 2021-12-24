#define MAXMONSTERNUMBER 256
extern data16 gMonsterID[MAXMONSTERNUMBER];
extern data32 gMonsterType[MAXMONSTERNUMBER], gMonsterLocX[MAXMONSTERNUMBER], gMonsterLocY[MAXMONSTERNUMBER], gMonsterLocZ[MAXMONSTERNUMBER];
extern data8 gMonsterAttack[MAXMONSTERNUMBER];

void Monster_init ();
void Monster_New (data16 ID, data32 Type, data32 X, data32 Y, data32 Z);
void Monster_Erase (data16 ID);
void Monster_Move (data16 ID, data32 X, data32 Y, data32 Z);
void Monster_Attack (data16 ID);
void Monster_Mission (data16 inID);
bool Monster_GetAttack (void);
