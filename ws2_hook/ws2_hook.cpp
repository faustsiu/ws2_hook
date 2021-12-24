// ws2_32.cpp : 定義 DLL 應用程式的匯出函式。
//

#include "stdafx.h"
#include "dllmain_export.h"
#include "sysinfo_export.h"
#include "RanHostServerData_export.h"
#include "RanGameServerData_export.h"
#include "Inventory_export.h"
#include "Mission_export.h"
#include "Monster_export.h"

// Hook function

extern "C" int __stdcall  __hook_connect (SOCKET s, const struct sockaddr *name, int namelen) 
{
	int SocketResult;
	data32 l_ServerAddress = ((struct sockaddr_in *)name)->sin_addr.S_un.S_addr;

	SocketResult = ws2_connect(s, name, namelen);

	if ((l_ServerAddress & 0xf8ffffff) == 0x20b9403a)
	{
		////58.64.185.32
		if (l_ServerAddress == 0x20b9403a) 
		{
			g_hRanHostServer = s;
		}
		////58.64.185.33 ~ 36 (33-39)
		else 
		{
			Monster_init ();
			g_hRanGameServer = s;
		}
	}


/* Old Server
if ((l_ServerAddress & 0xf8ffffff) == 0x203a69cb)
	{
		////203.105.58.32
		if (l_ServerAddress == 0x203a69cb) 
		{
			g_hRanHostServer = s;
		}
		////203.105.58.33 ~ 36 (33-39)
		else 
		{
			Monster_init ();
			g_hRanGameServer = s;
		}
	}
*/

	return SocketResult;
}

extern "C" int __stdcall  __hook_send (SOCKET s, const char *buf, int len, int flags)
{
	int SocketResult;

	SocketResult = ws2_send(s, buf, len, flags);
	if (SocketResult)
		if (s == g_hRanGameServer)
			RanParser_SendData((data8 *)buf, len);

	return SocketResult;  
}

extern "C" int __stdcall  __hook_recv (SOCKET s, char *buf, int len, int flags) 
{
	int SocketResult;

	SocketResult = ws2_recv(s, buf, len, flags);

	if (SocketResult)
	{
		if (s == g_hRanHostServer)
		{
			int bufspace = len - SocketResult;

			RanParser_RecvHostServerData ((data8 *)buf, SocketResult);
			if (bufspace >= 0xa0)
				SocketResult += sysyinfo_Display (&buf[SocketResult], bufspace);
		}
		else if (s == g_hRanGameServer)
		{
			RanParser_RecvGameServerData ((data8 *)buf, SocketResult);
			wf_flush ();
		}
	}
//	else
//		return sysyinfo_Display (buf, len);

	return SocketResult;
}
