#pragma once

typedef LRESULT (WINAPI * __pfnSendMessageA)
(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

extern __pfnSendMessageA pfnSendMessageA;

LRESULT
WINAPI
OnSendMessageA(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);



typedef LRESULT (WINAPI * __pfnSendMessageW)
(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);

extern __pfnSendMessageW pfnSendMessageW;

LRESULT
WINAPI
OnSendMessageW(
    IN HWND hWnd,
    IN UINT Msg,
    IN WPARAM wParam,
    IN LPARAM lParam);