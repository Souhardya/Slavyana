#include <windows.h>
#include <ntsecapi.h>
#include <winbase.h>
#include <stdio.h>

#include "./Dispatch_NTDLL_NtQueryDirectoryFile.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./HookHelp_File.h"
#include "./GetProcAddressEx.h"
#include "../Config/Config.h"
#include "./Main.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"



//
//Global
//
__pfnNtQueryDirectoryFile pfnNtQueryDirectoryFile = NULL;



//
//Dispatch_NTDLL_NtQueryDirectoryFile Functions
//
NTSTATUS
NTAPI
OnNtQueryDirectoryFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               FileInformation,
	IN ULONG                Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN              ReturnSingleEntry,
	IN PUNICODE_STRING      FileMask OPTIONAL,
	IN BOOLEAN              RestartScan)
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
		nRet = pfnNtQueryDirectoryFile(
			FileHandle,
			Event,
			ApcRoutine,
			ApcContext,
			IoStatusBlock,
			FileInformation,
			Length,
			FileInformationClass,
			ReturnSingleEntry,
			FileMask,
			RestartScan
			);

		return nRet;
	}

	nRet = pfnNtQueryDirectoryFile(
		FileHandle,
		Event,
		ApcRoutine,
		ApcContext,
		IoStatusBlock,
		FileInformation,
		Length,
		FileInformationClass,
		ReturnSingleEntry,
		FileMask,
		RestartScan
		);

	//
	//Check if is succeed or [STATUS_NO_MORE_FILES]
	//
	if( nRet < 0 ||
		nRet == STATUS_NO_MORE_FILES
		)
	{
		return nRet;
	}

	//
	//Check if only a single entry
	//
	if( ReturnSingleEntry == TRUE )
	{
		return nRet;
	}

