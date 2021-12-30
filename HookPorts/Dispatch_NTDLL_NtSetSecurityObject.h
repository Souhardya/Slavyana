#pragma once

#include "./Dispatch_NTDLL_NtEnumerateKey.h"
#include "./Dispatch_NTDLL_NtCreateThread.h"

typedef NTSTATUS (NTAPI * __pfnNtSetSecurityObject)
(
	IN HANDLE               ObjectHandle,
	IN SECURITY_INFORMATION SecurityInformationClass,
	IN PSECURITY_DESCRIPTOR DescriptorBuffer
	);

extern __pfnNtSetSecurityObject pfnNtSetSecurityObject;

NTSTATUS
NTAPI
OnNtSetSecurityObject(
	IN HANDLE               ObjectHandle,
	IN SECURITY_INFORMATION SecurityInformationClass,
	IN PSECURITY_DESCRIPTOR DescriptorBuffer
	);

NTSTATUS
NTAPI
NtSetSecurityObject(
	IN HANDLE               ObjectHandle,
	IN SECURITY_INFORMATION SecurityInformationClass,
	IN PSECURITY_DESCRIPTOR DescriptorBuffer
	);