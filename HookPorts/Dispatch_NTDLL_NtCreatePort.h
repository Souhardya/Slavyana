#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtCreatePort)
(
	OUT PHANDLE             PortHandle,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN ULONG                MaxConnectInfoLength,
	IN ULONG                MaxDataLength,
	IN OUT PULONG           Reserved OPTIONAL
	);

extern __pfnNtCreatePort pfnNtCreatePort;

NTSTATUS
NTAPI
OnNtCreatePort(
	OUT PHANDLE             PortHandle,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN ULONG                MaxConnectInfoLength,
	IN ULONG                MaxDataLength,
	IN OUT PULONG           Reserved OPTIONAL
	);
NTSTATUS
NTAPI
NtCreatePort(
	OUT PHANDLE             PortHandle,
	IN POBJECT_ATTRIBUTES   ObjectAttributes,
	IN ULONG                MaxConnectInfoLength,
	IN ULONG                MaxDataLength,
	IN OUT PULONG           Reserved OPTIONAL
	);