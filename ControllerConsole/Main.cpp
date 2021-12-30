#include <windows.h>
#include <stdio.h>
#include <signal.h>
#include <locale.h>

#include "./Main.h"
#include "./DisplayTools.h"
#include "./Initalization.h"
#include "./DispatchRoutine.h"
#include "./Install.h"
#include "../Common/DebugLog.h"



//
//Global
//



//
//Program entry
//
int wmain(void)
{
	//
	//Set Local Language
	//
	_wsetlocale(LC_ALL,L"chs");

	//
	//Support standard command line
	//
	_wsystem(L"");

	//
	//Disable force close
	//
	DisableForceClose();

	//
	//Set Console Title
	//
	SetConsoleTitle(L"Slavyana Sandbox -- Press [Ctrl]+[C] to quit!");

	//
	//Set quit callback
	//
	signal(SIGINT,OnBreak);

	//
	//DispatchRoutine
	//
	DispatchRoutineEntry();

	return 0;
}

//
//Quit Routine
//
void OnBreak(int nSig)
{
	wprintfex(
		L"\r\n\t\t[NOTIFY] Ask if user want to quit.\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	int iQuitConfirm = IDCANCEL;
	iQuitConfirm = 
		MessageBox(
					0,
					L"Do you want to quit Slavyana Controller?",
					L"Slavyana Quit confirm",
					MB_ICONQUESTION|MB_OKCANCEL|MB_SETFOREGROUND
					)==IDOK;

	if( iQuitConfirm != IDOK )
	{
		wprintfex(
			L"\r\n\t\t[NOTIFY] Continue running.\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		signal(SIGINT,OnBreak);
	}
	else
	{
		wprintfex(
			L"\r\n\t\t[NOTIFY] Exiting process...Please wait for a moment...\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

		EnableForceClose();

		ExitProcess(NULL);
	}

	ResetConsoleTextAttribute();

	return;
}