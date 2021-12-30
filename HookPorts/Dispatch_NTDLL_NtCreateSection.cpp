#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtCreateSection.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"



//
//Global
//
__pfnNtCreateSection pfnNtCreateSection = NULL;



//
//Dispatch_NTDLL_NtCreateSection Functions
//
NTSTATUS
NTAPI
OnNtCreateSection(
	OUT PHANDLE             SectionHandle,
	IN ULONG                DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN PLARGE_INTEGER       MaximumSize OPTIONAL,
	IN ULONG                PageAttributess,
	IN ULONG                SectionAttributes,
	IN HANDLE               FileHandle OPTIONAL)
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
		nRet = pfnNtCreateSection(
			SectionHandle,
			DesiredAccess,
			ObjectAttributes,
			MaximumSize,
			PageAttributess,
			SectionAttributes,
			FileHandle
			);

		return nRet;
	}

	nRet = pfnNtCreateSection(
		SectionHandle,
		DesiredAccess,
		ObjectAttributes,
		MaximumSize,
		PageAttributess,
		SectionAttributes,
		FileHandle
		);

	return nRet;
}

NTSTATUS
NTAPI
NtCreateSection(
	OUT PHANDLE             SectionHandle,
	IN ULONG                DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN PLARGE_INTEGER       MaximumSize OPTIONAL,
	IN ULONG                PageAttributess,
	IN ULONG                SectionAttributes,
	IN HANDLE               FileHandle OPTIONAL)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtCreateSection addFun = (__pfnNtCreateSection)GetProcAddress(hDll,"NtCreateSection");
	if(addFun)
	{
		ret = addFun(SectionHandle,DesiredAccess,ObjectAttributes,MaximumSize,PageAttributess,SectionAttributes,FileHandle);
	}
	return ret;
}