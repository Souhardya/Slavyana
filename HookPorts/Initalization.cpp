#include <windows.h>
#include <ntsecapi.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include <tlhelp32.h>

#include "./Initalization.h"
#include "../Config/Config.h"
#include "../Common/DebugLog.h"
#include "./Privilege.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./NativeAPI_NTDLL.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtClose.h"

#include "./CloneAPI_KERNEL32.h"



//
//Initalization Functions
//
int GetSandBoxNameFromRegistry( OUT WCHAR * lpSandBoxName )
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	//
	//List SandBox in Registry
	//
	WCHAR szSandBoxName[CONF_SandBoxNameLength+1] = {NULL};

	HKEY hkSandBoxMain = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};
	int nId = 0;

	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_SandBox);

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegSandBoxPath,NULL,KEY_ALL_ACCESS,&hkSandBoxMain) == ERROR_SUCCESS )
	{
		while( RegEnumKey(hkSandBoxMain,nId,szSandBoxName,CONF_SandBoxNameLength+1) == ERROR_SUCCESS )
		{
			nId++;
			if( QuerySandBoxState(szSandBoxName) == 1 )
			{
				if( IsProcessInSandBox(GetCurrentProcessId(),szSandBoxName) == TRUE )
				{
					RegCloseKey(hkSandBoxMain);
					lstrcpy(lpSandBoxName,szSandBoxName);

					return 1;
				}
			}
		}
		RegCloseKey(hkSandBoxMain);
	}
	else
	{
		return -1;
	}

	return -1;
}

int QuerySandBoxState(IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//0 = OFF
	//1 = ON
	int iRet = 0;

	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};
	lstrcpy(szSubKeyName,CONF_SoftwareReg_SandBox);
	lstrcat(szSubKeyName,L"_");
	lstrcat(szSubKeyName,lpSandBoxName);
	lstrcat(szSubKeyName,L"\\");
	lstrcat(szSubKeyName,CONF_SoftwareReg_SandBox_SYNC);

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkSandBox) == ERROR_SUCCESS )
	{
		iRet = 1;
	}
	RegCloseKey(hkSandBox);

	return iRet;
}

BOOL IsProcessInSandBox( IN DWORD dwProcessId ,IN WCHAR * lpSandBoxName )
{
	//Return Value:
	//TRUE = Inside SandBox
	//FALSE = Outside SandBox
	BOOL bRet = FALSE;

	//
	//Query subkey [HKEY_USERS\SandBox_XXX\SYNC\PROC\(PID)]
	//
	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};

	wsprintf(
		szSubKeyName,
		L"%s_%s\\%s\\%s\\%d",
		CONF_SoftwareReg_SandBox,
		lpSandBoxName,
		CONF_SoftwareReg_SandBox_SYNC,
		CONF_SoftwareReg_SandBox_SYNC_PROC,
		dwProcessId
		);

	if( RegOpenKeyEx(HKEY_USERS,szSubKeyName,NULL,KEY_ALL_ACCESS,&hkSandBox) == ERROR_SUCCESS )
	{
		bRet = TRUE;
	}
	RegCloseKey(hkSandBox);

	return bRet;
}

int HookPortInitalization(IN HMODULE hModule, IN const WCHAR * lpSandBoxName)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 1;

	//
	//Check if is patched
	//
	HINSTANCE hDll = NULL;
	hDll = GetModuleHandleW( CONF_CloneAPI_CloneKERNEL32 );
	if( hDll )
	{
		return 0;
	}

	//
	//Save [lpSandBoxName] to [g_szSandBoxName]
	//
	lstrcpy(g_szSandBoxName,lpSandBoxName);
	WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,g_szSandBoxName,-1,g_strSandBoxName,sizeof(g_strSandBoxName),NULL,NULL);

	//
	//Get Current Module info
	//
	if( GetCurrentModuleInfo(hModule) == -1 )
	{
		iRet = -1;
	}

	//
	//Log Process Information to RegistrySYNC
	//
	//[HKEY_USERS\SandBox_XXX\SYNC\PROC\(PID)]
	LogProcessInfoToRegistrySYNC(lpSandBoxName,GetCurrentProcessId());

	//
	//Get CloneAPI file path
	//
	GetCloneAPIPath();

	//
	//Get CloenAPI Module info
	//
	GetCloenAPIModuleInfo();

	//
	//Init [Patched Process Table]
	//
	InitPatchedProcessTable();

	//
	//Get RestoreFilePath
	//
	GetRestoreFilePath(lpSandBoxName);

	//
	//Get RestoreRegPath
	//
	GetRestoreRegPath(lpSandBoxName);

	//
	//Self check
	//
	if( SelfCheck() == -1 )
	{
		iRet = -1;
	}

	return iRet;
}

