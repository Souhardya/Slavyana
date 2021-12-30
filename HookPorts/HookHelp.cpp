#include <windows.h>
#include <ntsecapi.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <shlobj.h>
#pragma comment(lib,"psapi.lib")
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "./HookHelp.h"
#include "../Common/DebugLog.h"
#include "./Main.h"
#include "./BeginDispatch.h"
#include "../Config/Config.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./NativeAPI_NTDLL.h"

#include "./CloneAPI_KERNEL32.h"
#include "./CloneAPI_FLTLIB.h"



//
//Global
//

//PatchedProcessTable
DWORD g_dwPatchedProcessId[CONF_HookPort_MaxProcessCount];

//Current module
DWORD g_dwCurrentModule_ImageBase = 0;
DWORD g_dwCurrentModule_ImageHigh = 0;

//kernel32.dll
DWORD g_dwCloneAPIModule_KERNEL32_ImageBase = 0;
DWORD g_dwCloneAPIModule_KERNEL32_ImageHigh = 0;

//fltlib.dll
DWORD g_dwCloneAPIModule_FLTLIB_ImageBase = 0;
DWORD g_dwCloneAPIModule_FLTLIB_ImageHigh = 0;

//shell32.dll
DWORD g_dwCloneAPIModule_SHELL32_ImageBase = 0;
DWORD g_dwCloneAPIModule_SHELL32_ImageHigh = 0;

//advapi32.dll
DWORD g_dwCloneAPIModule_ADVAPI32_ImageBase = 0;
DWORD g_dwCloneAPIModule_ADVAPI32_ImageHigh = 0;



//
//HookHelp Functions
//
int GetCurrentModuleInfo( IN HMODULE hModule )
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = -1;

	//
	//Calculate Base Address & Size of Image
	//
	if( hModule )
	{
		MODULEINFO ModInfo = {0};

		if( GetModuleInformation(GetCurrentProcess(),hModule,&ModInfo,sizeof(ModInfo)) == TRUE )
		{
			//Base Address
			g_dwCurrentModule_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCurrentModule_ImageHigh = (DWORD)ModInfo.lpBaseOfDll+ModInfo.SizeOfImage;

			if( g_dwCurrentModule_ImageBase > 0 &&
				g_dwCurrentModule_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = {0};
				wsprintf(szDebugString,L"CurrentModuleImageBase = [0x%08X] CurrentModuleImageHigh = [0x%08X]",g_dwCurrentModule_ImageBase,g_dwCurrentModule_ImageHigh);
				DebugLog(DbgInfo,szDebugString);
#endif
			}
		}
	}

	return iRet;
}

