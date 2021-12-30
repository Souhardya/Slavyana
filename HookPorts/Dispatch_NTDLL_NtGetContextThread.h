#pragma once

typedef NTSTATUS (NTAPI * __pfnNtGetContextThread)
(
	IN HANDLE               ThreadHandle,
	OUT PCONTEXT            pContext );

extern __pfnNtGetContextThread pfnNtGetContextThread;

NTSTATUS
NTAPI
OnNtGetContextThread(
	IN HANDLE               ThreadHandle,
	OUT PCONTEXT            pContext );

NTSTATUS
NTAPI
NtGetContextThread(
	IN HANDLE               ThreadHandle,
	OUT PCONTEXT            pContext );