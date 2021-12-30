#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment (lib,"shlwapi.lib")

#include "../Common/DllModule.h"
#include "./BeginHookPort.h"
#include "../Config/Config.h"
#include "../Common/DebugLog.h"



//
//Define
//
//typedef BOOL (* PGetDllModule)(CHookPortControlInterface** lpParameter);



//
//Global
//
//CHookPortControlInterface* g_hDllModule = NULL;



//
//BeginHookPort Functions
//
int BeginHookPortRoutine(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = succeed

	//
	//Get HookPort FilePath
	//
	WCHAR szHookPortPath[MAX_PATH] = {NULL};
	GetModuleFileName(NULL,szHookPortPath,MAX_PATH);
	PathRemoveFileSpec(szHookPortPath);
	lstrcat(szHookPortPath,L"\\");
	lstrcat(szHookPortPath,CONF_HookPort_FileName);

	//
	//Load HookPort
	//
	HINSTANCE hDLL = NULL;
	hDLL = LoadLibrary( szHookPortPath );
	if( !hDLL )
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return -1;
	}

//	//
//	//Call Function
//	//
//	PGetDllModule pGetDllModule = NULL;
//	pGetDllModule = (PGetDllModule)GetProcAddress(hDLL, "GetHookPortControlInit");
//
//	if( pGetDllModule )
//	{
//		pGetDllModule(&g_hDllModule);
//		g_hDllModule->InitHookPort(lpSandBoxName);
//	}
//	else
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"ERROR");
//#endif
//
//		return -1;
//	}

	return 0;
}