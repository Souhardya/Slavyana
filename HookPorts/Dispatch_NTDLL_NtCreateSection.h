#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtCreateSection)
(
	OUT PHANDLE             SectionHandle,
	IN ULONG                DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN PLARGE_INTEGER       MaximumSize OPTIONAL,
	IN ULONG                PageAttributess,
	IN ULONG                SectionAttributes,
	IN HANDLE               FileHandle OPTIONAL);

extern __pfnNtCreateSection pfnNtCreateSection;

NTSTATUS
NTAPI
OnNtCreateSection(
	OUT PHANDLE             SectionHandle,
	IN ULONG                DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN PLARGE_INTEGER       MaximumSize OPTIONAL,
	IN ULONG                PageAttributess,
	IN ULONG                SectionAttributes,
	IN HANDLE               FileHandle OPTIONAL);

NTSTATUS
NTAPI
NtCreateSection(
	OUT PHANDLE             SectionHandle,
	IN ULONG                DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN PLARGE_INTEGER       MaximumSize OPTIONAL,
	IN ULONG                PageAttributess,
	IN ULONG                SectionAttributes,
	IN HANDLE               FileHandle OPTIONAL);