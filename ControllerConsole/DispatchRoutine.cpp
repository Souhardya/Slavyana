#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#include "./DisplayTools.h"
#include "./DispatchRoutine.h"
#include "./Initalization.h"
#include "./Install.h"
#include "./Remove.h"
#include "./BeginSandBox.h"
#include "./StopSandBox.h"
#include "../Common/DebugLog.h"



//
//Global
//



//
//DispatchRoutine Functions
//
int DispatchRoutineEntry()
{
	//
	//Display copyright information
	//
	wprintf(L"\r\n\r\n");
	wprintfex(
		L"\t\tSlavyana Controller)\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN);
	wprintf(L"\r\n\r\n");

	

	//
	//Display dispatch information
	//
	int iShowDispatch = 0;
	while(TRUE)
	{
		if( iShowDispatch == -1 )
		{
			iShowDispatch = ShowDispatch(CheckIsInstalled(),FALSE);
		}
		else
		{
			iShowDispatch = ShowDispatch(CheckIsInstalled(),TRUE);
		}
	}

	return 1;
}

int ShowDispatch(IN int lpInstalled,IN BOOL bIsShowNotice)
{
	//Dispatch:
	//1 = Begin
	//2 = Stop
	//3 = Setup
	//4 = Remove
	//5 = Config
	//6 = Show
	//exit = Exit

	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	WORD wdColorWhite = FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
	WORD wdColorGray = FOREGROUND_INTENSITY | FOREGROUND_RED&FOREGROUND_GREEN&FOREGROUND_BLUE;

	//
	//Notice
	//
	if( bIsShowNotice == TRUE )
	{
		wprintfex(
			L"\r\n\t[NOTICE] YOU CAN PRESS [Ctrl]+[C] TO QUIT AT ANY TIME!\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN);
	}

	//
	//Menu
	//
	if( bIsShowNotice == TRUE )
	{
		wprintfex(
			L"\r\n[Command]    [Description]\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_GREEN|FOREGROUND_BLUE);

		if(lpInstalled)
		{
			wprintfex(
				L"    1        Begin (Run program in SandBox)\r\n",
				wdColorWhite);
		}
		else
		{
			wprintfex(
				L"    1        Begin (Run program in SandBox)\r\n",
				wdColorGray);
		}

		if(lpInstalled)
		{
			wprintfex(
				L"    2        Stop (Terminate program running in SandBox)\r\n",
				wdColorWhite);
		}
		else
		{
			wprintfex(
				L"    2        Stop (Terminate program running in SandBox)\r\n",
				wdColorGray);
		}

		if(lpInstalled)
		{
			wprintfex(
				L"    3        Control (Control SandBox)\r\n",
				wdColorWhite);
		}
		else
		{
			wprintfex(
				L"    3        Control (Control SandBox)\r\n",
				wdColorGray);
		}

		if(lpInstalled)
		{
			wprintfex(
				L"    4        Install (Install SandBox into computer)\r\n",
				wdColorGray);
		}
		else
		{
			wprintfex(
				L"    4        Install (Install SandBox into computer)\r\n",
				wdColorWhite);
		}

		if(lpInstalled)
		{
			wprintfex(
				L"    5        Remove (Remove SandBox from computer)\r\n",
				wdColorWhite);
		}
		else
		{
			wprintfex(
				L"    5        Remove (Remove SandBox from computer)\r\n",
				wdColorGray);
		}

		if(lpInstalled)
		{
			wprintfex(
				L"    6        Config (Change settings of SandBox)\r\n",
				wdColorWhite);
		}
		else
		{
			wprintfex(
				L"    6        Config (Change settings of SandBox)\r\n",
				wdColorGray);
		}

		wprintfex(
			L"    7        Help (Show detail help information)\r\n",
			wdColorWhite);

		wprintfex(
			L"   exit      Exit (Quit SandBox)\r\n",
			wdColorWhite);
	}

	//
	//Waiting order
	//
	wprintfex(
		L"\r\n[NOTIFY] Please input command:\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	WCHAR szUserInput[256] = {NULL};

	_getws_s(szUserInput);

	//
	//Dispatch
	//
	if( wcsicmp(szUserInput,L"1") == 0 ||
		wcsicmp(szUserInput,L"Begin") == 0
		)
	{
		RoutineBegin();
	}
	else
	if( wcsicmp(szUserInput,L"2") == 0 ||
		wcsicmp(szUserInput,L"Stop") == 0
		)
	{
		RoutineStop();
	}
	else
	if( wcsicmp(szUserInput,L"3") == 0 ||
		wcsicmp(szUserInput,L"Control") == 0
		)
	{
		RoutineControl();
	}
	else
	if( wcsicmp(szUserInput,L"4") == 0 ||
		wcsicmp(szUserInput,L"Install") == 0
		)
	{
		RoutineInstall();
	}
	else
	if( wcsicmp(szUserInput,L"5") == 0 ||
		wcsicmp(szUserInput,L"Remove") == 0
		)
	{
		RoutineRemove();
	}
	else
	if( wcsicmp(szUserInput,L"6") == 0 ||
		wcsicmp(szUserInput,L"Config") == 0
		)
	{
		RoutineConfig();
	}
	else
	if( wcsicmp(szUserInput,L"7") == 0 ||
		wcsicmp(szUserInput,L"Help") == 0
		)
	{
		RoutineHelp();
	}
	else
	if( wcsicmp(szUserInput,L"exit") == 0 )
	{
		RoutineExit();
	}
	else
	{
		wprintfex(
			L"\r\n[NOTIFY] Bad command.\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		return -1;
	}

	return 0;
}

int RoutineBegin(void)
{
	BeginSandBoxRoutine();

	return 0;
}

int RoutineStop(void)
{
	StopSandBoxRoutine();

	return 0;
}

int RoutineControl(void)
{
	wprintf(L"\r\n[NOTIFY] Sorry!Not done yet!\r\n");

	return 0;
}

int RoutineInstall(void)
{
	//
	//Ask if want to install
	//
	wprintfex(
		L"\r\n[NOTIFY] Do you want to INSTALL SandBox into computer?(Y/N)\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	WCHAR szUserInput[256] = {NULL};

	_getws_s(szUserInput);

	if( wcsicmp(szUserInput,L"Y") != 0 )
	{
		return 0;
	}

	//
	//Check is installed
	//
	if( CheckIsInstalled() == 1 )
	{
		wprintfex(
			L"\r\n[NOTIFY] SandBox already installed on this computer! Please REMOVE it before installation.\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		_wsystem(L"pause");

		return -1;
	}

	//
	//Initalization
	//
	int iResult = -1;
	iResult = SandBoxInitalization();
	if( iResult < 0 )
	{
		wprintfex(
			L"SandBox initialize failed!\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		wprintfex(
			L"\r\n[NOTIFY] Do you STILL want to INSTALL SandBox into computer? SandBox may not work well in this system.(Y/N)\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		_getws_s(szUserInput);


		if( wcsicmp(szUserInput,L"Y") != 0 )
		{
			return -1;
		}
	}

	//
	//Install
	//
	InstallSandBox();

	//
	//Complete
	//
	wprintfex(
		L"\r\nInstallation complete successfully!\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	_wsystem(L"pause");

	return 0;
}

int RoutineRemove(void)
{
	//
	//Ask if want to remove
	//
	wprintfex(
		L"\r\n[NOTIFY] Do you want to REMOVE SandBox from computer?(Y/N)\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	WCHAR szUserInput[256] = {NULL};

	_getws_s(szUserInput);

	if( wcsicmp(szUserInput,L"Y") != 0 )
	{
		return 0;
	}

	//
	//Check is installed
	//
	if( CheckIsInstalled() != 1 )
	{
		wprintfex(
			L"\r\n[NOTIFY] SandBox not installed on this computer yet!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		_wsystem(L"pause");

		return -1;
	}

	wprintfex(
		L"\r\n\t\t[NOTIFY] Removing...Please wait for a moment...\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	//
	//Remove
	//
	RemoveSandBox();

	//
	//Complete
	//
	wprintfex(
		L"\r\nRemove succeed!\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN);

	_wsystem(L"pause");

	return 0;
}

int RoutineConfig(void)
{
	wprintf(L"\r\n[NOTIFY] Sorry!Not done yet!\r\n");

	return 0;
}

int RoutineHelp(void)
{
	wprintf(L"\r\n[NOTIFY] Sorry!Not done yet!\r\n");

	return 0;
}

int RoutineExit(void)
{
	wprintfex(
		L"\r\n\t\t[NOTIFY] Exiting process...Please wait for a moment...\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	EnableForceClose();

	ExitProcess(NULL);

	return 0;
}