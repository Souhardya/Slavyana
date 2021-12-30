#pragma once

DWORD GetProcAddressEx(IN HMODULE hModule, IN LPCSTR pProcName);

HINSTANCE GetKERNEL32ModHandle(void);