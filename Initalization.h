#pragma once



int GetSandBoxNameFromRegistry( OUT WCHAR * lpSandBoxName );

int QuerySandBoxState(IN const WCHAR * lpSandBoxName);

BOOL IsProcessInSandBox( IN DWORD dwProcessId ,IN WCHAR * lpSandBoxName );

int HookPortInitalization(IN HMODULE hModule, IN const WCHAR * lpSandBoxName);

int ResumeCurrentProcess(void);

int ConvertToGUIThread(void);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int LogProcessInfoToRegistrySYNC(IN const WCHAR * lpSandBoxName, IN DWORD dwProcessId);

int GetRestoreFilePath(IN const WCHAR * lpSandBoxName);

int GetRestoreRegPath(IN const WCHAR * lpSandBoxName);

int SelfCheck(void);

int GetCloneAPIPath(void);