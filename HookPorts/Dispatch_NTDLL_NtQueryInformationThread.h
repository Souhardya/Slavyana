#pragma once

typedef enum _THREAD_INFORMATION_CLASS {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;


typedef NTSTATUS (NTAPI * __pfnNtQueryInformationThread)
(
	IN HANDLE               ThreadHandle,
	IN THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID               ThreadInformation,
	IN ULONG                ThreadInformationLength,
	OUT PULONG              ReturnLength OPTIONAL );

extern __pfnNtQueryInformationThread pfnNtQueryInformationThread;

NTSTATUS
NTAPI
OnNtQueryInformationThread(
	IN HANDLE               ThreadHandle,
	IN THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID               ThreadInformation,
	IN ULONG                ThreadInformationLength,
	OUT PULONG              ReturnLength OPTIONAL );

NTSTATUS
NTAPI
NtQueryInformationThread(
	IN HANDLE               ThreadHandle,
	IN THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID               ThreadInformation,
	IN ULONG                ThreadInformationLength,
	OUT PULONG              ReturnLength OPTIONAL );