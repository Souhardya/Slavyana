#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "../Config/Config.h"
#include "./DisplayTools.h"
#include "./Remove.h"
#include "./Install.h"



//
//Remove Functions
//
int RemoveSandBox(void)
{
	//
	//Remove RestorePath
	//
	RemoveRestorePath();

	//
	//Create Directory [CloneAPI]
	//
	RemoveCloneAPIDirectory();

	//
	//Remove HKEY_LOCAL_MACHINE\SOFTWARE\XXX
	//
	RemoveSoftwareRegistry();

	return 0;
}

int RemoveSoftwareRegistry(void)
{
	HKEY hkProgram = NULL;
	WCHAR szRegProgramPath[256] = {NULL};
	lstrcpy(szRegProgramPath,L"SOFTWARE\\");
	lstrcat(szRegProgramPath,CONF_SoftwareRegName);

	SHDeleteKey(HKEY_LOCAL_MACHINE,szRegProgramPath);

	return 0;
}

int RemoveRestorePath(void)
{
	//
	//Get RestorePath
	//
	WCHAR szRestorePath[MAX_PATH] = {NULL};
	DWORD dwType = REG_SZ;
	DWORD dwBufLen = MAX_PATH;

	HKEY hkSandBoxCfg = NULL;
	WCHAR szRegConfigPath[256] = {NULL};

	lstrcpy(szRegConfigPath,L"SOFTWARE\\");
	lstrcat(szRegConfigPath,CONF_SoftwareRegName);
	lstrcat(szRegConfigPath,L"\\");
	lstrcat(szRegConfigPath,CONF_SoftwareReg_Config);

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegConfigPath,NULL,KEY_ALL_ACCESS,&hkSandBoxCfg);

	dwType = REG_EXPAND_SZ;
	dwBufLen = MAX_PATH;

	RegQueryValueEx(
					hkSandBoxCfg,
					CONF_SoftwareReg_Config_RestorePath,
					NULL,
					&dwType,
					(LPBYTE)&szRestorePath,
					&dwBufLen
					);

	RegCloseKey(hkSandBoxCfg);

	WCHAR szRestoreTruePath[MAX_PATH] = {NULL};
	lstrcpy(szRestoreTruePath,szRestorePath);

	RemoveDirectoryEx(szRestoreTruePath);

	return 0;
}

int RemoveDirectoryEx( IN const WCHAR * lpPathName )
{
	SHFILEOPSTRUCT fo = 
	{
		NULL,
		FO_DELETE,
		lpPathName,
		NULL,
		FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION,
		FALSE,
		NULL,
		NULL
	};

	return SHFileOperation(&fo);
}

int RemoveCloneAPIDirectory(void)
{
	//
	//Remove directory
	//
	WCHAR szCloneAPIDirectory[MAX_PATH] = {NULL};
	GetModuleFileName(NULL,szCloneAPIDirectory,MAX_PATH);
	PathRemoveFileSpec(szCloneAPIDirectory);
	lstrcat(szCloneAPIDirectory,L"\\");
	lstrcat(szCloneAPIDirectory,CONF_CloneAPI_DirectoryName);

	WCHAR szCloneAPITruePath[MAX_PATH] = {NULL};
	lstrcpy(szCloneAPITruePath,szCloneAPIDirectory);

	RemoveDirectoryEx(szCloneAPITruePath);

	return 0;
}