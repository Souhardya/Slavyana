#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"

typedef struct _PEB *PPEB;

//
// Basic Process Information
//  NtQueryInformationProcess using ProcessBasicInfo
//

typedef struct _PROCESS_BASIC_INFORMATION {
    NTSTATUS ExitStatus;
    PPEB PebBaseAddress;
    ULONG_PTR AffinityMask;
    KPRIORITY BasePriority;
    ULONG_PTR UniqueProcessId;
    ULONG_PTR InheritedFromUniqueProcessId;
} PROCESS_BASIC_INFORMATION;
typedef PROCESS_BASIC_INFORMATION *PPROCESS_BASIC_INFORMATION;

typedef enum _PROCESS_INFORMATION_CLASS {
	ProcessBasicInformation,
	ProcessQuotaLimits,
	ProcessIoCounters,
	ProcessVmCounters,
	ProcessTimes,
	ProcessBasePriority,
	ProcessRaisePriority,
	ProcessDebugPort,
	ProcessExceptionPort,
	ProcessAccessToken,
	ProcessLdtInformation,
	ProcessLdtSize,
	ProcessDefaultHardErrorMode,
	ProcessIoPortHandlers,
	ProcessPooledUsageAndLimits,
	ProcessWorkingSetWatch,
	ProcessUserModeIOPL,
	ProcessEnableAlignmentFaultFixup,
	ProcessPriorityClass,
	ProcessWx86Information,
	ProcessHandleCount,
	ProcessAffinityMask,
	ProcessPriorityBoost,
	MaxProcessInfoClass
} PROCESS_INFORMATION_CLASS, *PPROCESS_INFORMATION_CLASS;

typedef NTSTATUS (NTAPI * __pfnNtQueryInformationProcess)
(
	IN HANDLE               ProcessHandle,
	IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
	OUT PVOID               ProcessInformation,
	IN ULONG                ProcessInformationLength,
	OUT PULONG              ReturnLength );

extern __pfnNtQueryInformationProcess pfnNtQueryInformationProcess;

NTSTATUS
NTAPI
OnNtQueryInformationProcess(
	IN HANDLE               ProcessHandle,
	IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
	OUT PVOID               ProcessInformation,
	IN ULONG                ProcessInformationLength,
	OUT PULONG              ReturnLength );

NTSTATUS
NTAPI
NtQueryInformationProcess(
	IN HANDLE               ProcessHandle,
	IN PROCESS_INFORMATION_CLASS ProcessInformationClass,
	OUT PVOID               ProcessInformation,
	IN ULONG                ProcessInformationLength,
	OUT PULONG              ReturnLength );