int GetCloenAPIModuleInfo(void)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	//
	//Calculate Base Address & Size of Image
	//

	//kernel32.dll
	g_hCloneKERNEL32 = LoadLibrary( g_szCloneKERNEL32 );
	if( g_hCloneKERNEL32 )
	{
		MODULEINFO ModInfo = {0};

		if( GetModuleInformation(GetCurrentProcess(),g_hCloneKERNEL32,&ModInfo,sizeof(ModInfo)) == TRUE )
		{
			//Base Address
			g_dwCloneAPIModule_KERNEL32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_KERNEL32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll+ModInfo.SizeOfImage;

			if( g_dwCloneAPIModule_KERNEL32_ImageBase > 0 &&
				g_dwCloneAPIModule_KERNEL32_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = {0};
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_KERNEL32_ImageBase,
					g_dwCloneAPIModule_KERNEL32_ImageHigh
					);
				DebugLog(DbgInfo,szDebugString);
#endif
			}
		}
	}

	//fltlib.dll
	g_hCloneFLTLIB = LoadLibrary( g_szCloneFLTLIB );
	if( g_hCloneFLTLIB )
	{
		MODULEINFO ModInfo = {0};

		if( GetModuleInformation(GetCurrentProcess(),g_hCloneFLTLIB,&ModInfo,sizeof(ModInfo)) == TRUE )
		{
			//Base Address
			g_dwCloneAPIModule_FLTLIB_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_FLTLIB_ImageHigh = (DWORD)ModInfo.lpBaseOfDll+ModInfo.SizeOfImage;

			if( g_dwCloneAPIModule_FLTLIB_ImageBase > 0 &&
				g_dwCloneAPIModule_FLTLIB_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = {0};
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_FLTLIB_ImageBase,
					g_dwCloneAPIModule_FLTLIB_ImageHigh
					);
				DebugLog(DbgInfo,szDebugString);
#endif
			}
		}
	}

	//shell32.dll
	g_hCloneSHELL32 = LoadLibrary( g_szCloneSHELL32 );
	if( g_hCloneSHELL32 )
	{
		MODULEINFO ModInfo = {0};

		if( GetModuleInformation(GetCurrentProcess(),g_hCloneSHELL32,&ModInfo,sizeof(ModInfo)) == TRUE )
		{
			//Base Address
			g_dwCloneAPIModule_SHELL32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_SHELL32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll+ModInfo.SizeOfImage;

			if( g_dwCloneAPIModule_SHELL32_ImageBase > 0 &&
				g_dwCloneAPIModule_SHELL32_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = {0};
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_SHELL32_ImageBase,
					g_dwCloneAPIModule_SHELL32_ImageHigh
					);
				DebugLog(DbgInfo,szDebugString);
#endif
			}
		}
	}

	//advapi32.dll
	g_hCloneADVAPI32 = LoadLibrary( g_szCloneADVAPI32 );
	if( g_hCloneADVAPI32 )
	{
		MODULEINFO ModInfo = {0};

		if( GetModuleInformation(GetCurrentProcess(),g_hCloneADVAPI32,&ModInfo,sizeof(ModInfo)) == TRUE )
		{
			//Base Address
			g_dwCloneAPIModule_ADVAPI32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_ADVAPI32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll+ModInfo.SizeOfImage;

			if( g_dwCloneAPIModule_ADVAPI32_ImageBase > 0 &&
				g_dwCloneAPIModule_ADVAPI32_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = {0};
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_ADVAPI32_ImageBase,
					g_dwCloneAPIModule_ADVAPI32_ImageHigh
					);
				DebugLog(DbgInfo,szDebugString);
#endif
			}
		}
	}

	return iRet;
}

BOOL IsBypassCaller( IN DWORD lpdwReturnAddress )
{
	//Return Value:
	//TRUE = bypass
	//FALSE = not bypass

	//
	//Check Return Address
	//

	//Current module
	if( lpdwReturnAddress >= g_dwCurrentModule_ImageBase &&
		lpdwReturnAddress <= g_dwCurrentModule_ImageHigh
		)
	{
		return TRUE;
	}

	//kernel32.dll
	if( lpdwReturnAddress >= g_dwCloneAPIModule_KERNEL32_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_KERNEL32_ImageHigh
		)
	{
		return TRUE;
	}

	//fltlib.dll
	if( lpdwReturnAddress >= g_dwCloneAPIModule_FLTLIB_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_FLTLIB_ImageHigh
		)
	{
		return TRUE;
	}

	//shell32.dll
	if( lpdwReturnAddress >= g_dwCloneAPIModule_SHELL32_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_SHELL32_ImageHigh
		)
	{
		return TRUE;
	}

	//advapi.dll
	if( lpdwReturnAddress >= g_dwCloneAPIModule_ADVAPI32_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_ADVAPI32_ImageHigh
		)
	{
		return TRUE;
	}

	return FALSE;
}

