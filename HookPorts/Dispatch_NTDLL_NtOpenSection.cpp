#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtOpenSection.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtOpenSection pfnNtOpenSection = NULL;



//
//Dispatch_NTDLL_NtOpenSection Functions
//
NTSTATUS
NTAPI
OnNtOpenSection(
    OUT PHANDLE  SectionHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes
    )
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
		nRet = pfnNtOpenSection(
			SectionHandle,
			DesiredAccess,
			ObjectAttributes
			);

		return nRet;
	}

	//
	//Check DesiredAccess
	//
	//if(DesiredAccess & (SECTION_MAP_WRITE))
	//{
	//	return STATUS_ACCESS_DENIED;
	//}

	nRet = pfnNtOpenSection(
		SectionHandle,
		DesiredAccess,
		ObjectAttributes
		);

#ifdef Dbg
	//WCHAR szDebugString[512] = {0};
	//wsprintf(
	//	szDebugString,
	//	L"NtOpenSection ObjectName=[%s]",
	//	ObjectAttributes->ObjectName->Buffer
	//	);
	//DebugLog(DbgInfo,L"test");
#endif

	return nRet;
}

NTSTATUS
NTAPI
NtOpenSection(
    OUT PHANDLE  SectionHandle,
    IN ACCESS_MASK  DesiredAccess,
    IN POBJECT_ATTRIBUTES  ObjectAttributes
    )
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtOpenSection addFun = (__pfnNtOpenSection)GetProcAddress(hDll,"NtOpenSection");
	if(addFun)
	{
		ret = addFun(SectionHandle,DesiredAccess,ObjectAttributes);
	}
	return ret;
}