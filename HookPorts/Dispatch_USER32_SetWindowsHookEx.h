#pragma once

typedef HHOOK (WINAPI * __pfnSetWindowsHookExW)
(
	IN int idHook,
	IN HOOKPROC lpfn,
	IN HINSTANCE hmod,
	IN DWORD dwThreadId);

extern __pfnSetWindowsHookExW pfnSetWindowsHookExW;

HHOOK
WINAPI
OnSetWindowsHookExW(
	IN int idHook,
	IN HOOKPROC lpfn,
	IN HINSTANCE hmod,
	IN DWORD dwThreadId);