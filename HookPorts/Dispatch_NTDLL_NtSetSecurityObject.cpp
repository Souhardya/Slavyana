#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtSetSecurityObject.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./Main.h"
#include "./Initalization.h"
#include "./GetProcAddressEx.h"



//
//Global
//
__pfnNtSetSecurityObject pfnNtSetSecurityObject = NULL;



//
//Dispatch_NTDLL_NtSetSecurityObject Functions
//
NTSTATUS
NTAPI
OnNtSetSecurityObject(
	IN HANDLE               ObjectHandle,
	IN SECURITY_INFORMATION SecurityInformationClass,
	IN PSECURITY_DESCRIPTOR DescriptorBuffer
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
		nRet = pfnNtSetSecurityObject(
			ObjectHandle,
			SecurityInformationClass,
			DescriptorBuffer
			);

		return nRet;
	}

	nRet = pfnNtSetSecurityObject(
		ObjectHandle,
		SecurityInformationClass,
		DescriptorBuffer
		);

	return nRet;
}

NTSTATUS
NTAPI
NtSetSecurityObject(
	IN HANDLE               ObjectHandle,
	IN SECURITY_INFORMATION SecurityInformationClass,
	IN PSECURITY_DESCRIPTOR DescriptorBuffer
	)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtSetSecurityObject addFun = (__pfnNtSetSecurityObject)GetProcAddress(hDll,"NtSetSecurityObject");
	if(addFun)
	{
		ret = addFun(ObjectHandle,SecurityInformationClass,DescriptorBuffer);
	}
	return ret;
}