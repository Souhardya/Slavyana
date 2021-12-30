#pragma once

typedef struct _LPC_SECTION_OWNER_MEMORY {
	ULONG                   Length;
	HANDLE                  SectionHandle;
	ULONG                   OffsetInSection;
	ULONG                   ViewSize;
	PVOID                   ViewBase;
	PVOID                   OtherSideViewBase;
} LPC_SECTION_OWNER_MEMORY, *PLPC_SECTION_OWNER_MEMORY;

typedef struct _LPC_SECTION_MEMORY {
  ULONG                   Length;
  ULONG                   ViewSize;
  PVOID                   ViewBase;
} LPC_SECTION_MEMORY, *PLPC_SECTION_MEMORY;

typedef NTSTATUS (NTAPI * __pfnNtConnectPort)
(
	OUT PHANDLE             ClientPortHandle,
	IN PUNICODE_STRING      ServerPortName,
	IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	IN OUT PLPC_SECTION_OWNER_MEMORY ClientSharedMemory OPTIONAL,
	OUT PLPC_SECTION_MEMORY ServerSharedMemory OPTIONAL,
	OUT PULONG              MaximumMessageLength OPTIONAL,
	IN PVOID                ConnectionInfo OPTIONAL,
	IN PULONG               ConnectionInfoLength OPTIONAL
	);

extern __pfnNtConnectPort pfnNtConnectPort;

NTSTATUS
NTAPI
OnNtConnectPort(
	OUT PHANDLE             ClientPortHandle,
	IN PUNICODE_STRING      ServerPortName,
	IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	IN OUT PLPC_SECTION_OWNER_MEMORY ClientSharedMemory OPTIONAL,
	OUT PLPC_SECTION_MEMORY ServerSharedMemory OPTIONAL,
	OUT PULONG              MaximumMessageLength OPTIONAL,
	IN PVOID                ConnectionInfo OPTIONAL,
	IN PULONG               ConnectionInfoLength OPTIONAL
	);
NTSTATUS
NTAPI
NtConnectPort(
	OUT PHANDLE             ClientPortHandle,
	IN PUNICODE_STRING      ServerPortName,
	IN PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	IN OUT PLPC_SECTION_OWNER_MEMORY ClientSharedMemory OPTIONAL,
	OUT PLPC_SECTION_MEMORY ServerSharedMemory OPTIONAL,
	OUT PULONG              MaximumMessageLength OPTIONAL,
	IN PVOID                ConnectionInfo OPTIONAL,
	IN PULONG               ConnectionInfoLength OPTIONAL
	);