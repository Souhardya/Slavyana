#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL.h"
#include "./InlineHook.h"
#include "../Common/DebugLog.h"

#include "./Dispatch_NTDLL_LdrLoadDll.h"
#include "./Dispatch_NTDLL_LdrUnloadDll.h"
#include "./Dispatch_NTDLL_NtAdjustPrivilegesToken.h"
#include "./Dispatch_NTDLL_NtAllocateVirtualMemory.h"
#include "./Dispatch_NTDLL_NtClose.h"
#include "./Dispatch_NTDLL_NtConnectPort.h"
#include "./Dispatch_NTDLL_NtCreateEvent.h"
#include "./Dispatch_NTDLL_NtCreateFile.h"
#include "./Dispatch_NTDLL_NtCreateKey.h"
#include "./Dispatch_NTDLL_NtCreateMutant.h"
#include "./Dispatch_NTDLL_NtCreatePort.h"
#include "./Dispatch_NTDLL_NtCreateProcess.h"
#include "./Dispatch_NTDLL_NtCreateProcessEx.h"
#include "./Dispatch_NTDLL_NtCreateSection.h"
#include "./Dispatch_NTDLL_NtCreateSemaphore.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtDeleteFile.h"
#include "./Dispatch_NTDLL_NtDeleteKey.h"
#include "./Dispatch_NTDLL_NtDeleteValueKey.h"
#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "./Dispatch_NTDLL_NtEnumerateValueKey.h"
#include "./Dispatch_NTDLL_NtFsControlFile.h"
#include "./Dispatch_NTDLL_NtGetContextThread.h"
#include "./Dispatch_NTDLL_NtLoadDriver.h"
#include "./Dispatch_NTDLL_NtNotifyChangeKey.h"
#include "./Dispatch_NTDLL_NtNotifyChangeMultipleKeys.h"
#include "./Dispatch_NTDLL_NtOpenEvent.h"
#include "./Dispatch_NTDLL_NtOpenFile.h"
#include "./Dispatch_NTDLL_NtOpenKey.h"
#include "./Dispatch_NTDLL_NtOpenMutant.h"
#include "./Dispatch_NTDLL_NtOpenProcess.h"
#include "./Dispatch_NTDLL_NtOpenSection.h"
#include "./Dispatch_NTDLL_NtOpenSemaphore.h"
#include "./Dispatch_NTDLL_NtOpenThread.h"
#include "./Dispatch_NTDLL_NtProtectVirtualMemory.h"
#include "./Dispatch_NTDLL_NtQueryAttributesFile.h"
#include "./Dispatch_NTDLL_NtQueryDirectoryFile.h"
#include "./Dispatch_NTDLL_NtQueryFullAttributesFile.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"
#include "./Dispatch_NTDLL_NtQueryInformationProcess.h"
#include "./Dispatch_NTDLL_NtQueryInformationThread.h"
#include "./Dispatch_NTDLL_NtQueryKey.h"
#include "./Dispatch_NTDLL_NtQueryMultipleValueKey.h"
#include "./Dispatch_NTDLL_NtQueryObject.h"
#include "./Dispatch_NTDLL_NtQuerySystemInformation.h"
#include "./Dispatch_NTDLL_NtQueryValueKey.h"
#include "./Dispatch_NTDLL_NtReadFile.h"
#include "./Dispatch_NTDLL_NtRenameKey.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtSaveKey.h"
#include "./Dispatch_NTDLL_NtSecureConnectPort.h"
#include "./Dispatch_NTDLL_NtSetInformationFile.h"
#include "./Dispatch_NTDLL_NtSetInformationToken.h"
#include "./Dispatch_NTDLL_NtSetSecurityObject.h"
#include "./Dispatch_NTDLL_NtSetSystemInformation.h"
#include "./Dispatch_NTDLL_NtSetValueKey.h"
#include "./Dispatch_NTDLL_NtSystemDebugControl.h"
#include "./Dispatch_NTDLL_NtWriteFile.h"
#include "./Dispatch_NTDLL_NtWriteVirtualMemory.h"



