#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateSemaphore.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./NativeAPI_NTDLL.h"



//
//Global
//
__pfnNtCreateSemaphore pfnNtCreateSemaphore = NULL;



//
//Dispatch_NTDLL_NtCreateSemaphore Functions
//
NTSTATUS
NTAPI
OnNtCreateSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN ULONG                InitialCount,
	IN ULONG                MaximumCount)
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
		nRet = pfnNtCreateSemaphore(
			SemaphoreHandle,
			DesiredAccess,
			ObjectAttributes,
			InitialCount,
			MaximumCount
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
		nRet = pfnNtCreateSemaphore(
			SemaphoreHandle,
			DesiredAccess,
			ObjectAttributes,
			InitialCount,
			MaximumCount
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

		nRet = pfnNtCreateSemaphore(
			SemaphoreHandle,
			DesiredAccess,
			&oaObjAttrib,
			InitialCount,
			MaximumCount
			);

		return nRet;
	}

	nRet = pfnNtCreateSemaphore(
		SemaphoreHandle,
		DesiredAccess,
		ObjectAttributes,
		InitialCount,
		MaximumCount
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateSemaphore(
	OUT PHANDLE             SemaphoreHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN ULONG                InitialCount,
	IN ULONG                MaximumCount)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateSemaphore addFun = (__pfnNtCreateSemaphore)GetProcAddress(hDll,"NtCreateSemaphore");
	if(addFun)
	{
		ret = addFun(SemaphoreHandle,DesiredAccess,ObjectAttributes,InitialCount,MaximumCount);
	}
	return ret;
}