BOOL IsProcessPatched( IN DWORD dwProcessId ,IN BOOL bQueryInCached )
{
	//Return Value:
	//TRUE = Patched
	//FALSE = not Patched
	BOOL bRet = FALSE;

	//
	//Query in cached of [Patched Process Table]
	//
	if( bQueryInCached == TRUE )
	{
		for(int i=0;i<CONF_HookPort_MaxProcessCount;i++)
		{
			if( g_dwPatchedProcessId[i] == 0 )
			{
				break;
			}
			if( g_dwPatchedProcessId[i] == dwProcessId )
			{
				bRet = TRUE;
				break;
			}
		}

		return bRet;
	}

	//
	//Query subkey [HKEY_USERS\SandBox_XXX\SYNC\PROC\(PID)]
	//
	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};

	wsprintf(
		szSubKeyName,
		L"%s_%s\\%s\\%s\\%d",
		CONF_SoftwareReg_SandBox,
		g_szSandBoxName,
		CONF_SoftwareReg_SandBox_SYNC,
		CONF_SoftwareReg_SandBox_SYNC_PROC,
		dwProcessId
		);

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkSandBox) == ERROR_SUCCESS )
	{
		HANDLE hProc = NULL;
		hProc = CAPI_OpenProcess( PROCESS_QUERY_INFORMATION,FALSE,dwProcessId);
		if( !hProc )
		{
			bRet = FALSE;
		}
		else
		{
			bRet = TRUE;
		}
		CloseHandle(hProc);
	}
	RegCloseKey(hkSandBox);

	return bRet;
}

BOOL InitPatchedProcessTable(void)
{
	for(int i=0;i<CONF_HookPort_MaxProcessCount;i++)
	{
		g_dwPatchedProcessId[i] = 0;
	}

	return TRUE;
}

BOOL AddProcessIdToProcTable( IN DWORD dwProcessId )
{
	for(int i=0;i<CONF_HookPort_MaxProcessCount;i++)
	{
		if( g_dwPatchedProcessId[i] == 0 || 
			g_dwPatchedProcessId[i] == -1
			)
		{
			g_dwPatchedProcessId[i] = dwProcessId;
			break;
		}
	}

	return TRUE;
}

BOOL DelProcessIdFromProcTable( IN DWORD dwProcessId )
{
	for(int i=0;i<CONF_HookPort_MaxProcessCount;i++)
	{
		if( g_dwPatchedProcessId[i] == 0 )
		{
			break;
		}
		if( g_dwPatchedProcessId[i] == dwProcessId )
		{
			g_dwPatchedProcessId[i] = -1;
			break;
		}
	}

	return TRUE;
}

int ScanCurrentProcModule(void)
{
	HANDLE hSnapshot= NULL;
	MODULEENTRY32 moudle;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
	moudle.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot,&moudle);
	do
	{
		//
		//Ignore current module
		//
		if( moudle.hModule == g_hinstDLL )
		{
			continue;
		}
		//
		//Check if is target module
		//
		CheckTargetModule(moudle.szModule);
	}
	while( Module32Next(hSnapshot,&moudle) );
	CloseHandle(hSnapshot);

	return 0;
}

int GetFileDeleteTraceCount( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName )
{
	//Return Value:
	//-1 = error
	//nonezero = succeed
	int iRet = 1;

	////
	////Check if is Directory
	////
	//if( lstrlenW(lpszFileDosName) <= lstrlenW(L"C:\\C") )
	//{
	//	return iRet;
	//}

	////
	////Get FileDosPath
	////
	//WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	//WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	//WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	//WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	//WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	//lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
	//lstrcpyW(szFileDosName,lpszFileDosName);

	//lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

	////
	////Convert [DeviceName] to [DosName]
	////
	//CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

	//wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

	////
	////Check if is ShortPathName
	////
	//WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
	//lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

	//if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
	//{
	//	if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
	//	{
	//		//
	//		//Convert ShortPath to LongPath
	//		//
	//		lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);

	//		lstrcpyW(szLinkFileDevPath,szDeviceName);
	//		lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}

	////
	////Query reg value [HKEY_USERS\SandBox_XXX\(DelTrace_File)]
	////
	//HKEY hkDeleteTrace = NULL;
	//WCHAR szSubKeyName[256] = {NULL};

	//wsprintf(
	//	szSubKeyName,
	//	L"%s_%s\\%s",
	//	CONF_SoftwareReg_SandBox,
	//	g_szSandBoxName,
	//	CONF_SoftwareReg_SandBox_DelTrace_File
	//	);

	//if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDeleteTrace) == ERROR_SUCCESS )
	//{
	//	WCHAR szRegValueName[MAX_NAME] = {0};
	//	DWORD dwIndex = 0;
	//	DWORD cchValue = MAX_NAME;
	//	while( RegEnumValue(hkDeleteTrace,dwIndex,szRegValueName,&cchValue,NULL,NULL,NULL,NULL) == ERROR_SUCCESS )
	//	{
	//		cchValue = MAX_NAME;
	//		dwIndex++;

	//		//
	//		//Get Directory
	//		//
	//		PathRemoveFileSpec(szRegValueName);
	//		if( wcsicmp(szRegValueName,szLinkFileDevPath) == 0 )
	//		{
	//			iRet++;
	//		}
	//	}
	//}
	//RegCloseKey(hkDeleteTrace);

	return iRet;
}

int QueryFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName ,IN WCHAR * lpszFileName, IN ULONG lpuNameLength )
{
	//Return Value:
	//-1 = error
	//0 = Exist
	//1 = Deleted
	int iRet = 0;

	////
	////Get FileDosPath
	////
	//WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	//WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	//WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	//WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	//WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	//lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
	//lstrcpyW(szFileDosName,lpszFileDosName);

	//lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

	////
	////Convert [DeviceName] to [DosName]
	////
	//CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

	//wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

	////
	////Check if is ShortPathName
	////
	//WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
	//lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

	//if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
	//{
	//	if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
	//	{
	//		//
	//		//Convert ShortPath to LongPath
	//		//
	//		lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);

	//		lstrcpyW(szLinkFileDevPath,szDeviceName);
	//		lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}

	////
	////Query reg value [HKEY_USERS\SandBox_XXX\(DelTrace_File)]
	////
	//HKEY hkDeleteTrace = NULL;
	//WCHAR szSubKeyName[256] = {NULL};

	//wsprintf(
	//	szSubKeyName,
	//	L"%s_%s\\%s",
	//	CONF_SoftwareReg_SandBox,
	//	g_szSandBoxName,
	//	CONF_SoftwareReg_SandBox_DelTrace_File
	//	);

	//if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDeleteTrace) == ERROR_SUCCESS )
	//{
	//	WCHAR szRealFileName[MAX_NAME] = {NULL};
	//	WCHAR szLinkFileDevFullPath[MAX_NAME] = {NULL};
	//	WCHAR szRegDosPath[MAX_NAME] = {NULL};
	//	DWORD dwType = REG_SZ;
	//	DWORD dwBufLen = MAX_NAME;

	//	lstrcpyW(szRealFileName,lpszFileName);
	//	lstrcpynW(szRealFileName,szRealFileName,(lpuNameLength/2)+1);

	//	lstrcpyW(szLinkFileDevFullPath,szLinkFileDevPath);
	//	lstrcatW(szLinkFileDevFullPath,L"\\");
	//	lstrcatW(szLinkFileDevFullPath,szRealFileName);

	//	if( RegQueryValueEx(
	//					hkDeleteTrace,
	//					szLinkFileDevFullPath,
	//					NULL,
	//					&dwType,
	//					(LPBYTE)&szRegDosPath,
	//					&dwBufLen
	//					) == ERROR_SUCCESS )
	//	{
	//		iRet = 1;
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}
	//RegCloseKey(hkDeleteTrace);

	return iRet;
}