int ResumeCurrentProcess(void)
{
	HANDLE hSnapshot = NULL;
	THREADENTRY32 ThreadSnapshot;
	HANDLE hThread = NULL;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,GetCurrentProcessId());
	ThreadSnapshot.dwSize=sizeof(THREADENTRY32);

	Thread32First(hSnapshot,&ThreadSnapshot);
	do
	{
		if( ThreadSnapshot.th32ThreadID != GetCurrentThreadId() &&
			ThreadSnapshot.th32OwnerProcessID == GetCurrentProcessId()
			)
		{
			hThread=CAPI_OpenThread(THREAD_SUSPEND_RESUME,0,ThreadSnapshot.th32ThreadID);

			PULONG SuspendCount = 0;
			NtResumeThread(hThread,SuspendCount);

			CloseHandle(hThread);
		}
	}
	while(Thread32Next(hSnapshot,&ThreadSnapshot));
	CloseHandle(hSnapshot);

	return 0;
}

int ConvertToGUIThread(void)
{
	WNDCLASSEX wc;
	MSG msg;
	const WCHAR	ClassName[] = L"[SandBox]";
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = g_hinstDLL;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = &ClassName[0];
	RegisterClassEx(&wc);

	msg.hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		&ClassName[0],
		L"HookPort GUI Thread",
		0,
		0,
		0,
		1,
		1,
		HWND_DESKTOP,
		NULL,
		g_hinstDLL,
		0
		);

	ShowWindow(msg.hwnd,SW_HIDE);
	UpdateWindow(msg.hwnd);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		{
			// Resize the browser object to fit the window
			//ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
			return(0);
		}
		
	case WM_CREATE:
		{
			// Embed the browser object into our host window. We need do this only
			// once. Note that the browser object will start calling some of our
			// IOleInPlaceFrame and IOleClientSite functions as soon as we start
			// calling browser object functions in EmbedBrowserObject().
			//if (EmbedBrowserObject(hwnd)) return(-1);
			
			// Another window created with an embedded browser object
			//++WindowCount;
			
			// Success
			return(0);
		}
		
	case WM_DESTROY:
		{
			// Detach the browser object from this window, and free resources.
			//UnEmbedBrowserObject(hwnd);
			
			// One less window
			//--WindowCount;
			
			// If all the windows are now closed, quit this app
			//if(WindowCount==0)PostQuitMessage(0);
			
			return(TRUE);
		}
		
		
	}
	
	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}

int LogProcessInfoToRegistrySYNC(IN const WCHAR * lpSandBoxName, IN DWORD dwProcessId)
{
	//
	//Create subkey [HKEY_USERS\SandBox_XXX\SYNC\PROC\(PID)]
	//
	HKEY hkSandBox = NULL;
	WCHAR szSubKeyName[256] = {NULL};

	wsprintf(
		szSubKeyName,
		L"%s_%s\\%s\\%s\\%d",
		CONF_SoftwareReg_SandBox,
		lpSandBoxName,
		CONF_SoftwareReg_SandBox_SYNC,
		CONF_SoftwareReg_SandBox_SYNC_PROC,
		dwProcessId
		);

	if( RegCreateKey(HKEY_USERS,szSubKeyName,&hkSandBox) == ERROR_SUCCESS )
	{
		AddProcessIdToProcTable(dwProcessId);
	}
	RegCloseKey(hkSandBox);

	return 0;
}

int SelfCheck(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	return 0;
}

int GetRestoreFilePath(IN const WCHAR * lpSandBoxName)
{
	//
	//Get RestorePath
	//
	WCHAR szRestorePath[MAX_PATH] = {NULL};

	DWORD dwType = REG_SZ;
	DWORD dwBufLen = MAX_PATH;

	HKEY hkSandBoxCfg = NULL;
	WCHAR szRegConfigPath[256] = {NULL};

	lstrcpy(szRegConfigPath,L"SOFTWARE\\");
	lstrcat(szRegConfigPath,CONF_SoftwareRegName);
	lstrcat(szRegConfigPath,L"\\");
	lstrcat(szRegConfigPath,CONF_SoftwareReg_Config);

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegConfigPath,NULL,KEY_ALL_ACCESS,&hkSandBoxCfg);

	dwType = REG_EXPAND_SZ;
	dwBufLen = MAX_PATH;

	RegQueryValueEx(
					hkSandBoxCfg,
					CONF_SoftwareReg_Config_RestorePath,
					NULL,
					&dwType,
					(LPBYTE)&szRestorePath,
					&dwBufLen
					);

	RegCloseKey(hkSandBoxCfg);

	lstrcatW(szRestorePath,L"\\");
	lstrcatW(szRestorePath,lpSandBoxName);

	lstrcpyW(g_szRestorePath,szRestorePath);