//
//Global
//
BOOL bNTDLLPatched = FALSE;



//
//Dispatch_NTDLL Functions
//
int Dispatch_NTDLL_Start(void)
{
	//Return Value:
	//-1 = error
	//0 = patched
	//1 = succeed

	if( bNTDLLPatched == TRUE )
	{
		return 0;
	}

	HINSTANCE hNTDLL = NULL;
	hNTDLL = LoadLibrary(L"ntdll.dll");

	//
	//Patch API
	//

	//LdrLoadDll
	InlineHook(
		(__pfnLdrLoadDll)GetProcAddress(hNTDLL,"LdrLoadDll"),
		OnLdrLoadDll,
		(void **)&pfnLdrLoadDll
		);

	//LdrUnloadDll
	InlineHook(
		(__pfnLdrUnloadDll)GetProcAddress(hNTDLL,"LdrUnloadDll"),
		OnLdrUnloadDll,
		(void **)&pfnLdrUnloadDll
		);

	//NtAdjustPrivilegesToken
	InlineHook(
		(__pfnNtAdjustPrivilegesToken)GetProcAddress(hNTDLL,"NtAdjustPrivilegesToken"),
		OnNtAdjustPrivilegesToken,
		(void **)&pfnNtAdjustPrivilegesToken
		);

	////NtAllocateVirtualMemory
	//InlineHook(
	//	(__pfnNtAllocateVirtualMemory)GetProcAddress(hNTDLL,"NtAllocateVirtualMemory"),
	//	OnNtAllocateVirtualMemory,
	//	(void **)&pfnNtAllocateVirtualMemory
	//	);

	//NtClose
	InlineHook(
		(__pfnNtClose)GetProcAddress(hNTDLL,"NtClose"),
		OnNtClose,
		(void **)&pfnNtClose
		);

	//NtConnectPort
	InlineHook(
		(__pfnNtConnectPort)GetProcAddress(hNTDLL,"NtConnectPort"),
		OnNtConnectPort,
		(void **)&pfnNtConnectPort
		);

	//NtCreateEvent
	InlineHook(
		(__pfnNtCreateEvent)GetProcAddress(hNTDLL,"NtCreateEvent"),
		OnNtCreateEvent,
		(void **)&pfnNtCreateEvent
		);

	//NtCreateFile
	InlineHook(
		(__pfnNtCreateFile)GetProcAddress(hNTDLL,"NtCreateFile"),
		OnNtCreateFile,
		(void **)&pfnNtCreateFile
		);

	//NtCreateKey
	InlineHook(
		(__pfnNtCreateKey)GetProcAddress(hNTDLL,"NtCreateKey"),
		OnNtCreateKey,
		(void **)&pfnNtCreateKey
		);

	////NtCreateMutant
	//InlineHook(
	//	(__pfnNtCreateMutant)GetProcAddress(hNTDLL,"NtCreateMutant"),
	//	OnNtCreateMutant,
	//	(void **)&pfnNtCreateMutant
	//	);

	//NtCreatePort
	InlineHook(
		(__pfnNtCreatePort)GetProcAddress(hNTDLL,"NtCreatePort"),
		OnNtCreatePort,
		(void **)&pfnNtCreatePort
		);

	//NtCreateProcess
	InlineHook(
		(__pfnNtCreateProcess)GetProcAddress(hNTDLL,"NtCreateProcess"),
		OnNtCreateProcess,
		(void **)&pfnNtCreateProcess
		);

	//NtCreateProcessEx
	InlineHook(
		(__pfnNtCreateProcessEx)GetProcAddress(hNTDLL,"NtCreateProcessEx"),
		OnNtCreateProcessEx,
		(void **)&pfnNtCreateProcessEx
		);

	//NtCreateSection
	InlineHook(
		(__pfnNtCreateSection)GetProcAddress(hNTDLL,"NtCreateSection"),
		OnNtCreateSection,
		(void **)&pfnNtCreateSection
		);

	////NtCreateSemaphore
	//InlineHook(
	//	(__pfnNtCreateSemaphore)GetProcAddress(hNTDLL,"NtCreateSemaphore"),
	//	OnNtCreateSemaphore,
	//	(void **)&pfnNtCreateSemaphore
	//	);

	//NtCreateThread
	InlineHook(
		(__pfnNtCreateThread)GetProcAddress(hNTDLL,"NtCreateThread"),
		OnNtCreateThread,
		(void **)&pfnNtCreateThread
		);

	//NtDeleteFile
	InlineHook(
		(__pfnNtDeleteFile)GetProcAddress(hNTDLL,"NtDeleteFile"),
		OnNtDeleteFile,
		(void **)&pfnNtDeleteFile
		);

	//NtDeleteKey
	InlineHook(
		(__pfnNtDeleteKey)GetProcAddress(hNTDLL,"NtDeleteKey"),
		OnNtDeleteKey,
		(void **)&pfnNtDeleteKey
		);

	//NtDeleteValueKey
	InlineHook(
		(__pfnNtDeleteValueKey)GetProcAddress(hNTDLL,"NtDeleteValueKey"),
		OnNtDeleteValueKey,
		(void **)&pfnNtDeleteValueKey
		);

	//NtEnumerateKey
	InlineHook(
		(__pfnNtEnumerateKey)GetProcAddress(hNTDLL,"NtEnumerateKey"),
		OnNtEnumerateKey,
		(void **)&pfnNtEnumerateKey
		);

	//NtEnumerateValueKey
	InlineHook(
		(__pfnNtEnumerateValueKey)GetProcAddress(hNTDLL,"NtEnumerateValueKey"),
		OnNtEnumerateValueKey,
		(void **)&pfnNtEnumerateValueKey
		);

	//NtFsControlFile
	InlineHook(
		(__pfnNtFsControlFile)GetProcAddress(hNTDLL,"NtFsControlFile"),
		OnNtFsControlFile,
		(void **)&pfnNtFsControlFile
		);

	////NtGetContextThread
	//InlineHook(
	//	(__pfnNtGetContextThread)GetProcAddress(hNTDLL,"NtGetContextThread"),
	//	OnNtGetContextThread,
	//	(void **)&pfnNtGetContextThread
	//	);

	//NtLoadDriver
	InlineHook(
		(__pfnNtLoadDriver)GetProcAddress(hNTDLL,"NtLoadDriver"),
		OnNtLoadDriver,
		(void **)&pfnNtLoadDriver
		);

	//NtNotifyChangeKey
	InlineHook(
		(__pfnNtNotifyChangeKey)GetProcAddress(hNTDLL,"NtNotifyChangeKey"),
		OnNtNotifyChangeKey,
		(void **)&pfnNtNotifyChangeKey
		);

	//NtNotifyChangeMultipleKeys
	InlineHook(
		(__pfnNtNotifyChangeMultipleKeys)GetProcAddress(hNTDLL,"NtNotifyChangeMultipleKeys"),
		OnNtNotifyChangeMultipleKeys,
		(void **)&pfnNtNotifyChangeMultipleKeys
		);

	//NtOpenEvent
	InlineHook(
		(__pfnNtOpenEvent)GetProcAddress(hNTDLL,"NtOpenEvent"),
		OnNtOpenEvent,
		(void **)&pfnNtOpenEvent
		);

	//NtOpenFile
	InlineHook(
		(__pfnNtOpenFile)GetProcAddress(hNTDLL,"NtOpenFile"),
		OnNtOpenFile,
		(void **)&pfnNtOpenFile
		);

	//NtOpenKey
	InlineHook(
		(__pfnNtOpenKey)GetProcAddress(hNTDLL,"NtOpenKey"),
		OnNtOpenKey,
		(void **)&pfnNtOpenKey
		);

	////NtOpenMutant
	//InlineHook(
	//	(__pfnNtOpenMutant)GetProcAddress(hNTDLL,"NtOpenMutant"),
	//	OnNtOpenMutant,
	//	(void **)&pfnNtOpenMutant
	//	);

	//NtOpenProcess
	InlineHook(
		(__pfnNtOpenProcess)GetProcAddress(hNTDLL,"NtOpenProcess"),
		OnNtOpenProcess,
		(void **)&pfnNtOpenProcess
		);

	//NtOpenSection
	InlineHook(
		(__pfnNtOpenSection)GetProcAddress(hNTDLL,"NtOpenSection"),
		OnNtOpenSection,
		(void **)&pfnNtOpenSection
		);

	////NtOpenSemaphore
	//InlineHook(
	//	(__pfnNtOpenSemaphore)GetProcAddress(hNTDLL,"NtOpenSemaphore"),
	//	OnNtOpenSemaphore,
	//	(void **)&pfnNtOpenSemaphore
	//	);

	//NtOpenThread
	InlineHook(
		(__pfnNtOpenThread)GetProcAddress(hNTDLL,"NtOpenThread"),
		OnNtOpenThread,
		(void **)&pfnNtOpenThread
		);

	////NtProtectVirtualMemory
	//InlineHook(
	//	(__pfnNtProtectVirtualMemory)GetProcAddress(hNTDLL,"NtProtectVirtualMemory"),
	//	OnNtProtectVirtualMemory,
	//	(void **)&pfnNtProtectVirtualMemory
	//	);

	//NtQueryAttributesFile
	InlineHook(
		(__pfnNtQueryAttributesFile)GetProcAddress(hNTDLL,"NtQueryAttributesFile"),
		OnNtQueryAttributesFile,
		(void **)&pfnNtQueryAttributesFile
		);

	//NtQueryDirectoryFile
	InlineHook(
		(__pfnNtQueryDirectoryFile)GetProcAddress(hNTDLL,"NtQueryDirectoryFile"),
		OnNtQueryDirectoryFile,
		(void **)&pfnNtQueryDirectoryFile
		);

	//NtQueryFullAttributesFile
	InlineHook(
		(__pfnNtQueryFullAttributesFile)GetProcAddress(hNTDLL,"NtQueryFullAttributesFile"),
		OnNtQueryFullAttributesFile,
		(void **)&pfnNtQueryFullAttributesFile
		);

	//NtQueryInformationFile
	InlineHook(
		(__pfnNtQueryInformationFile)GetProcAddress(hNTDLL,"NtQueryInformationFile"),
		OnNtQueryInformationFile,
		(void **)&pfnNtQueryInformationFile
		);

	//NtQueryInformationProcess
	InlineHook(
		(__pfnNtQueryInformationProcess)GetProcAddress(hNTDLL,"NtQueryInformationProcess"),
		OnNtQueryInformationProcess,
		(void **)&pfnNtQueryInformationProcess
		);

	//NtQueryInformationThread
	InlineHook(
		(__pfnNtQueryInformationThread)GetProcAddress(hNTDLL,"NtQueryInformationThread"),
		OnNtQueryInformationThread,
		(void **)&pfnNtQueryInformationThread
		);

	//NtQueryKey
	InlineHook(
		(__pfnNtQueryKey)GetProcAddress(hNTDLL,"NtQueryKey"),
		OnNtQueryKey,
		(void **)&pfnNtQueryKey
		);

	//NtQueryMultipleValueKey
	InlineHook(
		(__pfnNtQueryMultipleValueKey)GetProcAddress(hNTDLL,"NtQueryMultipleValueKey"),
		OnNtQueryMultipleValueKey,
		(void **)&pfnNtQueryMultipleValueKey
		);

	//NtQueryObject
	InlineHook(
		(__pfnNtQueryObject)GetProcAddress(hNTDLL,"NtQueryObject"),
		OnNtQueryObject,
		(void **)&pfnNtQueryObject
		);

	//NtQuerySystemInformation
	InlineHook(
		(__pfnNtQuerySystemInformation)GetProcAddress(hNTDLL,"NtQuerySystemInformation"),
		OnNtQuerySystemInformation,
		(void **)&pfnNtQuerySystemInformation
		);

	//NtQueryValueKey
	InlineHook(
		(__pfnNtQueryValueKey)GetProcAddress(hNTDLL,"NtQueryValueKey"),
		OnNtQueryValueKey,
		(void **)&pfnNtQueryValueKey
		);

	//NtReadFile
	InlineHook(
		(__pfnNtReadFile)GetProcAddress(hNTDLL,"NtReadFile"),
		OnNtReadFile,
		(void **)&pfnNtReadFile
		);

	//NtRenameKey
	InlineHook(
		(__pfnNtRenameKey)GetProcAddress(hNTDLL,"NtRenameKey"),
		OnNtRenameKey,
		(void **)&pfnNtRenameKey
		);

	//NtResumeThread
	InlineHook(
		(__pfnNtResumeThread)GetProcAddress(hNTDLL,"NtResumeThread"),
		OnNtResumeThread,
		(void **)&pfnNtResumeThread
		);

	//NtSaveKey
	InlineHook(
		(__pfnNtSaveKey)GetProcAddress(hNTDLL,"NtSaveKey"),
		OnNtSaveKey,
		(void **)&pfnNtSaveKey
		);

	//NtSecureConnectPort
	InlineHook(
		(__pfnNtSecureConnectPort)GetProcAddress(hNTDLL,"NtSecureConnectPort"),
		OnNtSecureConnectPort,
		(void **)&pfnNtSecureConnectPort
		);

	//NtSetInformationFile
	InlineHook(
		(__pfnNtSetInformationFile)GetProcAddress(hNTDLL,"NtSetInformationFile"),
		OnNtSetInformationFile,
		(void **)&pfnNtSetInformationFile
		);

	//NtSetInformationToken
	InlineHook(
		(__pfnNtSetInformationToken)GetProcAddress(hNTDLL,"NtSetInformationToken"),
		OnNtSetInformationToken,
		(void **)&pfnNtSetInformationToken
		);

	//NtSetSecurityObject
	InlineHook(
		(__pfnNtSetSecurityObject)GetProcAddress(hNTDLL,"NtSetSecurityObject"),
		OnNtSetSecurityObject,
		(void **)&pfnNtSetSecurityObject
		);

	//NtSetSystemInformation
	InlineHook(
		(__pfnNtSetSystemInformation)GetProcAddress(hNTDLL,"NtSetSystemInformation"),
		OnNtSetSystemInformation,
		(void **)&pfnNtSetSystemInformation
		);

	//NtSetValueKey
	InlineHook(
		(__pfnNtSetValueKey)GetProcAddress(hNTDLL,"NtSetValueKey"),
		OnNtSetValueKey,
		(void **)&pfnNtSetValueKey
		);

	//NtSystemDebugControl
	InlineHook(
		(__pfnNtSystemDebugControl)GetProcAddress(hNTDLL,"NtSystemDebugControl"),
		OnNtSystemDebugControl,
		(void **)&pfnNtSystemDebugControl
		);

	//NtWriteFile
	InlineHook(
		(__pfnNtWriteFile)GetProcAddress(hNTDLL,"NtWriteFile"),
		OnNtWriteFile,
		(void **)&pfnNtWriteFile
		);

	//NtWriteVirtualMemory
	InlineHook(
		(__pfnNtWriteVirtualMemory)GetProcAddress(hNTDLL,"NtWriteVirtualMemory"),
		OnNtWriteVirtualMemory,
		(void **)&pfnNtWriteVirtualMemory
		);

	return 1;
}