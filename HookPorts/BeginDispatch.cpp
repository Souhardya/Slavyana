#include <windows.h>
#include <stdio.h>

#include "./BeginDispatch.h"
#include "../Config/Config.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"

#include "./Dispatch_NTDLL.h"
#include "./Dispatch_KERNEL32.h"
#include "./Dispatch_USER32.h"
#include "./Dispatch_ADVAPI32.h"



//
//BeginDispatch Functions
//
int BeginDispatchBySandBoxName(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 0;

	//
	//Patch DLL
	//
	Dispatch_NTDLL_Start();

	Dispatch_KERNEL32_Start();

	Dispatch_USER32_Start();

	Dispatch_ADVAPI32_Start();

	ScanCurrentProcModule();

	return iRet;
}

int CheckTargetModule( IN const WCHAR * lpModuleName )
{
	//
	//List of Module will be patch
	//
	//ntdll.dll kernel32.dll user32.dll gdi32.dll advapi32.dll shell32.dll
	//ole32.dll ws2_32.dll crypt32.dll msi.dll sxs.dll secur32.dll hnetcfg.dll
	//setupapi.dll wtsapi32.dll pstorec.dll

//	if( wcsicmp(lpModuleName,L"kernel32.dll") == 0 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"kernel32.dll Patched!");
//#endif
//	}
//
//	if( wcsicmp(lpModuleName,L"user32.dll") == 0 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"user32.dll Patched!");
//#endif
//	}
//
//	if( wcsicmp(lpModuleName,L"gdi32.dll") == 0 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"gdi32.dll Patched!");
//#endif
//	}
//
//	if( wcsicmp(lpModuleName,L"advapi32.dll") == 0 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"advapi32.dll Patched!");
//#endif
//	}
//
//	if( wcsicmp(lpModuleName,L"shell32.dll") == 0 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"shell32.dll Patched!");
//#endif
//	}

	return 0;
}