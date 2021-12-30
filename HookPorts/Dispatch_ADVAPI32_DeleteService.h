#pragma once

typedef BOOL (WINAPI * __pfnDeleteService)
(
	SC_HANDLE   hService);

extern __pfnDeleteService pfnDeleteService;

BOOL
WINAPI
OnDeleteService(
	SC_HANDLE   hService);