//#ifdef Dbg
//	WCHAR szDebugLine[512] = {0};
//	wsprintf(
//		szDebugLine,
//		L"NtQueryDirFile nRet=[0x%08X] Length=[%d] FileInformationClass=[%d] IoStatusBlock->Information=[%d] ReturnSingleEntry=[%d] RestartScan=[%d]",
//		nRet,
//		Length,
//		FileInformationClass,
//		IoStatusBlock->Information,
//		ReturnSingleEntry,
//		RestartScan
//		);
//	DebugLog(DbgInfo,szDebugLine);
//#endif

	//
	//Check if is a file request
	//
	ULONG NameOffset;
	ULONG NameSizeOffset;

	WCHAR *FileNamePtr;
	PULONG FileNameLength;

	PBYTE Curr,Prev;
	PULONG DeltaCurr, DeltaPrev;

	Curr = (PBYTE)FileInformation;

	switch( FileInformationClass )
	{
		case FileDirectoryInformation:
			{
				NameOffset = ((ULONG)&(((PFILE_DIRECTORY_INFORMATION)Curr)->FileName)) - ((ULONG)Curr);
				NameSizeOffset = ((ULONG)&(((PFILE_DIRECTORY_INFORMATION)Curr)->FileNameLength)) - ((ULONG)Curr);
			}break;

		case FileFullDirectoryInformation:
			{
				NameOffset = ((ULONG)&(((PFILE_FULL_DIR_INFORMATION)Curr)->FileName)) - ((ULONG)Curr);
				NameSizeOffset = ((ULONG)&(((PFILE_FULL_DIR_INFORMATION)Curr)->FileNameLength))	- ((ULONG)Curr);
			}break;

		case FileBothDirectoryInformation:
			{
				NameOffset = ((ULONG)&(((PFILE_BOTH_DIR_INFORMATION)Curr)->FileName)) - ((ULONG)Curr);
				NameSizeOffset = ((ULONG)&(((PFILE_BOTH_DIR_INFORMATION)Curr)->FileNameLength))	- ((ULONG)Curr);
			}break;

		case FileNamesInformation:
			{
				NameOffset = ((ULONG)&(((PFILE_NAMES_INFORMATION)Curr)->FileName)) - ((ULONG)Curr);
				NameSizeOffset = ((ULONG)&(((PFILE_NAMES_INFORMATION)Curr)->FileNameLength)) - ((ULONG)Curr);
			}break;

		case FileIdBothDirectoryInformation:
			{
				NameOffset = ((ULONG)&(((PFILE_ID_BOTH_DIR_INFORMATION)Curr)->FileName)) - ((ULONG)Curr);
				NameSizeOffset = ((ULONG)&(((PFILE_ID_BOTH_DIR_INFORMATION)Curr)->FileNameLength)) - ((ULONG)Curr);
			}break;

		case FileIdFullDirectoryInformation:
			{
				NameOffset = ((ULONG)&(((PFILE_ID_FULL_DIR_INFORMATION)Curr)->FileName)) - ((ULONG)Curr);
				NameSizeOffset = ((ULONG)&(((PFILE_ID_FULL_DIR_INFORMATION)Curr)->FileNameLength)) - ((ULONG)Curr);
			}break;

		default:
			{
				return nRet;
			}
	}

	//
	//Get Directory Path
	//
	//(\\??\\C:\\WINDOWS\\XXX)
	WCHAR szLongNtPath[MAX_NAME] = {NULL};
	//(\\??\\C:\\SandBox\\DefaultBox\\Drive\\C\\WINDOWS\\XXX)
	WCHAR szDispatchNtPath[MAX_NAME] = {NULL};

	GetLongNtPathName(FileHandle,NULL,NULL,g_szRestoreNtPath,szLongNtPath,szDispatchNtPath);

	int iFileToHide = 0;

	int iDispatchPathExist = -1;
	iDispatchPathExist = NtPathExist(szDispatchNtPath);
	if( iDispatchPathExist == 0 )//FILE_DOES_NOT_EXIST
	{
#ifdef Dbg
		DebugLog(DbgInfo,L"Dispatch path does not exist!");
#endif

		return nRet;
	}
	else
	if( iDispatchPathExist == 1 )//FILE_EXISTS
	{
		iFileToHide = GetDirectoryMarkAsDeletedFileCount(szDispatchNtPath);

//#ifdef Dbg
//		WCHAR szDebugLine[MAX_NAME] = {0};
//		swprintf(
//			szDebugLine,
//			L"\r\niDeletedFileCount = [%d]",
//			iFileToHide
//			);
//		OutputDebugString(szDebugLine);
//#endif
	}

	//
	//Fake the data
	//
	int i;
	for(i=0; i<iFileToHide; i++)
	{
		Curr = (PBYTE)FileInformation;
		Prev = NULL;

		while( Curr )
		{
			//Get the NextEntryOffsets
			DeltaCurr = (PULONG)Curr;
			DeltaPrev = (PULONG)Prev;

			FileNamePtr = (WCHAR*)((PBYTE)Curr + NameOffset);
			FileNameLength = (PULONG)((PBYTE)Curr + NameSizeOffset);

//#ifdef Dbg
//			WCHAR szDebugLine[MAX_NAME] = {0};
//			swprintf(
//				szDebugLine,
//				L"\r\nFileNamePtr = [%s]",
//				FileNamePtr
//				);
//			OutputDebugString(szDebugLine);
//#endif

			if( FileNamePtr != NULL &&
				GetFileDeletedMark(szDispatchNtPath,FileNamePtr,*FileNameLength) == 1
				)
			{
#ifdef Dbg
				WCHAR szDebugString[MAX_NAME] = {0};
				wsprintf(
					szDebugString,
					L"HideFileName = [%s]",
					FileNamePtr
					);
				DebugLog(DbgInfo,szDebugString);
#endif

				//File to hide
				if( Prev )
				{
					if( *DeltaCurr )
					{
						//NextEntryOffset of the Previous entry "points" to the next
						*DeltaPrev += *DeltaCurr;
					}
					else
					{
						//Is last file
						*DeltaPrev = 0;
					}
				}//Prev
				else
				{
					//Is first of the list
					if( *DeltaCurr )
					{
						//There's a next entry
						//*(PBYTE)FileInformation += *DeltaCurr;
						FileInformation = (PBYTE)((PBYTE)FileInformation + *DeltaCurr);
					}
					else
					{
						//No file
						FileInformation = NULL;
					}
				}
			}
			else
			{
				//Ff Curr is not to hide, Prev becomes Curr
				Prev = Curr;
			}

			if( *DeltaCurr )
			{
				Curr += *DeltaCurr;
			}
			else
			{
				Curr = NULL;
			}
		}//while
	}//for

	return nRet;
}

NTSTATUS
NTAPI
NtQueryDirectoryFile(
	IN HANDLE               FileHandle,
	IN HANDLE               Event OPTIONAL,
	IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
	IN PVOID                ApcContext OPTIONAL,
	OUT PIO_STATUS_BLOCK    IoStatusBlock,
	OUT PVOID               FileInformation,
	IN ULONG                Length,
	IN FILE_INFORMATION_CLASS FileInformationClass,
	IN BOOLEAN              ReturnSingleEntry,
	IN PUNICODE_STRING      FileMask OPTIONAL,
	IN BOOLEAN              RestartScan)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtQueryDirectoryFile addFun = (__pfnNtQueryDirectoryFile)GetProcAddress(hDll,"NtQueryDirectoryFile");
	if(addFun)
	{
		ret = addFun(FileHandle,Event,ApcRoutine,ApcContext,IoStatusBlock,FileInformation,Length,FileInformationClass,ReturnSingleEntry,FileMask,RestartScan);
	}
	return ret;
}