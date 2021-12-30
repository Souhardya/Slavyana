#include <windows.h>
#include <stdio.h>

#include "./DisplayTools.h"



//
//DisplayTools Functions
//

void wprintfex(IN WCHAR * lpText,IN WORD color)
{
	HANDLE hOut = NULL;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	WORD wOldColorAttrs = NULL;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
	SetConsoleTextAttribute(hOut,color);
	wprintf(L"%s",lpText);
	SetConsoleTextAttribute(hOut,wOldColorAttrs);

	return;
}

void printfex(IN char * strText,IN WORD color)
{
	HANDLE hOut = NULL;
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
	WORD wOldColorAttrs = NULL;

	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(hOut, &csbiInfo);
	wOldColorAttrs = csbiInfo.wAttributes;
	SetConsoleTextAttribute(hOut,color);
	printf("%s",strText);
	SetConsoleTextAttribute(hOut,wOldColorAttrs);

	return;
}

void ResetConsoleTextAttribute(void)
{
	HANDLE hOut = NULL;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOut,FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

	return;
}

void DisableForceClose(void)
{
	//
	//Disable force close
	//
	WCHAR szConsoleTitle[256] = {NULL};
	GetConsoleTitle(szConsoleTitle,256);
	HWND hwnd = FindWindow(NULL,szConsoleTitle);
	HMENU hmenu = GetSystemMenu(hwnd,FALSE);
	RemoveMenu(hmenu,SC_CLOSE,NULL);

	return;
}

void EnableForceClose(void)
{
	//
	//Enable force close
	//
	WCHAR szConsoleTitle[256] = {NULL};
	GetConsoleTitle(szConsoleTitle,256);
	HWND hwnd = FindWindow(NULL,szConsoleTitle);
	HMENU hmenu = GetSystemMenu(hwnd,FALSE);
	InsertMenu(hmenu,MF_BYPOSITION,NULL,SC_CLOSE,NULL);

	return;
}