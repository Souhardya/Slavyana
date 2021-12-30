#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtOpenProcess.h"

#define THREAD_TERMINATE               (0x0001)  // winnt
// end_ntddk end_wdm end_ntifs
#define THREAD_SUSPEND_RESUME          (0x0002)  // winnt
#define THREAD_ALERT                   (0x0004)
#define THREAD_GET_CONTEXT             (0x0008)  // winnt
#define THREAD_SET_CONTEXT             (0x0010)  // winnt
// begin_ntddk begin_wdm begin_ntifs
#define THREAD_SET_INFORMATION         (0x0020)  // winnt
// end_ntddk end_wdm end_ntifs
#define THREAD_QUERY_INFORMATION       (0x0040)  // winnt
// begin_winnt
#define THREAD_SET_THREAD_TOKEN        (0x0080)
#define THREAD_IMPERSONATE             (0x0100)
#define THREAD_DIRECT_IMPERSONATION    (0x0200)

typedef NTSTATUS (NTAPI * __pfnNtOpenThread)
(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId );

extern __pfnNtOpenThread pfnNtOpenThread;

NTSTATUS
NTAPI
OnNtOpenThread(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId );

NTSTATUS
NTAPI
NtOpenThread(
	OUT PHANDLE             ThreadHandle,
	IN ACCESS_MASK          AccessMask,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN PCLIENT_ID           ClientId );