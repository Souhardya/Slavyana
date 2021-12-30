#pragma once

#include "./Dispatch_NTDLL_NtResumeThread.h"

typedef struct _LPC_SECTION_WRITE
{
	ULONG Length;
	HANDLE SectionHandle;
	ULONG SectionOffset;
	ULONG ViewSize;
	PVOID ViewBase;
	PVOID TargetViewBase;
} LPC_SECTION_WRITE, *PLPC_SECTION_WRITE;

typedef struct _LPC_SECTION_READ
{
	ULONG Length;
	ULONG ViewSize;
	PVOID ViewBase;
} LPC_SECTION_READ, *PLPC_SECTION_READ;

typedef struct _LPC_MESSAGE
{
	USHORT DataSize;
	USHORT MessageSize;
	USHORT MessageType;
	USHORT VirtualRangesOffset;
	CLIENT_ID ClientId;
	ULONG_PTR MessageId;
	ULONG_PTR SectionSize;
	UCHAR Data[ANYSIZE_ARRAY];
} LPC_MESSAGE, *PLPC_MESSAGE;

typedef NTSTATUS (NTAPI * __pfnNtSecureConnectPort)
(
	PHANDLE PortHandle,
	PUNICODE_STRING PortName,
	PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	PLPC_SECTION_WRITE WriteSection,
	PSID pSid,
	PLPC_SECTION_READ ReadSection,
	PULONG MaximumMessageLength,
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
	);

extern __pfnNtSecureConnectPort pfnNtSecureConnectPort;

NTSTATUS
NTAPI
OnNtSecureConnectPort(
	PHANDLE PortHandle,
	PUNICODE_STRING PortName,
	PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	PLPC_SECTION_WRITE WriteSection,
	PSID pSid,
	PLPC_SECTION_READ ReadSection,
	PULONG MaximumMessageLength,
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
	);
NTSTATUS
NTAPI
NtSecureConnectPort(
	PHANDLE PortHandle,
	PUNICODE_STRING PortName,
	PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	PLPC_SECTION_WRITE WriteSection,
	PSID pSid,
	PLPC_SECTION_READ ReadSection,
	PULONG MaximumMessageLength,
	PVOID ConnectInfo,
	PULONG pConnectInfoLength
	);