#ifdef Dbg
	DebugLog(DbgInfo,L"g_szRestorePath=");
	OutputDebugString(g_szRestorePath);
#endif

	//
	//Get RestoreNtPath
	//
	WCHAR szRestoreNtPath[MAX_PATH] = {NULL};

	lstrcpyW(szRestoreNtPath,szRestorePath);

	UNICODE_STRING usNtFileName;
	RtlDosPathNameToNtPathName_U(szRestoreNtPath,&usNtFileName,NULL,NULL);

	lstrcpyW(g_szRestoreNtPath,usNtFileName.Buffer);

#ifdef Dbg
	DebugLog(DbgInfo,L"g_szRestoreNtPath=");
	OutputDebugString(g_szRestoreNtPath);
#endif

	//
	//Get RestoreDevicePath
	//
	WCHAR szFileDevicePath[MAX_NAME] = {NULL};

	HANDLE hFile = NULL;
	OBJECT_ATTRIBUTES oaObjAttrib;
	IO_STATUS_BLOCK IoSB;
	NTSTATUS nCreateFile;

	InitializeObjectAttributes(
		&oaObjAttrib,
		&usNtFileName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	nCreateFile = NtCreateFile(
				&hFile,
				FILE_LIST_DIRECTORY | SYNCHRONIZE,
				&oaObjAttrib,
				&IoSB,
				NULL,
				FILE_ATTRIBUTE_NORMAL,
				FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
				FILE_OPEN,
				FILE_SYNCHRONOUS_IO_NONALERT | FILE_DIRECTORY_FILE,
				NULL,
				NULL
				);

	if( nCreateFile == STATUS_SUCCESS )
	{
		QueryNameInfoByNtQueryObject(hFile,szFileDevicePath);
		lstrcpyW(g_szRestoreDevPath,szFileDevicePath);
	}
	NtClose(hFile);

#ifdef Dbg
	DebugLog(DbgInfo,L"g_szRestoreDevPath=");
	OutputDebugString(g_szRestoreDevPath);
#endif

	return 0;
}

int GetRestoreRegPath(IN const WCHAR * lpSandBoxName)
{
	//
	//Get RestoreRegPath
	//
	WCHAR szSubKeyName[256] = {NULL};
	wsprintfW(szSubKeyName,L"%s_%s",CONF_SoftwareReg_SandBox,lpSandBoxName);

	lstrcpyW(g_szRestoreRegPath,szSubKeyName);

	lstrcpyW(g_szRestoreFullRegKrnlPath,L"\\REGISTRY\\USER\\");
	lstrcatW(g_szRestoreFullRegKrnlPath,szSubKeyName);

	return 0;
}

int GetCloneAPIPath(void)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	//
	//Get [CloneAPI] directory
	//
	WCHAR szCloneAPIDirectory[MAX_PATH] = {NULL};
	GetModuleFileName(g_hinstDLL,szCloneAPIDirectory,MAX_PATH);
	PathRemoveFileSpec(szCloneAPIDirectory);
	lstrcat(szCloneAPIDirectory,L"\\");
	lstrcat(szCloneAPIDirectory,CONF_CloneAPI_DirectoryName);

	//kernel32.dll
	wsprintf(g_szCloneKERNEL32,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneKERNEL32);

#ifdef Dbg
	WCHAR szDebugString[512] = {0};
	wsprintf(szDebugString,L"g_szCloneKERNEL32=[%s]",g_szCloneKERNEL32);
	DebugLog(DbgInfo,szDebugString);
#endif

	//fltlib.dll
	wsprintf(g_szCloneFLTLIB,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneFLTLIB);

#ifdef Dbg
	wsprintf(szDebugString,L"g_szCloneFLTLIB=[%s]",g_szCloneFLTLIB);
	DebugLog(DbgInfo,szDebugString);
#endif

	//shell32.dll
	wsprintf(g_szCloneSHELL32,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneSHELL32);

#ifdef Dbg
	wsprintf(szDebugString,L"g_szCloneSHELL32=[%s]",g_szCloneSHELL32);
	DebugLog(DbgInfo,szDebugString);
#endif

	//advapi32.dll
	wsprintf(g_szCloneADVAPI32,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneADVAPI32);

#ifdef Dbg
	wsprintf(szDebugString,L"g_szCloneADVAPI32=[%s]",g_szCloneADVAPI32);
	DebugLog(DbgInfo,szDebugString);
#endif

	return iRet;
}