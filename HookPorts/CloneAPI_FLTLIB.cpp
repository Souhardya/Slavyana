#include <windows.h>

#include "./CloneAPI_FLTLIB.h"
#include "./Main.h"



//
//CloneAPI_FLTLIB Functions
//
HRESULT
WINAPI
CAPI_FilterGetDosName(
	IN LPCWSTR  lpVolumeName,
	IN OUT LPWSTR  lpDosName,
	IN DWORD  dwDosNameBufferSize
	)
{
	//
	//Parameters:
	//
	//[lpVolumeName]
	//The lpVolumeName input string can be any of the following. The trailing backslash (\) is optional. 
	//
	//A drive letter, such as "D:\"
	//A path to a volume mount point, such as "c:\mnt\edrive\"
	//A unique volume identifier (also called a volume GUID name), such as "\??\Volume{7603f260-142a-11d4-ac67-806d6172696f}\"
	//A nonpersistent device name (also called a target name or an NT device name), such as "\Device\HarddiskVolume1\"
	//
	//[Return Value]
	//FilterGetDosName returns S_OK if successful. Otherwise, it returns an error value.

	HRESULT ret = NULL;
	typedef HRESULT (WINAPI *lpAddFun)(LPCWSTR,LPWSTR,DWORD);
	HINSTANCE hDll = GetModuleHandleW( CONF_CloneAPI_CloneFLTLIB );
	lpAddFun addFun = (lpAddFun)GetProcAddress(hDll,"FilterGetDosName");
	if( addFun != NULL )
	{
		ret = addFun(lpVolumeName,lpDosName,dwDosNameBufferSize);
	}
	return ret;
}