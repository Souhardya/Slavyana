#include <windows.h>
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

#include "../Config/Config.h"
#include "./DisplayTools.h"
#include "./Install.h"
#include "./Remove.h"



//
//Install Functions
//
int CheckIsInstalled(void)
{
	//Return Value:
	//-1 = error
	//0 = not installed
	//1 = installed

	int iInstalled = -1;

	HKEY hkProgram = NULL;
	WCHAR szRegProgramPath[256] = {NULL};
	lstrcpy(szRegProgramPath,L"SOFTWARE\\");
	lstrcat(szRegProgramPath,CONF_SoftwareRegName);

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegProgramPath,0,KEY_ALL_ACCESS,&hkProgram);

	if( hkProgram == NULL )
	{
		//Not installed
		iInstalled = 0;
	}
	else
	{
		//Installed
		iInstalled = 1;
	}

	RegCloseKey(hkProgram);

	return iInstalled;
}

int InstallSandBox(void)
{
	//
	//Create HKEY_LOCAL_MACHINE\SOFTWARE\XXX
	//
	CreateSoftwareRegistry();

	//
	//Setup HKEY_LOCAL_MACHINE\SOFTWARE\XXX
	//
	SetupSoftwareRegistry();

	//
	//Create HKEY_LOCAL_MACHINE\SOFTWARE\XXX\Registry
	//
	SetupRegRegistry();

	//
	//Create Directory [CloneAPI]
	//
	CreateCloneAPIDirectory();

	//
	//Create SandBox [DefaultBox]
	//
	SetupRegSandBox(L"DefaultBox");

	return 0;
}

int CreateSoftwareRegistry(void)
{
	HKEY hkProgram = NULL;
	WCHAR szRegProgramPath[256] = {NULL};
	lstrcpy(szRegProgramPath,L"SOFTWARE\\");
	lstrcat(szRegProgramPath,CONF_SoftwareRegName);

	RegCreateKey(HKEY_LOCAL_MACHINE,szRegProgramPath,&hkProgram);

	RegCloseKey(hkProgram);

	return 0;
}

int SetupSoftwareRegistry(void)
{
	HKEY hkSandBoxMain = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};

	//
	//Create subkey [Config]
	//
	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_Config);

	RegCreateKey(HKEY_LOCAL_MACHINE,szRegSandBoxPath,&hkSandBoxMain);

	SetupRegConfig();

	RegCloseKey(hkSandBoxMain);

	//
	//Create subkey [SandBox]
	//
	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_SandBox);

	RegCreateKey(HKEY_LOCAL_MACHINE,szRegSandBoxPath,&hkSandBoxMain);

	RegCloseKey(hkSandBoxMain);

	return 0;
}

int SetupRegSandBox(IN const WCHAR * lpSandBoxName)
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

	//
	//Create RestorePath
	//
	lstrcat(szRestorePath,L"\\");
	lstrcat(szRestorePath,lpSandBoxName);

	CreateDirectory(szRestorePath,NULL);

	//
	//Create [RestorePath\Drive]
	//
	WCHAR szRestoreDrivePath[MAX_PATH] = {NULL};
	lstrcpy(szRestoreDrivePath,szRestorePath);
	lstrcat(szRestoreDrivePath,L"\\");
	lstrcat(szRestoreDrivePath,CONF_SandBox_Drive_Name);

	CreateDirectory(szRestoreDrivePath,NULL);

	//
	//Create RegHive
	//
	WCHAR szRegHivePath[MAX_PATH] = {NULL};
	lstrcpy(szRegHivePath,szRestorePath);
	lstrcat(szRegHivePath,L"\\");
	lstrcat(szRegHivePath,CONF_SandBox_RegHive_FileName);

	HKEY hkRegistry = NULL;
	WCHAR szRegRegistryPath[256] = {NULL};

	lstrcpy(szRegRegistryPath,L"SOFTWARE\\");
	lstrcat(szRegRegistryPath,CONF_SoftwareRegName);
	lstrcat(szRegRegistryPath,L"\\");
	lstrcat(szRegRegistryPath,CONF_SoftwareReg_SandBox_Registry);

	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegRegistryPath,NULL,KEY_ALL_ACCESS,&hkRegistry) == ERROR_SUCCESS )
	{
		RegSaveKey(hkRegistry,szRegHivePath,NULL);
	}

	RegCloseKey(hkRegistry);

	//
	//Create subkey [SandBox\XXX]
	//
	HKEY hkSandBox = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};

	WCHAR szSubKeyPath[256] = {NULL};

	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_SandBox);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,lpSandBoxName);

	RegCreateKey(HKEY_LOCAL_MACHINE,szRegSandBoxPath,&hkSandBox);

	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\View]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_View);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Restore]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_Restore);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\CleanUp]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_CleanUp);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Begin]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_Begin);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Stop]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_Stop);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\File]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_File);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Limit]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_Limit);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Resource]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_Resource);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Software]
	//
	lstrcpy(szSubKeyPath,szRegSandBoxPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_Software);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	return 0;
}

