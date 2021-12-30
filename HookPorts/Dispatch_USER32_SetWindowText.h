#pragma once

typedef BOOL (WINAPI * __pfnSetWindowTextA)
(
    IN HWND hWnd,
    IN LPCSTR lpString);

extern __pfnSetWindowTextA pfnSetWindowTextA;

BOOL
WINAPI
OnSetWindowTextA(
    IN HWND hWnd,
    IN LPCSTR lpString);



typedef BOOL (WINAPI * __pfnSetWindowTextW)
(
    IN HWND hWnd,
    IN LPCWSTR lpString);

extern __pfnSetWindowTextW pfnSetWindowTextW;

BOOL
WINAPI
OnSetWindowTextW(
    IN HWND hWnd,
    IN LPCWSTR lpString);