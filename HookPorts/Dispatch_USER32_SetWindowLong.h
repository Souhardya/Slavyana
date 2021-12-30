#pragma once

typedef LONG (WINAPI * __pfnSetWindowLongW)
(
    IN HWND hWnd,
    IN int nIndex,
    IN LONG dwNewLong);

extern __pfnSetWindowLongW pfnSetWindowLongW;

LONG
WINAPI
OnSetWindowLongW(
    IN HWND hWnd,
    IN int nIndex,
    IN LONG dwNewLong);