int SetupRegConfig(void)
{
	//
	//Create directory
	//
	WCHAR szRestorePath[MAX_PATH] = {NULL};
	GetSystemDirectory(szRestorePath,MAX_PATH);
	szRestorePath[3] = '\0';
	lstrcat(szRestorePath,L"SandBox");

	CreateDirectory(szRestorePath,NULL);

	//
	//Set value [RestorePath]
	//
	HKEY hkSandBoxMain = NULL;
	WCHAR szRegSandBoxPath[256] = {NULL};

	lstrcpy(szRegSandBoxPath,L"SOFTWARE\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareRegName);
	lstrcat(szRegSandBoxPath,L"\\");
	lstrcat(szRegSandBoxPath,CONF_SoftwareReg_Config);

	RegOpenKeyEx(HKEY_LOCAL_MACHINE,szRegSandBoxPath,NULL,KEY_ALL_ACCESS,&hkSandBoxMain);

	RegSetValueEx(
		hkSandBoxMain,
		CONF_SoftwareReg_Config_RestorePath,
		NULL,
		REG_EXPAND_SZ,
		(const BYTE *)szRestorePath,
		MAX_PATH
		);

	RegCloseKey(hkSandBoxMain);

	return 0;
}

int SetupRegRegistry(void)
{
	//
	//Create subkey [SandBox\XXX\Registry]
	//
	HKEY hkSandBox = NULL;
	WCHAR szRegRegistryPath[256] = {NULL};

	lstrcpy(szRegRegistryPath,L"SOFTWARE\\");
	lstrcat(szRegRegistryPath,CONF_SoftwareRegName);
	lstrcat(szRegRegistryPath,L"\\");
	lstrcat(szRegRegistryPath,CONF_SoftwareReg_SandBox_Registry);
	RegCreateKey(HKEY_LOCAL_MACHINE,szRegRegistryPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	WCHAR szSubKeyPath[256] = {NULL};

	//
	//Create subkey [SandBox\XXX\Registry\HKEY_CLASSES_ROOT]
	//
	lstrcpy(szSubKeyPath,szRegRegistryPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_HKEY_CLASSES_ROOT);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Registry\HKEY_CURRENT_USER]
	//
	lstrcpy(szSubKeyPath,szRegRegistryPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_HKEY_CURRENT_USER);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Registry\HKEY_LOCAL_MACHINE]
	//
	lstrcpy(szSubKeyPath,szRegRegistryPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_HKEY_LOCAL_MACHINE);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Registry\HKEY_USERS]
	//
	lstrcpy(szSubKeyPath,szRegRegistryPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_HKEY_USERS);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	//
	//Create subkey [SandBox\XXX\Registry\HKEY_CURRENT_CONFIG]
	//
	lstrcpy(szSubKeyPath,szRegRegistryPath);
	lstrcat(szSubKeyPath,L"\\");
	lstrcat(szSubKeyPath,CONF_SoftwareReg_SandBox_HKEY_CURRENT_CONFIG);
	RegCreateKey(HKEY_LOCAL_MACHINE,szSubKeyPath,&hkSandBox);
	RegCloseKey(hkSandBox);

	return 0;
}

int CreateCloneAPIDirectory(void)
{
	//
	//Create directory
	//
	WCHAR szCloneAPIDirectory[MAX_PATH] = {NULL};
	GetModuleFileName(NULL,szCloneAPIDirectory,MAX_PATH);
	PathRemoveFileSpec(szCloneAPIDirectory);
	lstrcat(szCloneAPIDirectory,L"\\");
	lstrcat(szCloneAPIDirectory,CONF_CloneAPI_DirectoryName);

	CreateDirectory(szCloneAPIDirectory,NULL);

	//
	//Copy file to CloneAPI directory
	//
	WCHAR szSystemDirectory[MAX_PATH] = {NULL};
	GetSystemDirectory(szSystemDirectory,MAX_PATH);

	WCHAR szOriginalPath[MAX_PATH] = {NULL};
	WCHAR szCloneAPIPath[MAX_PATH] = {NULL};

	//kernel32.dll
	wsprintf(szOriginalPath,L"%s\\%s",szSystemDirectory,L"kernel32.dll");
	wsprintf(szCloneAPIPath,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneKERNEL32);
	DeleteFile(szCloneAPIPath);
	CopyFile(szOriginalPath,szCloneAPIPath,TRUE);

	//fltlib.dll
	wsprintf(szOriginalPath,L"%s\\%s",szSystemDirectory,L"fltlib.dll");
	wsprintf(szCloneAPIPath,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneFLTLIB);
	DeleteFile(szCloneAPIPath);
	CopyFile(szOriginalPath,szCloneAPIPath,TRUE);

	//shell32.dll
	wsprintf(szOriginalPath,L"%s\\%s",szSystemDirectory,L"shell32.dll");
	wsprintf(szCloneAPIPath,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneSHELL32);
	DeleteFile(szCloneAPIPath);
	CopyFile(szOriginalPath,szCloneAPIPath,TRUE);

	//advapi32.dll
	wsprintf(szOriginalPath,L"%s\\%s",szSystemDirectory,L"advapi32.dll");
	wsprintf(szCloneAPIPath,L"%s\\%s",szCloneAPIDirectory,CONF_CloneAPI_CloneADVAPI32);
	DeleteFile(szCloneAPIPath);
	CopyFile(szOriginalPath,szCloneAPIPath,TRUE);

	return 0;
}