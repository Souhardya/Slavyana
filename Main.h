#pragma once

#include "../Config/Config.h"



//
//Define
//
DWORD WINAPI ThreadStartHook(LPVOID lpParameter);

extern HINSTANCE g_hinstDLL;

extern WCHAR g_szSandBoxName[CONF_SandBoxNameLength+1];//Unicode
extern char g_strSandBoxName[CONF_SandBoxNameLength+1];//ANSI

extern WCHAR g_szRestorePath[MAX_PATH];
extern WCHAR g_szRestoreNtPath[MAX_PATH];
extern WCHAR g_szRestoreDevPath[MAX_PATH];

extern WCHAR g_szRestoreRegPath[MAX_PATH];
extern WCHAR g_szRestoreFullRegKrnlPath[MAX_PATH];

//kernel32
extern HINSTANCE g_hCloneKERNEL32;
extern WCHAR g_szCloneKERNEL32[MAX_PATH];

//fltlib
extern HINSTANCE g_hCloneFLTLIB;
extern WCHAR g_szCloneFLTLIB[MAX_PATH];

//shell32
extern HINSTANCE g_hCloneSHELL32;
extern WCHAR g_szCloneSHELL32[MAX_PATH];

//advapi32
extern HINSTANCE g_hCloneADVAPI32;
extern WCHAR g_szCloneADVAPI32[MAX_PATH];



BOOL HookPort_Start_DllMain(void);