#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateMutant.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./NativeAPI_NTDLL.h"



//
//Global
//
__pfnNtCreateMutant pfnNtCreateMutant = NULL;



//
//Dispatch_NTDLL_NtCreateMutant Functions
//
NTSTATUS
NTAPI
OnNtCreateMutant(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN BOOLEAN              InitialOwner)
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
		nRet = pfnNtCreateMutant(
			MutantHandle,
			DesiredAccess,
			ObjectAttributes,
			InitialOwner
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
		nRet = pfnNtCreateMutant(
			MutantHandle,
			DesiredAccess,
			ObjectAttributes,
			InitialOwner
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

		nRet = pfnNtCreateMutant(
			MutantHandle,
			DesiredAccess,
			&oaObjAttrib,
			InitialOwner
			);

		return nRet;
	}

	nRet = pfnNtCreateMutant(
		MutantHandle,
		DesiredAccess,
		ObjectAttributes,
		InitialOwner
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateMutant(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN BOOLEAN              InitialOwner)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateMutant addFun = (__pfnNtCreateMutant)GetProcAddress(hDll,"NtCreateMutant");
	if(addFun)
	{
		ret = addFun(MutantHandle,DesiredAccess,ObjectAttributes,InitialOwner);
	}
	return ret;
}