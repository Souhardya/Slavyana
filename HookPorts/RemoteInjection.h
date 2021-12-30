#pragma once

#include "../Config/Config.h"

//
//Definition
//

//
//RemoteThread Error Codes
//
#define DEF_ThreadError_Unknown				-1
#define DEF_ThreadError_GetKERNEL32Mod		10
#define DEF_ThreadError_CallLoadLibraryA	11



//
//struct
//
typedef struct tagObjectConfig
{
	WCHAR szSandBoxName[CONF_SandBoxNameLength+1];
}OBJECTCONFIG, *LPOBJECTCONFIG;

int InjectProcess(IN HMODULE hModule, IN DWORD dwProcessId, IN LPOBJECTCONFIG lp, IN LPCSTR szFnName);

void * __stdcall shellcode_getkernel32modbase();

void * __stdcall shellcode_getntdllmodbase();