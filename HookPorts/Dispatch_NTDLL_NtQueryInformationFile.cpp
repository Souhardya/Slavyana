#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Global
//
__pfnNtQueryInformationFile pfnNtQueryInformationFile = NULL;



//
//Dispatch_NTDLL_NtQueryInformationFile Functions
//
NTSTATUS
NTAPI
OnNtQueryInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass)
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
		nRet = pfnNtQueryInformationFile(
			FileHandle,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass
			);

		return nRet;
	}

	nRet = pfnNtQueryInformationFile(
		FileHandle,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass
		);

	return nRet;
}

NTSTATUS
NTAPI
NtQueryInformationFile(
    IN HANDLE FileHandle,
    OUT PIO_STATUS_BLOCK IoStatusBlock,
    OUT PVOID FileInformation,
    IN ULONG Length,
    IN FILE_INFORMATION_CLASS FileInformationClass)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryInformationFile addFun = (__pfnNtQueryInformationFile)GetProcAddress(hDll,"NtQueryInformationFile");
	if(addFun)
	{
		ret = addFun(FileHandle,IoStatusBlock,FileInformation,Length,FileInformationClass);
	}
	return ret;
}

int QueryFileNameByNtQueryInformationFile( IN HANDLE FileHandle, OUT WCHAR * lpszFileName )
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = -1;

	//
	//Query info by call NtQueryInformationFile
	//
	NTSTATUS nStatus;
    IO_STATUS_BLOCK IoStatus = {NULL};
	WCHAR szFileNameInfo[MAX_NAME] = {NULL};
	PFILE_NAME_INFORMATION szFileName = (PFILE_NAME_INFORMATION)szFileNameInfo;

    nStatus = NtQueryInformationFile(
		FileHandle,
		&IoStatus,
		szFileName,
		sizeof(szFileNameInfo),
		FileNameInformation
		);
    if(nStatus == STATUS_SUCCESS)
	{
		lstrcpyW(lpszFileName,szFileName->FileName);

		iRet = 0;
	}

	return iRet;
}