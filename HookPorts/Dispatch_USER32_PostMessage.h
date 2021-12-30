#pragma once

typedef BOOL (WINAPI * __pfnPostMessageA)
(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

extern __pfnPostMessageA pfnPostMessageA;

BOOL
WINAPI
OnPostMessageA(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);



typedef BOOL (WINAPI * __pfnPostMessageW)
(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

extern __pfnPostMessageW pfnPostMessageW;

BOOL
WINAPI
OnPostMessageW(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);