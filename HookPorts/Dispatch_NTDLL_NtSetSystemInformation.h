#pragma once

#include "./Dispatch_NTDLL_NtSetSystemInformation.h"
#include "./Dispatch_NTDLL_NtQuerySystemInformation.h"

typedef NTSTATUS (NTAPI * __pfnNtSetSystemInformation)
(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN PVOID                SystemInformation,
	IN ULONG                SystemInformationLength );

extern __pfnNtSetSystemInformation pfnNtSetSystemInformation;

NTSTATUS
NTAPI
OnNtSetSystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN PVOID                SystemInformation,
	IN ULONG                SystemInformationLength );

NTSTATUS
NTAPI
NtSetSystemInformation(
	IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
	IN PVOID                SystemInformation,
	IN ULONG                SystemInformationLength );