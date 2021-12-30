#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>
#include <Iphlpapi.h>
#pragma comment (lib,"Iphlpapi.lib")

#include "../Config/Config.h"
#include "./DisplayTools.h"
#include "./Initalization.h"
#include "./Privilege.h"
#include "./ProcessTools.h"
#include "./VerifyFiles.h"



//
//Initalization Functions
//
int SandBoxInitalization(void)
{
	int iRet = 0;
	//
	//Check the OS
	//
	if( CheckSystemInfo() == -1 )
	{
		iRet--;

		wprintfex(
			L"Slavyana may not work well on this Operating System!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
	else
	{
		wprintfex(
			L"\r\nCheck Operating System complete successfully!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	}

	//
	//Adjust the privileges
	//
	if( AdjustPrivileges() == -1 )
	{
		iRet--;

		wprintfex(
			L"\r\nSlavyana may not work well on this low privilege mode!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
	else
	{
		wprintfex(
			L"Adjust privileges complete successfully!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	}

	//
	//Check the environment
	//
	if( CheckEnvironment() == -1 )
	{
		iRet--;

		wprintfex(
			L"Slavyana may not work well in this environment!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
	else
	{
		wprintfex(
			L"Check environment complete successfully!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	}

	//
	//Self check
	//
	if( SelfCheck() == -1 )
	{
		iRet--;

		wprintfex(
			L"Self check failed!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);
	}
	else
	{
		wprintfex(
			L"Self check complete successfully!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_GREEN);
	}

	return iRet;
}


int CheckSystemInfo(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	wprintfex(
		L"Checking Operating System...Please wait for a moment...\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN|FOREGROUND_BLUE);

	//
	//Check Operating System version
	//
	wprintf(L"Checking Operating System version...\r\n");

	OSVERSIONINFOEX OsVerInfoEx;
	OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !GetVersionEx((OSVERSIONINFO *)&OsVerInfoEx) )
	{
		return -1;
	}

	WCHAR szOsVerInfo[256] = {NULL};
	WCHAR szOsName[32] = {NULL};

	if( OsVerInfoEx.dwPlatformId == VER_PLATFORM_WIN32_NT )
	{
		lstrcpy(szOsName,L"Unknown");

		if( OsVerInfoEx.dwMajorVersion<=4 )
		{
			lstrcpy(szOsName,L"NT");
		}
		if( OsVerInfoEx.dwMajorVersion==5 && OsVerInfoEx.dwMinorVersion==0 )
		{
			lstrcpy(szOsName,L"2000");
		}
		if( OsVerInfoEx.dwMajorVersion==5 && OsVerInfoEx.dwMinorVersion==1 )
		{
			lstrcpy(szOsName,L"XP");
		}
		if( OsVerInfoEx.dwMajorVersion==5 && OsVerInfoEx.dwMinorVersion==2 )
		{
			lstrcpy(szOsName,L"2003");
		}
		if( OsVerInfoEx.dwMajorVersion==6 && OsVerInfoEx.dwMinorVersion==0 )
		{
			lstrcpy(szOsName,L"Vista");
		}
		if( OsVerInfoEx.dwMajorVersion==6 && OsVerInfoEx.dwMinorVersion==1 )
		{
			lstrcpy(szOsName,L"7");
		}
	}
	else
	{
		wprintfex(
			L"Do not support this Operating System!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		return -1;
	}

	swprintf(
		szOsVerInfo,
		L"OS Name:   \tMicrosoft Windows %s\r\nOS Version:\t%d.%d.%d Service Pack %d\r\n\r\n",
		szOsName,
		OsVerInfoEx.dwMajorVersion,
		OsVerInfoEx.dwMinorVersion,
		OsVerInfoEx.dwBuildNumber,
		OsVerInfoEx.wServicePackMajor
		);

	wprintfex(
		szOsVerInfo,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	//
	//Check CPU info
	//
	wprintf(L"Checking CPU info...\r\n");

    SYSTEM_INFO systeminfo;
	GetSystemInfo(&systeminfo);

	WCHAR szNumberOfProcessors[256] = {NULL};

	swprintf(szNumberOfProcessors,L"CPU:       \t%d CPU installed.\r\n",systeminfo.dwNumberOfProcessors);
	wprintfex(
		szNumberOfProcessors,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	HKEY hCPUKey = NULL;

	for(DWORD i=0;i<systeminfo.dwNumberOfProcessors;i++)
	{
		WCHAR szCPUInfo[1024] = {NULL};

		DWORD dwCPUMhz = 0;
		DWORD dwBytes = sizeof(DWORD);
		DWORD dwType = REG_DWORD;
		WCHAR szIdentifier[256] = {NULL};
		WCHAR szVendorIdentifier[256] = {NULL};

		WCHAR szCPUSubKey[256] = {NULL};
		swprintf(szCPUSubKey,L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\%d",i);

		RegOpenKey(HKEY_LOCAL_MACHINE, szCPUSubKey, &hCPUKey);

		RegQueryValueEx(hCPUKey, L"~MHz", NULL, &dwType, (LPBYTE)&dwCPUMhz, &dwBytes);

		dwType = REG_SZ;
		dwBytes = 256;

		RegQueryValueEx(hCPUKey, L"Identifier", NULL, &dwType, (LPBYTE)&szIdentifier, &dwBytes);
		RegQueryValueEx(hCPUKey, L"VendorIdentifier", NULL, &dwType, (LPBYTE)&szVendorIdentifier, &dwBytes);

		swprintf(
			szCPUInfo,
			L"           \t[%d]: %s %s ~%d Mhz\r\n",
			i,
			szIdentifier,
			szVendorIdentifier,
			dwCPUMhz);

		wprintfex(
			szCPUInfo,
			FOREGROUND_INTENSITY | FOREGROUND_BLUE);
	}

	RegCloseKey(hCPUKey);

	//
	//Check System info
	//
	wprintf(L"\r\nChecking System info...\r\n");

	WCHAR szWindowsDirectory[MAX_PATH] = {NULL};
	WCHAR szSystemDirectory[MAX_PATH] = {NULL};

	GetWindowsDirectory(szWindowsDirectory,MAX_PATH);
	GetSystemDirectory(szSystemDirectory,MAX_PATH);

	WCHAR szDirectoryInfo[1024] = {NULL};

	swprintf(szDirectoryInfo,L"WinDir:    \t%s\r\n",szWindowsDirectory);
	wprintfex(
		szDirectoryInfo,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	swprintf(szDirectoryInfo,L"SysDir:    \t%s\r\n",szSystemDirectory);
	wprintfex(
		szDirectoryInfo,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	//
	//Check Memory info
	//
	wprintf(L"\r\nChecking Memory info...\r\n");

    MEMORYSTATUS mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatus(&mem);

	WCHAR szMemoryInfo[1024] = {NULL};

	swprintf(szMemoryInfo,L"PhysMem:   \t%d MB\r\n",mem.dwTotalPhys/1024/1024);
	wprintfex(
		szMemoryInfo,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	swprintf(szMemoryInfo,L"PhysFree:  \t%d MB\r\n",mem.dwAvailPhys/1024/1024);
	wprintfex(
		szMemoryInfo,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	swprintf(szMemoryInfo,L"VirtualMem:\t%d MB\r\n",mem.dwTotalVirtual/1024/1024);
	wprintfex(
		szMemoryInfo,
		FOREGROUND_INTENSITY | FOREGROUND_BLUE);

	//
	//Check Drive info
	//
	wprintf(L"\r\nChecking Drive info...\r\n");

	SetErrorMode(SEM_FAILCRITICALERRORS);

	for(int i=0;i<26;++i)
	{
		UINT uType = DRIVE_UNKNOWN;
		WCHAR szDisk[3] = {NULL};
		WCHAR szDriveType[32] = {NULL};
		ULARGE_INTEGER FreeBytesAvailable,TotalNumberOfBytes,TotalNumberOfFreeBytes;
		unsigned long TotalSpace,FreeSpace;

		swprintf(szDisk,L"%c:",'A'+i);
		uType=GetDriveType(szDisk);

		switch(uType)
		{
			case DRIVE_UNKNOWN:
				{
					lstrcpy(szDriveType,L"UNKNOWN");
				}break;
			case DRIVE_NO_ROOT_DIR:
				{
					lstrcpy(szDriveType,L"NO_ROOT_DIR");
				}break;
			case DRIVE_REMOVABLE:
				{
					lstrcpy(szDriveType,L"REMOVABLE");
				}break;
			case DRIVE_REMOTE:
				{
					lstrcpy(szDriveType,L"REMOTE");
				}break;
			case DRIVE_CDROM:
				{
					lstrcpy(szDriveType,L"CDROM");
				}break;
			case DRIVE_RAMDISK:
				{
					lstrcpy(szDriveType,L"RAMDISK");
				}break;
			case DRIVE_FIXED:
				{
					lstrcpy(szDriveType,L"FIXED");

					GetDiskFreeSpaceEx(szDisk,&FreeBytesAvailable,&TotalNumberOfBytes,&TotalNumberOfFreeBytes);
					TotalSpace = TotalNumberOfBytes.QuadPart/(1024*1024);
					FreeSpace = TotalNumberOfFreeBytes.QuadPart/(1024*1024);
				}break;
			default:
				{
					lstrcpy(szDriveType,L"UNKNOWN");
				}break;
		}

		WCHAR szDriveInfo[1024] = {NULL};
		if( uType!=DRIVE_NO_ROOT_DIR )
		{
			if( uType!= DRIVE_FIXED )
			{
				swprintf(szDriveInfo,L"[%s]        \t[%s]\r\n",szDisk,szDriveType);
				wprintfex(
					szDriveInfo,
					FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			}
			else
			{
				swprintf(
					szDriveInfo,
					L"[%s]        \t[%s] TotalSpace:%6ldMB(%4.1fGB) FreeSpace:%6ldMB(%4.1fGB)\r\n",
					szDisk,
					szDriveType,
					TotalSpace,
					float(TotalSpace)/1024,
					FreeSpace,
					float(FreeSpace)/1024
					);
				wprintfex(
					szDriveInfo,
					FOREGROUND_INTENSITY | FOREGROUND_BLUE);
			}
		}
	}

	//
	//Check Network info
	//
	wprintf(L"\r\nChecking Network info...\r\n");

	IP_ADAPTER_INFO *IpAdaptersInfo =NULL;
	IP_ADAPTER_INFO *IpAdaptersInfoHead =NULL;

	IpAdaptersInfo = (IP_ADAPTER_INFO*) GlobalAlloc(GPTR,sizeof(IP_ADAPTER_INFO));
	if( IpAdaptersInfo == NULL )
	{
		return 0;
	}

	DWORD dwDataSize = sizeof(IP_ADAPTER_INFO);
	DWORD dwRetVal = GetAdaptersInfo(IpAdaptersInfo,&dwDataSize);

	if( ERROR_SUCCESS != dwRetVal )
	{   
		GlobalFree(IpAdaptersInfo);
		IpAdaptersInfo = NULL;

		if( ERROR_BUFFER_OVERFLOW == dwRetVal )
		{
			IpAdaptersInfo = (IP_ADAPTER_INFO*) GlobalAlloc(GPTR,dwDataSize);
			if( IpAdaptersInfo == NULL )
			{
				return 0;
			}
			if( ERROR_SUCCESS != GetAdaptersInfo(IpAdaptersInfo,&dwDataSize) )
			{
				GlobalFree(IpAdaptersInfo);
				return 0;
			}
		}
		else
		{
			return 0;
		}
	}

	IpAdaptersInfoHead = IpAdaptersInfo;

	//Count the number of NIC
	int iNumberOfNIC = 0;
	do
	{
		if( IpAdaptersInfo->Type == MIB_IF_TYPE_ETHERNET )
		{
			iNumberOfNIC++;

			char strAdapterInfo[1024] = {NULL};
			sprintf(
				strAdapterInfo,
				"[NIC]      \t[%d]: %s\r\n           \tIP Address:[%s]\r\n           \tSubnet Mask:[%s]\r\n           \tDefault Gateway:[%s]\r\n",
				iNumberOfNIC,
				IpAdaptersInfo->Description,
				IpAdaptersInfo->IpAddressList.IpAddress.String,
				IpAdaptersInfo->IpAddressList.IpMask.String,
				IpAdaptersInfo->GatewayList.IpAddress.String
				);
			printfex(strAdapterInfo,FOREGROUND_INTENSITY | FOREGROUND_BLUE);

			strcpy(strAdapterInfo,"           \tPhysical Address:[");
			char strMACAddress[32] = {NULL};
			for(int i = 0; i < IpAdaptersInfo->AddressLength; i++)
			{
				if( i == (IpAdaptersInfo->AddressLength - 1) )
				{
					sprintf(strMACAddress, "%02X", (int) IpAdaptersInfo->Address[i]);
				}
				else
				{
					sprintf(strMACAddress, "%02X-", (int) IpAdaptersInfo->Address[i]);
				}
				strcat(strAdapterInfo,strMACAddress);
			}
			strcat(strAdapterInfo,"]\r\n");
			printfex(strAdapterInfo,FOREGROUND_INTENSITY | FOREGROUND_BLUE);
		}
		IpAdaptersInfo = IpAdaptersInfo->Next;
	}
	while(IpAdaptersInfo);

	if( IpAdaptersInfoHead != NULL )   
	{
		GlobalFree(IpAdaptersInfoHead);
	}

	return 1;
}

int AdjustPrivileges(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 1;

	wprintfex(
		L"Adjusting privileges...Please wait for a moment...\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN|FOREGROUND_BLUE);

	wprintf(L"Adjusting privileges...\r\n\r\n");

	//
	//Adjust privileges
	//
	//if( EnableSpecificPrivilege(TRUE,SE_ASSIGNPRIMARYTOKEN_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_ASSIGNPRIMARYTOKEN_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}//SYSTEM
	//if( EnableSpecificPrivilege(TRUE,SE_AUDIT_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_AUDIT_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}//SYSTEM
	if( EnableSpecificPrivilege(TRUE,SE_BACKUP_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_BACKUP_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_CHANGE_NOTIFY_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_CHANGE_NOTIFY_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_CREATE_PAGEFILE_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_CREATE_PAGEFILE_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	//if( EnableSpecificPrivilege(TRUE,SE_CREATE_PERMANENT_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_CREATE_PERMANENT_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}//SYSTEM
	//if( EnableSpecificPrivilege(TRUE,SE_CREATE_TOKEN_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_CREATE_TOKEN_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}//SYSTEM
	if( EnableSpecificPrivilege(TRUE,SE_DEBUG_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_DEBUG_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_INC_BASE_PRIORITY_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_INC_BASE_PRIORITY_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_INCREASE_QUOTA_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_INCREASE_QUOTA_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_LOAD_DRIVER_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_LOAD_DRIVER_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	//if( EnableSpecificPrivilege(TRUE,SE_LOCK_MEMORY_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_LOCK_MEMORY_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}//SYSTEM
	if( EnableSpecificPrivilege(TRUE,SE_PROF_SINGLE_PROCESS_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_PROF_SINGLE_PROCESS_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	//if( EnableSpecificPrivilege(TRUE,SE_REMOTE_SHUTDOWN_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_REMOTE_SHUTDOWN_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}
	if( EnableSpecificPrivilege(TRUE,SE_RESTORE_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_RESTORE_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SECURITY_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_SECURITY_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SHUTDOWN_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_SHUTDOWN_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_SYSTEM_ENVIRONMENT_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_SYSTEM_ENVIRONMENT_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	//if( EnableSpecificPrivilege(TRUE,SE_SYSTEM_PROFILE_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_SYSTEM_PROFILE_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}
	if( EnableSpecificPrivilege(TRUE,SE_SYSTEMTIME_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_SYSTEMTIME_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	if( EnableSpecificPrivilege(TRUE,SE_TAKE_OWNERSHIP_NAME) == FALSE )
	{
		wprintfex(L"Enable [SE_TAKE_OWNERSHIP_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
		iRet = -1;
	}
	//if( EnableSpecificPrivilege(TRUE,SE_TCB_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_TCB_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}//SYSTEM
	//if( EnableSpecificPrivilege(TRUE,SE_UNSOLICITED_INPUT_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_UNSOLICITED_INPUT_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}
	//if( EnableSpecificPrivilege(TRUE,SE_MACHINE_ACCOUNT_NAME) == FALSE )
	//{
	//	wprintfex(L"Enable [SE_MACHINE_ACCOUNT_NAME] privilege failed!\r\n",FOREGROUND_INTENSITY | FOREGROUND_RED);
	//	iRet = -1;
	//}

	return iRet;
}

int CheckEnvironment(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 0;

	wprintfex(
		L"Checking environment...Please wait for a moment...\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN|FOREGROUND_BLUE);

	//
	//Check process
	//
	wprintf(L"Checking process...\r\n\r\n");

	int iOpenProcError = 0;
	HANDLE hSnapshot = NULL;
	PROCESSENTRY32 pe32;
	hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if( hSnapshot == NULL )
	{
		return -1;
	}
	pe32.dwSize = sizeof( PROCESSENTRY32 );

	Process32First( hSnapshot, &pe32 );
	do
	{
		//Ignore [current process]\[NTOSKRNL]
		if( pe32.th32ProcessID == GetCurrentProcessId() ||
			pe32.th32ProcessID == 0
			)
		{
			continue;
		}

		//Call OpenProcess to get the handle of process
		HANDLE hProc = NULL;
		hProc = OpenProcess( PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE,FALSE,pe32.th32ProcessID);	
		if( !hProc )
		{
			iOpenProcError++;

			WCHAR szOpenProcErrorInfo[1024] = {NULL};
			WCHAR szProcessPath[MAX_PATH]={0};
			lstrcpy(szProcessPath,L"Unknown");
			GetProcessPath( pe32.th32ProcessID,szProcessPath,sizeof(szProcessPath),TRUE );

			swprintf(
				szOpenProcErrorInfo,
				L"[Process]  \tPID = [%d] ImagePath = [%s]\r\n",
				pe32.th32ProcessID,
				szProcessPath
				);
			wprintfex(
				szOpenProcErrorInfo,
				FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
		CloseHandle(hProc);
	}
	while( Process32Next( hSnapshot, &pe32 ) );
	CloseHandle( hSnapshot );

	if( iOpenProcError > 0 )
	{
		wprintfex(
			L"\r\nSlavyana may not work well with this(these) process!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		iRet = -1;
	}

	//
	//Check module
	//
	wprintf(L"Checking module...\r\n\r\n");

	int iModuleError = 0;
	HANDLE hModuleSnapshot= NULL;
	MODULEENTRY32 moudle;

	hModuleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,GetCurrentProcessId());
	moudle.dwSize = sizeof(MODULEENTRY32);
	Module32First(hModuleSnapshot,&moudle);
	do
	{
		//Ignore current module
		if( moudle.hModule == GetModuleHandle(NULL) )
		{
			continue;
		}

		//Verify signature
		if( CheckFileTrust(moudle.szExePath) != 1 )
		{
			iModuleError++;

			WCHAR szModuleErrorInfo[1024] = {NULL};
			swprintf(
				szModuleErrorInfo,
				L"[Module]   \tName = [%s]\r\n           \tImagePath = [%s]\r\n",
				moudle.szModule,
				moudle.szExePath
				);
			wprintfex(
				szModuleErrorInfo,
				FOREGROUND_INTENSITY | FOREGROUND_RED);
		}
	}
	while( Module32Next(hModuleSnapshot,&moudle) );

	CloseHandle(hModuleSnapshot);

	if( iModuleError > 0 )
	{
		wprintfex(
			L"\r\nSlavyana may not work well with this(these) module!\r\n\r\n",
			FOREGROUND_INTENSITY | FOREGROUND_RED);

		iRet = -1;
	}

	return iRet;
}

int SelfCheck(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	wprintfex(
		L"Self checking...Please wait for a moment...\r\n\r\n",
		FOREGROUND_INTENSITY | FOREGROUND_GREEN|FOREGROUND_BLUE);

	//
	//Self check
	//
	wprintf(L"Self checking...\r\n\r\n");

	return 1;
}