//int AddFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName )
//{
//	//Return Value:
//	//-1 = error
//	//0 = succeed
//	int iRet = 0;
//
//	//
//	//Link Table
//	//
//	//Link(FileDevicePath->FileDosPath):
//	//ValueName
//	//[\\Device\\HarddiskVolume1\\WINDOWS\\XXX](FileDevicePath)
//	//Data
//	//[C:\\WINDOWS\\XXX](FileDosPath)
//
//	//
//	//Get FileDosPath
//	//
//	WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
//	WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
//	WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)
//
//	WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
//	WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)
//
//	lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
//	lstrcpyW(szFileDosName,lpszFileDosName);
//
//	lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);
//
//	//
//	//Convert [DeviceName] to [DosName]
//	//
//	CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);
//
//	wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);
//
//	//
//	//Check if is ShortPathName
//	//
//	WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
//	lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);
//
//	if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
//	{
//		if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
//		{
//			//
//			//Convert ShortPath to LongPath
//			//
//			lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);
//
//			lstrcpyW(szLinkFileDevPath,szDeviceName);
//			lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
//		}
//	}
//	else
//	{
//		iRet = -1;
//	}
//
//	//
//	//Set reg value [HKEY_USERS\SandBox_XXX\(DelTrace_File)]
//	//
//	HKEY hkDeleteTrace = NULL;
//	WCHAR szSubKeyName[256] = {NULL};
//
//	wsprintf(
//		szSubKeyName,
//		L"%s_%s\\%s",
//		CONF_SoftwareReg_SandBox,
//		g_szSandBoxName,
//		CONF_SoftwareReg_SandBox_DelTrace_File
//		);
//
//	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDeleteTrace) == ERROR_SUCCESS )
//	{
//		RegSetValueEx(
//			hkDeleteTrace,
//			szLinkFileDevPath,
//			NULL,
//			REG_SZ,
//			(const BYTE *)szLinkFileDosPath,
//			(DWORD)(lstrlenW(szLinkFileDosPath)*sizeof(WCHAR))
//			);
//	}
//	else
//	{
//		iRet = -1;
//	}
//	RegCloseKey(hkDeleteTrace);
//
//	return iRet;
//}

int DelFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName )
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	////
	////Link Table
	////
	////Link(FileDevicePath->FileDosPath):
	////ValueName
	////[\\Device\\HarddiskVolume1\\WINDOWS\\XXX](FileDevicePath)
	////Data
	////[C:\\WINDOWS\\XXX](FileDosPath)

	////
	////Get FileDosPath
	////
	//WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	//WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	//WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	//WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	//WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	//lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
	//lstrcpyW(szFileDosName,lpszFileDosName);

	//lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

	////
	////Convert [DeviceName] to [DosName]
	////
	//CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

	//wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

	////
	////Check if is ShortPathName
	////
	//WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
	//lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

	//if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
	//{
	//	if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
	//	{
	//		//
	//		//Convert ShortPath to LongPath
	//		//
	//		lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);

	//		lstrcpyW(szLinkFileDevPath,szDeviceName);
	//		lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}

	////
	////Delete reg value [HKEY_USERS\SandBox_XXX\(DelTrace_File)]
	////
	//HKEY hkDeleteTrace = NULL;
	//WCHAR szSubKeyName[256] = {NULL};

	//wsprintf(
	//	szSubKeyName,
	//	L"%s_%s\\%s",
	//	CONF_SoftwareReg_SandBox,
	//	g_szSandBoxName,
	//	CONF_SoftwareReg_SandBox_DelTrace_File
	//	);

	//if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDeleteTrace) == ERROR_SUCCESS )
	//{
	//	RegDeleteValue(hkDeleteTrace,szLinkFileDevPath);
	//}
	//else
	//{
	//	iRet = -1;
	//}
	//RegCloseKey(hkDeleteTrace);

	return iRet;
}

int TestFileDeleteTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName )
{
	//Return Value:
	//-1 = error
	//0 = Exist
	//1 = Deleted
	int iRet = 0;

	////
	////Get FileDosPath
	////
	//WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	//WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	//WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	//WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	//WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	//lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
	//lstrcpyW(szFileDosName,lpszFileDosName);

	//lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

	////
	////Convert [DeviceName] to [DosName]
	////
	//CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

	//wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

	////
	////Check if is ShortPathName
	////
	//WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
	//lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

	//if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
	//{
	//	if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
	//	{
	//		//
	//		//Convert ShortPath to LongPath
	//		//
	//		lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);

	//		lstrcpyW(szLinkFileDevPath,szDeviceName);
	//		lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}

	////
	////Query reg value [HKEY_USERS\SandBox_XXX\(DelTrace_File)]
	////
	//HKEY hkDeleteTrace = NULL;
	//WCHAR szSubKeyName[256] = {NULL};

	//wsprintf(
	//	szSubKeyName,
	//	L"%s_%s\\%s",
	//	CONF_SoftwareReg_SandBox,
	//	g_szSandBoxName,
	//	CONF_SoftwareReg_SandBox_DelTrace_File
	//	);

	//if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDeleteTrace) == ERROR_SUCCESS )
	//{
	//	WCHAR szRegDosPath[MAX_NAME] = {NULL};
	//	DWORD dwType = REG_SZ;
	//	DWORD dwBufLen = MAX_NAME;

	//	if( RegQueryValueEx(
	//					hkDeleteTrace,
	//					szLinkFileDevPath,
	//					NULL,
	//					&dwType,
	//					(LPBYTE)&szRegDosPath,
	//					&dwBufLen
	//					) == ERROR_SUCCESS )
	//	{
	//		iRet = 1;
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}
	//RegCloseKey(hkDeleteTrace);

	return iRet;
}

int AddFileDispatchTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName ,OUT WCHAR * lpszNewFileNtPath )
{
	int iRet = 0;

	////
	////Get FileDosPath
	////
	//WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	//WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	//WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	//WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	//WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	//lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
	//lstrcpyW(szFileDosName,lpszFileDosName);

	//lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

	////
	////Convert [DeviceName] to [DosName]
	////
	//CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

	//wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

	////
	////Check if is ShortPathName
	////
	//WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
	//lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

	//if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
	//{
	//	if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
	//	{
	//		//
	//		//Convert ShortPath to LongPath
	//		//
	//		lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);

	//		lstrcpyW(szLinkFileDevPath,szDeviceName);
	//		lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}

	////
	////Get NewFileNtPath
	////
	//WCHAR szNewFileNtPath[MAX_NAME] = {NULL};//(X:\\SandBox\\DefaultBox\\Device\\HarddiskVolume1\\XXX)

	//wsprintf(szNewFileNtPath,L"%s%s",g_szRestorePath,szLinkFileDevPath);

	////
	////Init UNICODE_STRING
	////
	//WCHAR szNtFileName[MAX_NAME] = {NULL};//(\\??\\X:\\SandBox\\DefaultBox\\Device\\HarddiskVolume1\\XXX)
	//UNICODE_STRING usNtFileName;

	//RtlDosPathNameToNtPathName_U(szNewFileNtPath,&usNtFileName,NULL,NULL);

	//lstrcpyW(lpszNewFileNtPath,usNtFileName.Buffer);

	//lstrcpyW(szNtFileName,usNtFileName.Buffer);

	////
	////Free UNICODE_STRING
	////
	//RtlFreeUnicodeString(&usNtFileName);

	////
	////Set reg value [HKEY_USERS\SandBox_XXX\(DspTrace_File)]
	////
	//HKEY hkDispatchTrace = NULL;
	//WCHAR szSubKeyName[256] = {NULL};

	//wsprintf(
	//	szSubKeyName,
	//	L"%s_%s\\%s",
	//	CONF_SoftwareReg_SandBox,
	//	g_szSandBoxName,
	//	CONF_SoftwareReg_SandBox_DspTrace_File
	//	);

	//if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDispatchTrace) == ERROR_SUCCESS )
	//{
	//	RegSetValueEx(
	//		hkDispatchTrace,
	//		szLinkFileDevPath,
	//		NULL,
	//		REG_SZ,
	//		(const BYTE *)szNtFileName,
	//		(DWORD)(lstrlenW(szNtFileName)*sizeof(WCHAR))
	//		);
	//}
	//else
	//{
	//	iRet = -1;
	//}
	//RegCloseKey(hkDispatchTrace);

	////
	////SHCreateDirectoryEx(NULL,lpDir,NULL);
	////
	//PathRemoveFileSpec(szNewFileNtPath);

	//SHCreateDirectoryEx(NULL,szNewFileNtPath,NULL);

	return iRet;
}

