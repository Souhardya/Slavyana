#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"
#include "./Dispatch_NTDLL_NtQueryInformationFile.h"

typedef enum _OBJECT_INFORMATION_CLASS {
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllInformation,
	ObjectDataInformation
} OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

typedef NTSTATUS (NTAPI * __pfnNtQueryObject)
(
	IN HANDLE               ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID               ObjectInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength);

extern __pfnNtQueryObject pfnNtQueryObject;

NTSTATUS
NTAPI
OnNtQueryObject(
	IN HANDLE               ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID               ObjectInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength);

NTSTATUS
NTAPI
NtQueryObject(
	IN HANDLE               ObjectHandle,
	IN OBJECT_INFORMATION_CLASS ObjectInformationClass,
	OUT PVOID               ObjectInformation,
	IN ULONG                Length,
	OUT PULONG              ResultLength);

int QueryNameInfoByNtQueryObject( IN HANDLE ObjectHandle, OUT WCHAR * lpszNameInfo );