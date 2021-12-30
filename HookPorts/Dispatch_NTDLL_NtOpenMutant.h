#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtOpenMutant)
(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

extern __pfnNtOpenMutant pfnNtOpenMutant;

NTSTATUS
NTAPI
OnNtOpenMutant(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

NTSTATUS
NTAPI
NtOpenMutant(
	OUT PHANDLE             MutantHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);