int QueryFileDispatchTrace( IN WCHAR * lpszFileDevicePath, IN WCHAR * lpszFileDosName ,OUT WCHAR * lpszNewFileNtPath )
{
	//Return Value:
	//-1 = error
	//0 = Not existed
	//1 = Existed
	int iRet = 0;

	////
	////Get FileDosPath
	////
	//WCHAR szDeviceName[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1)
	//WCHAR szVolumeName[MAX_NAME] = {NULL};//(C:)
	//WCHAR szFileDosName[MAX_NAME] = {NULL};//(\\WINDOWS\\XXX)

	//WCHAR szLinkFileDevPath[MAX_NAME] = {NULL};//(\\Device\\HarddiskVolume1\\WINDOWS\\XXX)
	//WCHAR szLinkFileDosPath[MAX_NAME] = {NULL};//(C:\\WINDOWS\\XXX)

	//lstrcpyW(szLinkFileDevPath,lpszFileDevicePath);
	//lstrcpyW(szFileDosName,lpszFileDosName);

	//lstrcpynW(szDeviceName,szLinkFileDevPath,lstrlenW(szLinkFileDevPath) - lstrlenW(szFileDosName) + 1);

	////
	////Convert [DeviceName] to [DosName]
	////
	//CAPI_FilterGetDosName(szDeviceName,szVolumeName,MAX_NAME);

	//wsprintfW(szLinkFileDosPath,L"%s%s",szVolumeName,szFileDosName);

	////
	////Check if is ShortPathName
	////
	//WCHAR szLinkFileDosLongPath[MAX_NAME] = {NULL};
	//lstrcpyW(szLinkFileDosLongPath,szLinkFileDosPath);

	//if( CAPI_GetLongPathNameW(szLinkFileDosPath,szLinkFileDosLongPath,MAX_NAME) )
	//{
	//	if( lstrlenW(szLinkFileDosPath) != lstrlenW(szLinkFileDosLongPath) )
	//	{
	//		//
	//		//Convert ShortPath to LongPath
	//		//
	//		lstrcpyW(szLinkFileDosPath,szLinkFileDosLongPath);

	//		lstrcpyW(szLinkFileDevPath,szDeviceName);
	//		lstrcatW(szLinkFileDevPath,&szLinkFileDosLongPath[lstrlen(szVolumeName)]);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}

	////
	////Query reg value [HKEY_USERS\SandBox_XXX\(DspTrace_File)]
	////
	//HKEY hkDispatchTrace = NULL;
	//WCHAR szSubKeyName[256] = {NULL};

	//wsprintf(
	//	szSubKeyName,
	//	L"%s_%s\\%s",
	//	CONF_SoftwareReg_SandBox,
	//	g_szSandBoxName,
	//	CONF_SoftwareReg_SandBox_DspTrace_File
	//	);

	//if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkDispatchTrace) == ERROR_SUCCESS )
	//{
	//	WCHAR szRegNewNtPath[MAX_NAME] = {NULL};

	//	DWORD dwType = REG_SZ;
	//	DWORD dwBufLen = MAX_NAME;

	//	if( RegQueryValueEx(
	//					hkDispatchTrace,
	//					szLinkFileDevPath,
	//					NULL,
	//					&dwType,
	//					(LPBYTE)&szRegNewNtPath,
	//					&dwBufLen
	//					) == ERROR_SUCCESS )
	//	{
	//		iRet = 1;

	//		lstrcpyW(lpszNewFileNtPath,szRegNewNtPath);
	//	}
	//}
	//else
	//{
	//	iRet = -1;
	//}
	//RegCloseKey(hkDispatchTrace);

	return iRet;
}