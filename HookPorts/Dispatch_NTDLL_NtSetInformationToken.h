#pragma once

typedef NTSTATUS (NTAPI * __pfnNtSetInformationToken)
(
	IN HANDLE               TokenHandle,
	IN TOKEN_INFORMATION_CLASS TokenInformationClass,
	OUT PVOID               TokenInformation,
	IN ULONG                TokenInformationLength
	);

extern __pfnNtSetInformationToken pfnNtSetInformationToken;

NTSTATUS
NTAPI
OnNtSetInformationToken(
	IN HANDLE               TokenHandle,
	IN TOKEN_INFORMATION_CLASS TokenInformationClass,
	OUT PVOID               TokenInformation,
	IN ULONG                TokenInformationLength
	);

NTSTATUS
NTAPI
NtSetInformationToken(
	IN HANDLE               TokenHandle,
	IN TOKEN_INFORMATION_CLASS TokenInformationClass,
	OUT PVOID               TokenInformation,
	IN ULONG                TokenInformationLength
	);