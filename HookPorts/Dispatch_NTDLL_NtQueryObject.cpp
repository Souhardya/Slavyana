#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"



//
//Global
//
__pfnNtQueryObject pfnNtQueryObject = NULL;



//
//Dispatch_NTDLL_NtQueryObject Functions
//
NTSTATUS
NTAPI
OnNtQueryObject(
	IN HANDLE               ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID               ObjectInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength)
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
		nRet = pfnNtQueryObject(
			ObjectHandle,
			ObjectInformationClass,
			ObjectInformation,
			Length,
			ResultLength
			);

		return nRet;
	}

	nRet = pfnNtQueryObject(
		ObjectHandle,
		ObjectInformationClass,
		ObjectInformation,
		Length,
		ResultLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryObject(
	IN HANDLE               ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID               ObjectInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryObject addFun = (__pfnNtQueryObject)GetProcAddress(hDll,"NtQueryObject");
	if(addFun)
	{
		ret = addFun(ObjectHandle,ObjectInformationClass,ObjectInformation,Length,ResultLength);
	}
	return ret;
}

int QueryNameInfoByNtQueryObject( IN HANDLE ObjectHandle, OUT WCHAR * lpszNameInfo )
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = -1;

	//
	//Query info by call NtQueryObject
	//
	ULONG uResultLength = 0;
	POBJECT_NAME_INFORMATION pNameInfo = (POBJECT_NAME_INFORMATION)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, 0x1000);   

	NTSTATUS nStatusQueryObject;
	nStatusQueryObject = NtQueryObject(ObjectHandle, ObjectNameInformation, pNameInfo, 0x1000, &uResultLength);
	DWORD iQueryCount = 1;
	while( nStatusQueryObject == STATUS_INFO_LENGTH_MISMATCH )
	{
		pNameInfo = (POBJECT_NAME_INFORMATION)HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, pNameInfo, 0x1000 * iQueryCount);
		nStatusQueryObject = NtQueryObject(ObjectHandle, ObjectNameInformation, pNameInfo, 0x1000, NULL);
		iQueryCount++;
	}
	if( nStatusQueryObject == STATUS_SUCCESS )
	{
		lstrcpyW(lpszNameInfo,pNameInfo->Name.Buffer);

		iRet = 0;
	}
	HeapFree(GetProcessHeap(),0,pNameInfo);

	return iRet;
}