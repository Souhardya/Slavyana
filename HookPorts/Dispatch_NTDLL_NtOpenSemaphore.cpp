#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenSemaphore.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./NativeAPI_NTDLL.h"



//
//Global
//
__pfnNtOpenSemaphore pfnNtOpenSemaphore = NULL;



//
//Dispatch_NTDLL_NtOpenSemaphore Functions
//
NTSTATUS
NTAPI
OnNtOpenSemaphore(
	OUT PHANDLE             SemaphoreHandle,
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
		nRet = pfnNtOpenSemaphore(
			SemaphoreHandle,
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
		nRet = pfnNtOpenSemaphore(
			SemaphoreHandle,
			DesiredAccess,
			ObjectAttributes
			);

		return nRet;
	}

	//
	//Dispatch
	//
	OBJECT_ATTRIBUTES oaObjAttrib = *(POBJECT_ATTRIBUTES)ObjectAttributes;

	WCHAR szSemaphoreName[MAX_NAME] = {NULL};
	if( ObjectAttributes->ObjectName->Buffer != NULL )
	{
		lstrcpy(szSemaphoreName,ObjectAttributes->ObjectName->Buffer);

		wsprintfW(szSemaphoreName,L"%s_%s",g_szSandBoxName,ObjectAttributes->ObjectName->Buffer);

		UNICODE_STRING usMutantName;

		//
		//Init UNICODE_STRING
		//
		RtlInitUnicodeString(&usMutantName,szSemaphoreName);

		oaObjAttrib.ObjectName->Length = usMutantName.Length;
		oaObjAttrib.ObjectName->MaximumLength = usMutantName.MaximumLength;
		oaObjAttrib.ObjectName->Buffer = usMutantName.Buffer;

#ifdef Dbg
		DebugLog(DbgInfo,L"ObjectAttributes->ObjectName->Buffer=");
		OutputDebugString(szSemaphoreName);
#endif

		nRet = pfnNtOpenSemaphore(
			SemaphoreHandle,
			DesiredAccess,
			&oaObjAttrib
			);

		return nRet;
	}

	nRet = pfnNtOpenSemaphore(
		SemaphoreHandle,
		DesiredAccess,
		ObjectAttributes
		);

	return nRet;
}

NTSTATUS
NTAPI
NtOpenSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenSemaphore addFun = (__pfnNtOpenSemaphore)GetProcAddress(hDll,"NtOpenSemaphore");
	if(addFun)
	{
		ret = addFun(SemaphoreHandle,DesiredAccess,ObjectAttributes);
	}
	return ret;
}