#pragma once

int BeginSandBoxRoutine(void);

int ListSandBoxToBegin(void);

int QuerySandBoxState(IN const WCHAR * lpSandBoxName);

int BeginSandBoxDispatch(IN const WCHAR * lpString);

int BeginSandBoxByName(IN const WCHAR * lpSandBoxName);

int BeginSandBoxLoadRegHive(IN const WCHAR * lpSandBoxName);

int BeginSandBoxCreateRegistrySYNC(IN const WCHAR * lpSandBoxName);

int BeginSandBoxCallLauncher(IN const WCHAR * lpSandBoxName, IN const WCHAR * lpApplicationPath);

int BeginSandBoxCleanCallback(IN const WCHAR * lpSandBoxName);