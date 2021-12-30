#pragma once

typedef BOOL (WINAPI * __pfnUnhookWindowsHookEx)
(
	IN HHOOK hhk);

extern __pfnUnhookWindowsHookEx pfnUnhookWindowsHookEx;

BOOL
WINAPI
OnUnhookWindowsHookEx(
	IN HHOOK hhk);