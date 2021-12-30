#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenMutant.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./NativeAPI_NTDLL.h"



//
//Global
//
__pfnNtOpenMutant pfnNtOpenMutant = NULL;



//
//Dispatch_NTDLL_NtOpenMutant Functions
//
NTSTATUS
NTAPI
OnNtOpenMutant(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes)
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtOpenMutant(
			MutantHandle,
			DesiredAccess,
			ObjectAttributes
			);

		return nRet;
	}

	//
	//Check if Address Valid
	//
	if( IsBadCodePtr((FARPROC)ObjectAttributes) ||
		IsBadCodePtr((FARPROC)ObjectAttributes->ObjectName) ||
		IsBadCodePtr((FARPROC)ObjectAttributes->ObjectName->Buffer)
		)
	{
		nRet = pfnNtOpenMutant(
			MutantHandle,
			DesiredAccess,
			ObjectAttributes
			);

		return nRet;
	}

	//
	//Dispatch
	//
	OBJECT_ATTRIBUTES oaObjAttrib = *(POBJECT_ATTRIBUTES)ObjectAttributes;

	WCHAR szMutantName[MAX_NAME] = {NULL};
	if( ObjectAttributes->ObjectName->Buffer != NULL )
	{
		lstrcpy(szMutantName,ObjectAttributes->ObjectName->Buffer);

		wsprintfW(szMutantName,L"%s_%s",g_szSandBoxName,ObjectAttributes->ObjectName->Buffer);

		UNICODE_STRING usMutantName;

		//
		//Init UNICODE_STRING
		//
		RtlInitUnicodeString(&usMutantName,szMutantName);

		oaObjAttrib.ObjectName->Length = usMutantName.Length;
		oaObjAttrib.ObjectName->MaximumLength = usMutantName.MaximumLength;
		oaObjAttrib.ObjectName->Buffer = usMutantName.Buffer;

#ifdef Dbg
		DebugLog(DbgInfo,L"ObjectAttributes->ObjectName->Buffer=");
		OutputDebugString(szMutantName);
#endif

		nRet = pfnNtOpenMutant(
			MutantHandle,
			DesiredAccess,
			&oaObjAttrib
			);

		return nRet;
	}

	nRet = pfnNtOpenMutant(
		MutantHandle,
		DesiredAccess,
		ObjectAttributes
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenMutant(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenMutant addFun = (__pfnNtOpenMutant)GetProcAddress(hDll,"NtOpenMutant");
	if(addFun)
	{
		ret = addFun(MutantHandle,DesiredAccess,ObjectAttributes);
	}
	return ret;
}