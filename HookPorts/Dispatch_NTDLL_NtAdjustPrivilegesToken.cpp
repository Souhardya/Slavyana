#include <windows.h>
#include <ntsecapi.h>

#include "./Dispatch_NTDLL_NtAdjustPrivilegesToken.h"
#include "../Common/DebugLog.h"
#include "./HookHelp.h"
#include "./GetProcAddressEx.h"
#include "./Dispatch_NTDLL_NtResumeThread.h"



//
//Global
//
__pfnNtAdjustPrivilegesToken pfnNtAdjustPrivilegesToken = NULL;



//
//Dispatch_NTDLL_NtAdjustPrivilegesToken Functions
//
NTSTATUS
NTAPI
OnNtAdjustPrivilegesToken(
	IN HANDLE               TokenHandle,
	IN BOOLEAN              DisableAllPrivileges,
	IN PTOKEN_PRIVILEGES    TokenPrivileges,
	IN ULONG                PreviousPrivilegesLength,
	OUT PTOKEN_PRIVILEGES   PreviousPrivileges OPTIONAL,
	OUT PULONG              RequiredLength OPTIONAL)
{
	NTSTATUS nRet;

	DWORD dwRetAddr = 0;
	__asm
	{
		mov eax, [ebp+4];
		sub eax, 5;
		mov dwRetAddr, eax;
	}
	if( IsBypassCaller(dwRetAddr) )
	{
		nRet = pfnNtAdjustPrivilegesToken(
			TokenHandle,
			DisableAllPrivileges,
			TokenPrivileges,
			PreviousPrivilegesLength,
			PreviousPrivileges,
			RequiredLength
			);

		return nRet;
	}

	//
	//Check if is enable Privileges request
	//
	BOOL bBypass = FALSE;

	if( TokenPrivileges->PrivilegeCount == 1 )
	{
		if( TokenPrivileges->Privileges[0].Attributes == SE_PRIVILEGE_ENABLED || 
			TokenPrivileges->Privileges[0].Attributes == SE_PRIVILEGE_ENABLED_BY_DEFAULT
			)
		{
			LONG PrivilegeType = TokenPrivileges->Privileges[0].Luid.LowPart;

			switch( PrivilegeType )
			{
				case SE_CREATE_TOKEN_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_ASSIGNPRIMARYTOKEN_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_LOCK_MEMORY_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_INCREASE_QUOTA_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_MACHINE_ACCOUNT_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_TCB_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_SECURITY_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_TAKE_OWNERSHIP_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_LOAD_DRIVER_PRIVILEGE:
					{
						bBypass = FALSE;
					}break;

				case SE_SYSTEM_PROFILE_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_SYSTEMTIME_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_PROF_SINGLE_PROCESS_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_INC_BASE_PRIORITY_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_CREATE_PAGEFILE_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_CREATE_PERMANENT_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_BACKUP_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_RESTORE_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_SHUTDOWN_PRIVILEGE:
					{
						bBypass = FALSE;
					}break;

				case SE_DEBUG_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_AUDIT_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_SYSTEM_ENVIRONMENT_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_CHANGE_NOTIFY_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_REMOTE_SHUTDOWN_PRIVILEGE:
					{
						bBypass = FALSE;
					}break;

				case SE_UNDOCK_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_SYNC_AGENT_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				case SE_ENABLE_DELEGATION_PRIVILEGE:
					{
						bBypass = TRUE;
					}break;

				default:
					{
						bBypass = FALSE;
					}break;
			}
		}
	}

	//
	//Check if bypass request
	//
	if( bBypass == FALSE )
	{
		nRet = STATUS_UNSUCCESSFUL;
		return nRet;
	}

	//ULONG pri = TokenPrivileges->Privileges[0].Attributes;
	//LUID Luid = TokenPrivileges->Privileges[0].Luid;
	//DWORD PrivilegeCount = TokenPrivileges->PrivilegeCount;
	//DWORD Type = Luid.LowPart;

//#ifdef Dbg
//	WCHAR szOut[256] = {0};
//	wsprintf(szOut,L"\r\nPrivilegeCount = [%d] pri = [%d] Type = [%d]",PrivilegeCount,pri,Type);
//	OutputDebugString(szOut);
//#endif

	nRet = pfnNtAdjustPrivilegesToken(
		TokenHandle,
		DisableAllPrivileges,
		TokenPrivileges,
		PreviousPrivilegesLength,
		PreviousPrivileges,
		RequiredLength
		);

	return nRet;
}

NTSTATUS
NTAPI
NtAdjustPrivilegesToken(
	IN HANDLE               TokenHandle,
	IN BOOLEAN              DisableAllPrivileges,
	IN PTOKEN_PRIVILEGES    TokenPrivileges,
	IN ULONG                PreviousPrivilegesLength,
	OUT PTOKEN_PRIVILEGES   PreviousPrivileges OPTIONAL,
	OUT PULONG              RequiredLength OPTIONAL)
{
	NTSTATUS ret;
	HINSTANCE hDll = GetModuleHandleW(L"ntdll.dll");
	__pfnNtAdjustPrivilegesToken addFun = (__pfnNtAdjustPrivilegesToken)GetProcAddress(hDll,"NtAdjustPrivilegesToken");
	if(addFun)
	{
		ret = addFun(TokenHandle,DisableAllPrivileges,TokenPrivileges,PreviousPrivilegesLength,PreviousPrivileges,RequiredLength);
	}
	return ret;
}