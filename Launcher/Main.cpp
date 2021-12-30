#include <windows.h>
#include <stdio.h>

#include "./Main.h"
#include "../Config/Config.h"
#include "./Initalization.h"
#include "./BeginLauncher.h"
#include "./BeginHookPort.h"
#include "../Common/DebugLog.h"



//
//Program entry
//
int WINAPI wWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPWSTR lpCmdLine,int nCmdShow)
{
	//
	//Check arguments
	//
	//(Launcher.exe -Launch [SandBox Name])
	WCHAR szCommandLine[256] = {NULL};
	lstrcpy(szCommandLine,lpCmdLine);

	if( lstrlen(szCommandLine) >= 1 &&
		lstrlen(szCommandLine) <= CONF_SandBoxNameLength
		)
	{
		WCHAR szSandBoxName[CONF_SandBoxNameLength+1] = {NULL};
		lstrcpy(szSandBoxName,szCommandLine);

		if( LauncherInitalization(szSandBoxName) != -1 )
		{
			if( BeginHookPortRoutine(szSandBoxName) != -1 )
			{
				BeginLauncherByName(szSandBoxName);
			}
		}
	}

	return 0;
}