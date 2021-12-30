#pragma once

typedef BOOL (WINAPI * __pfnControlService)
(
	SC_HANDLE           hService,
	DWORD               dwControl,
	LPSERVICE_STATUS    lpServiceStatus);

extern __pfnControlService pfnControlService;

BOOL
WINAPI
OnControlService(
	SC_HANDLE           hService,
	DWORD               dwControl,
	LPSERVICE_STATUS    lpServiceStatus);