#include <windows.h>
#include <ntsecapi.h>

#include "./NativeAPI_NTDLL.h"
#include "./Main.h"



//
//NativeAPI_NTDLL Functions
//
NTSTATUS
NTAPI
RtlInitUnicodeString(
	PUNICODE_STRING DestinationString,
	PCWSTR SourceString
	)
{
	NTSTATUS ret = NULL;
	typedef NTSTATUS (WINAPI *lpAddFun)(PUNICODE_STRING,PCWSTR);
	HINSTANCE hDll = GetModuleHandleW( L"ntdll.dll" );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RtlInitUnicodeString");
	if( addFun != NULL )
	{
		ret = addFun(DestinationString,SourceString);
	}
	return ret;
}

NTSTATUS
NTAPI
RtlFreeUnicodeString(
	IN PUNICODE_STRING UnicodeString
	)
{
	NTSTATUS ret = NULL;
	typedef NTSTATUS (WINAPI *lpAddFun)(PUNICODE_STRING);
	HINSTANCE hDll = GetModuleHandleW( L"ntdll.dll" );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RtlFreeUnicodeString");
	if( addFun != NULL )
	{
		ret = addFun(UnicodeString);
	}
	return ret;
}

NTSTATUS
NTAPI
RtlDosPathNameToNtPathName_U(
	IN PCWSTR DosPathName,
	OUT PUNICODE_STRING NtPathName,
	OUT PCWSTR *NtFileNamePart,
	OUT CURDIR *DirectoryInfo
	)
{
	NTSTATUS ret = NULL;
	typedef NTSTATUS (WINAPI *lpAddFun)(PCWSTR,PUNICODE_STRING,PCWSTR *,CURDIR *);
	HINSTANCE hDll = GetModuleHandleW( L"ntdll.dll" );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"RtlDosPathNameToNtPathName_U");
	if( addFun != NULL )
	{
		ret = addFun(DosPathName,NtPathName,NtFileNamePart,DirectoryInfo);
	}
	return ret;
}