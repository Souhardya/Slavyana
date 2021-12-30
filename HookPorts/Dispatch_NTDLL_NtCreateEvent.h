#pragma once

#include "./Dispatch_NTDLL_NtCreateThread.h"

//
// Event type
//

typedef enum _EVENT_TYPE {
    NotificationEvent,
    SynchronizationEvent
    } EVENT_TYPE;

typedef NTSTATUS (NTAPI * __pfnNtCreateEvent)
(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN EVENT_TYPE           EventType,
	IN BOOLEAN              InitialState);

extern __pfnNtCreateEvent pfnNtCreateEvent;

NTSTATUS
NTAPI
OnNtCreateEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN EVENT_TYPE           EventType,
	IN BOOLEAN              InitialState);

NTSTATUS
NTAPI
NtCreateEvent(
	OUT PHANDLE             EventHandle,
	IN ACCESS_MASK          DesiredAccess,
	IN POBJECT_ATTRIBUTES   ObjectAttributes OPTIONAL,
	IN EVENT_TYPE           EventType,
	IN BOOLEAN              InitialState);