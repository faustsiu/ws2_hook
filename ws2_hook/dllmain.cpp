// dllmain.cpp : 定義 DLL 應用程式的進入點。
#include "stdafx.h"
#include "RanHostServerData_export.h"
#include "RanGameServerData_export.h"
#include "Mmsystem.h"
extern MMRESULT (WINAPI *ptimeSetEvent)(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD dwUser, UINT fuEvent);    

//#include "NetCommand_export.h"
//#include "Mission_export.h"


#pragma pack(1)

// Hook 
#define HOOK_CONNECT 0x00
#define HOOK_SEND 0x01
#define HOOK_RECV 0x02
#define MAXHOOKFUNC 0x03
HMODULE hWS2_Win = 0;
HMODULE hTimer_Win = 0;

typedef int (CALLBACK* LPFNDLLFUNCCONNECT)(SOCKET s, const struct sockaddr *name, int namelen);
typedef int (CALLBACK* LPFNDLLFUNCSEND)(SOCKET s, const char *buf, int len, int flags);
typedef int (CALLBACK* LPFNDLLFUNCRECV)(SOCKET s, char *buf, int len, int flags);

LPFNDLLFUNCCONNECT ws2_connect;
LPFNDLLFUNCSEND ws2_send;
LPFNDLLFUNCRECV ws2_recv;


#define MAXBUF 1460

static SOCKET g_wf_s;
static char g_wf_buf[MAXBUF];
static int g_wf_len = 0;
static int g_wf_flags;

void wf_flush (void)
{
	if (g_wf_len)
	{
		ws2_send (g_wf_s, g_wf_buf, g_wf_len, g_wf_flags);
		g_wf_len = 0;
	}
}

int wf_send (SOCKET s, const char *buf, int len, int flags)
{
	static SOCKET old_s = NULL;

	if (g_wf_s != s || g_wf_flags != flags)
	{
		wf_flush ();
		g_wf_s = s;
		g_wf_flags = flags;
	}
	else if (g_wf_len + len > MAXBUF)
		wf_flush ();

	memcpy (&g_wf_buf[g_wf_len], buf, len);
	g_wf_len += len;

	return len;
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:

		g_hRanHostServer = NULL;
		g_hRanGameServer = NULL;

		hWS2_Win = LoadLibrary("ws2_2008.dll");
		if (!hWS2_Win) return false;

		hTimer_Win = LoadLibrary("winmm.dll");
		if (!hTimer_Win) return false;

		ws2_connect = (LPFNDLLFUNCCONNECT)GetProcAddress (hWS2_Win, "connect"); 
		ws2_send = (LPFNDLLFUNCSEND)GetProcAddress (hWS2_Win, "send"); 
		ws2_recv = (LPFNDLLFUNCRECV)GetProcAddress (hWS2_Win, "recv");

		ptimeSetEvent = (MMRESULT (WINAPI *)(UINT uDelay, UINT uResolution, LPTIMECALLBACK fptc, DWORD dwUser, UINT fuEvent))GetProcAddress(hTimer_Win, "timeSetEvent");    

		break;

	case DLL_PROCESS_DETACH:
		FreeLibrary(hWS2_Win);
		FreeLibrary(hTimer_Win);
		break;
	}

	return TRUE;
}

// Export function
extern "C" unsigned long __stdcall  __GetHookerStatus (data32 inRegister)
{
switch (inRegister)
	{
		case 0:
			return gCharLocArea;
		case 1:
			return gHook_Status1;
		case 2:
			return gHook_Status2;
		case 3:
			return gHook_Status3;
		case 4:
			return gHook_Status4;
		case 5:
			return gHook_Status;
/*
case 6:
			return gHook_Status6;
*/
}
	return 0;
}

extern "C" void __stdcall  __AutoRegenPP (data16 inEnable, data16 inHPLimit, data16 inMPLimit, data16 inSPLimit)
{
	gAutoRegenPP = inEnable;
	RegenHPLv = inHPLimit;
	RegenMPLv = inMPLimit;
	RegenSPLv = inSPLimit;
}

extern "C" void __stdcall  __AutoPicking (data16 inEnable) 
{
	gAutoPicking = inEnable;
}

extern "C" void __stdcall  __EntryArea (data16 inGateNumber)
{
	gGateID = inGateNumber;
}

extern "C" void __stdcall  __AutoMission (data16 inMission) 
{
	if (inMission == 9999)
	{
		gPlace = 1;
	}
	else
	{
		gMission = inMission;
	
		if (gMission)
			gMission |= 0x8000;
	}
}

extern "C" void __stdcall  __AttackTest (data32 inSkill, data32 inDelay) 
{
	atttest_Delay = inDelay;
	atttest_Skill = inSkill;
}

extern "C" void __stdcall __GetSysInfo (data32 inType)
{
	getsysinfo = inType;
}
