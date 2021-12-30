#include <windows.h>

#include "./CloneAPI_SHELL32.h"
#include "./Main.h"



//
//CloneAPI_SHELL32 Functions
//
int CAPI_SHCreateDirectoryExW(HWND hwnd, LPCWSTR pszPath, SECURITY_ATTRIBUTES *psa)
{
	int ret = NULL;
	typedef int (WINAPI *lpAddFun)(HWND,LPCWSTR,SECURITY_ATTRIBUTES *);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneSHELL32 );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"SHCreateDirectoryExW");
	if( addFun != NULL )
	{
		ret = addFun(hwnd,pszPath,psa);
	}
	return ret;
}