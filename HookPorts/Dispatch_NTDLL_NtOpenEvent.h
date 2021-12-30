#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"
#include "./Dispatch_NTDLL_NtCreateEvent.h"

typedef NTSTATUS (NTAPI * __pfnNtOpenEvent)
(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

extern __pfnNtOpenEvent pfnNtOpenEvent;

NTSTATUS
NTAPI
OnNtOpenEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);

NTSTATUS
NTAPI
NtOpenEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes);