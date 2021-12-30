#pragma once

typedef NTSTATUS (NTAPI * __pfnNtAllocateVirtualMemory)
(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN ULONG                ZeroBits,
	IN OUT PULONG           RegionSize,
	IN ULONG                AllocationType,
	IN ULONG                Protect );

extern __pfnNtAllocateVirtualMemory pfnNtAllocateVirtualMemory;

NTSTATUS
NTAPI
OnNtAllocateVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN ULONG                ZeroBits,
	IN OUT PULONG           RegionSize,
	IN ULONG                AllocationType,
	IN ULONG                Protect );

NTSTATUS
NTAPI
NtAllocateVirtualMemory(
	IN HANDLE               ProcessHandle,
	IN OUT PVOID            *BaseAddress,
	IN ULONG                ZeroBits,
	IN OUT PULONG           RegionSize,
	IN ULONG                AllocationType,
	IN ULONG                Protect );