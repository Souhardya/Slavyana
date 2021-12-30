#include <windows.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <tlhelp32.h>


#include "./Main.h"
#include "../Config/Config.h"
#include "../Common/DllModule.h"
#include "../Common/DebugLog.h"
#include "./Initalization.h"
#include "./BeginDispatch.h"
#include "./RemoteInjection.h"



//
//Define
//
//class CHookPortControlInit : public CHookPortControlInterface
//{
//public:
//
//	virtual BOOL InitHookPort(LPCWSTR lpSandBoxName);
//};



//
//Global
//
HINSTANCE g_hinstDLL = NULL;

//CHookPortControlInit g_hDllModule;

WCHAR g_szSandBoxName[CONF_SandBoxNameLength+1] = {NULL};//Unicode
char g_strSandBoxName[CONF_SandBoxNameLength+1] = {NULL};//ANSI

WCHAR g_szRestorePath[MAX_PATH] = {NULL};
WCHAR g_szRestoreNtPath[MAX_PATH] = {NULL};
WCHAR g_szRestoreDevPath[MAX_PATH] = {NULL};

WCHAR g_szRestoreRegPath[MAX_PATH] = {NULL};
WCHAR g_szRestoreFullRegKrnlPath[MAX_PATH] = {NULL};

//kernel32
HINSTANCE g_hCloneKERNEL32 = NULL;
WCHAR g_szCloneKERNEL32[MAX_PATH] = {NULL};

//fltlib
HINSTANCE g_hCloneFLTLIB = NULL;
WCHAR g_szCloneFLTLIB[MAX_PATH] = {NULL};

//shell32
HINSTANCE g_hCloneSHELL32 = NULL;
WCHAR g_szCloneSHELL32[MAX_PATH] = {NULL};

//advapi32
HINSTANCE g_hCloneADVAPI32 = NULL;
WCHAR g_szCloneADVAPI32[MAX_PATH] = {NULL};



//
//DllMain
//
BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	g_hinstDLL = hinstDLL;

	//
	//Sets a random starting point
	//
	srand( (int) time(0) );

    switch( fdwReason )
    {
		case DLL_PROCESS_ATTACH:
			{
				//
				//Initialize once for each new process
				//Return FALSE to fail DLL load
				//

				if( HookPort_Start_DllMain() == FALSE )
				{
					ExitProcess(0);
				}
			}break;
		case DLL_THREAD_ATTACH:
			{
				//
				//Do thread-specific initialization
				//
			}break;
		case DLL_THREAD_DETACH:
			{
				//
				//Do thread-specific cleanup
				//
			}break;
		case DLL_PROCESS_DETACH:
			{
				//
				//Perform any necessary cleanup
				//
			}break;
    }

	//
	//Successful DLL_PROCESS_ATTACH
	//
	return TRUE;
}



//
//Dll Export [GetHookPortControlInit]
//
//extern "C" __declspec(dllexport) BOOL GetHookPortControlInit(CHookPortControlInterface** lpParameter)   
//{
//	*lpParameter = (CHookPortControlInterface*)&g_hDllModule;
//
//	return TRUE;
//}
//
//BOOL CHookPortControlInit::InitHookPort(LPCWSTR lpSandBoxName)
//{
//	//
//	//Initalization
//	//
//	int iInitState = -1;
//	iInitState = HookPortInitalization(g_hinstDLL,lpSandBoxName);
//
//	if( iInitState == -1 ||
//		iInitState == 0
//		)
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"ERROR");
//#endif
//
//		return FALSE;
//	}
//
//	//
//	//Begin Dispatch
//	//
//	if( BeginDispatchBySandBoxName(lpSandBoxName) == -1 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"ERROR");
//#endif
//
//		return FALSE;
//	}
//
//	return TRUE;
//}

//
//Dll Export [HookPort_Start]
//
extern "C" __declspec(dllexport) BOOL HookPort_Start(LPVOID lpParameter)
{
	//
	//Initalization
	//
	LPOBJECTCONFIG OBJCfg = (LPOBJECTCONFIG)lpParameter;
	int iInitState = -1;
	iInitState = HookPortInitalization(g_hinstDLL,OBJCfg->szSandBoxName);

	if( iInitState == -1 ||
		iInitState == 0
		)
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return FALSE;
	}

	//
	//Begin Dispatch
	//
	if( BeginDispatchBySandBoxName(g_szSandBoxName) == -1 )
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return FALSE;
	}

	//
	//Resume Current Process
	//
	ResumeCurrentProcess();

	//
	//Convert thread to a GUI thread
	//
	ConvertToGUIThread();

	return TRUE;
}

BOOL HookPort_Start_DllMain(void)
{
	//
	//Get SandBox Name
	//
	WCHAR szSandBoxName[CONF_SandBoxNameLength+1] = {NULL};

	if( GetSandBoxNameFromRegistry(szSandBoxName) == -1 )
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return FALSE;
	}

	//
	//Initalization
	//
	int iInitState = -1;
	iInitState = HookPortInitalization(g_hinstDLL,szSandBoxName);

	if( iInitState == -1 ||
		iInitState == 0
		)
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return FALSE;
	}

	//
	//Begin Dispatch
	//
	if( BeginDispatchBySandBoxName(g_szSandBoxName) == -1 )
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"ERROR");
#endif

		return FALSE;
	}

	return TRUE;
}