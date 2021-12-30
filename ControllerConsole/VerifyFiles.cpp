#include <windows.h>
#include <stdio.h>
#include <SoftPub.h>

#include "./VerifyFiles.h"



//
//Define
//
typedef HANDLE HCATADMIN;
typedef HANDLE HCATINFO;

typedef BOOL (WINAPI *_CryptCATAdminAcquireContext)( HCATADMIN* phCatAdmin, const GUID* pgSubsystem, DWORD dwFlags );
typedef BOOL (WINAPI *_CryptCATAdminReleaseContext)( HCATADMIN hCatAdmin, DWORD dwFlags );
typedef BOOL (WINAPI *_CryptCATAdminCalcHashFromFileHandle)( HANDLE hFile, DWORD* pcbHash, BYTE* pbHash, DWORD dwFlags );
typedef HCATINFO (WINAPI *_CryptCATAdminEnumCatalogFromHash)( HCATADMIN hCatAdmin, BYTE* pbHash, DWORD cbHash, DWORD dwFlags, HCATINFO* phPrevCatInfo );
typedef BOOL (WINAPI *_CryptCATAdminReleaseCatalogContext)( HCATADMIN hCatAdmin, HCATINFO hCatInfo, DWORD dwFlags );
typedef LONG (WINAPI *_WinVerifyTrust)( HWND hWnd, GUID* pgActionID, WINTRUST_DATA* pWinTrustData );

_CryptCATAdminAcquireContext pCryptCATAdminAcquireContext = NULL;
_CryptCATAdminReleaseContext pCryptCATAdminReleaseContext = NULL;
_CryptCATAdminCalcHashFromFileHandle pCryptCATAdminCalcHashFromFileHandle = NULL;
_CryptCATAdminEnumCatalogFromHash pCryptCATAdminEnumCatalogFromHash = NULL;
_CryptCATAdminReleaseCatalogContext pCryptCATAdminReleaseCatalogContext = NULL;
_WinVerifyTrust pWinVerifyTrust = NULL;

enum
{
	APILOAD_NONE = 0,
	APILOAD_SUCCEED,
	APILOAD_FAILED
} APILOADSTATE;



//
//Global
//
int bApiLoad = APILOAD_NONE;
HANDLE m_hCatAdminContext = NULL;



//
//VerifyFiles Functions
//
DWORD CheckFileTrust(IN LPCWSTR lpFileName)
{
	//Return Value:
	//-1 = error
	//0 = none Signature
	//1 = Microsoft Signature
	//2 = Other Signature

	if(! _LoadFunc())
		return -1;

	if(m_hCatAdminContext == NULL)
	{
		if(! pCryptCATAdminAcquireContext(&m_hCatAdminContext, NULL, 0 ) )
			return -1;
	}

	HRESULT hr = VerifySignatureOfPEFile(lpFileName);
	if(SUCCEEDED(hr))
	{
		return 2;
	}

	BOOL bVerifyed = FALSE;

	if( m_hCatAdminContext )
	{
		LPBYTE Hash;
		DWORD HashSize;

		if(_CatCalcHash(lpFileName, &HashSize, &Hash))
		{
			HANDLE hCatalogContext = pCryptCATAdminEnumCatalogFromHash(m_hCatAdminContext, Hash, HashSize, 0, NULL);
			if(hCatalogContext)
			{
				bVerifyed = TRUE;
				
				pCryptCATAdminReleaseCatalogContext(m_hCatAdminContext, hCatalogContext, 0);
			}

			LocalFree(Hash);
		}
	}

	if( bVerifyed == TRUE )
	{
		return 1;
	}

	return 0;
}

// Verifying the Signature of a PE File
HRESULT VerifySignatureOfPEFile(IN LPCWSTR lpFileName)
{
	if( ! _LoadFunc() )
		return FALSE;

	WINTRUST_DATA WintrustData;
	WINTRUST_FILE_INFO WintrustFileInfo;
	GUID VerifyGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	
	ZeroMemory(&WintrustFileInfo, sizeof(WINTRUST_FILE_INFO));
	WintrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO); 
	WintrustFileInfo.pcwszFilePath = lpFileName; 
	
	ZeroMemory(&WintrustData, sizeof(WINTRUST_DATA));
	WintrustData.cbStruct = sizeof(WINTRUST_DATA);
	WintrustData.dwUnionChoice = WTD_CHOICE_FILE;
	WintrustData.pFile = &WintrustFileInfo;
	WintrustData.dwUIChoice = WTD_UI_NONE;
	WintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
	WintrustData.dwStateAction = WTD_STATEACTION_AUTO_CACHE;
	WintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;
	
	return pWinVerifyTrust(NULL, &VerifyGuid , &WintrustData);
}

BOOL _CatCalcHash(IN LPCWSTR lpFileName, IN DWORD* pcbHash, IN BYTE** ppbHash)//验证Microsoft数字签名
{
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL,	OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD Err;
		LPBYTE Hash;
		DWORD HashSize;
		
		//
		// Start out with a hash buffer size that should be large enough for
		// most requests.
		//
		HashSize = 100;
		
		do
		{
			Hash = (LPBYTE)LocalAlloc(LPTR, HashSize);
			
			if(!Hash)
			{
				Err = ERROR_NOT_ENOUGH_MEMORY;
				break;
			}
			
			if(pCryptCATAdminCalcHashFromFileHandle(hFile, &HashSize, Hash, 0))
			{
				Err = NO_ERROR;
			}
			else
			{
				Err = GetLastError();
				
				//
				// If this API did screw up and not set last error, go ahead
				// and set something.
				//
				if(Err == NO_ERROR)
				{
					Err = ERROR_INVALID_DATA;
				}
				
				LocalFree(Hash);
				
				if(Err != ERROR_INSUFFICIENT_BUFFER)
				{
					//
					// The API failed for some reason other than
					// buffer-too-small.  We gotta bail.
					//
					Hash = NULL;// reset this so we won't try to free it later
					break;
				}
			}
		} while(Err != NO_ERROR);
		
		CloseHandle(hFile);

		if(Err == NO_ERROR)
		{
			*pcbHash = HashSize;
			*ppbHash = Hash;

			return TRUE;
		}
	}

	*pcbHash = 0;
	*ppbHash = NULL;
	return FALSE;
}

BOOL _LoadFunc(void)
{
	if(bApiLoad == APILOAD_NONE)
	{
		HMODULE m_hModule = LoadLibrary(SP_POLICY_PROVIDER_DLL_NAME) ;

		pCryptCATAdminAcquireContext = (_CryptCATAdminAcquireContext)GetProcAddress( m_hModule, "CryptCATAdminAcquireContext" ) ;
		pCryptCATAdminReleaseContext = (_CryptCATAdminReleaseContext)GetProcAddress( m_hModule, "CryptCATAdminReleaseContext" ) ;
		pCryptCATAdminCalcHashFromFileHandle = (_CryptCATAdminCalcHashFromFileHandle)GetProcAddress( m_hModule , "CryptCATAdminCalcHashFromFileHandle" );
		pCryptCATAdminEnumCatalogFromHash = (_CryptCATAdminEnumCatalogFromHash)GetProcAddress( m_hModule , "CryptCATAdminEnumCatalogFromHash" );
		pCryptCATAdminReleaseCatalogContext = (_CryptCATAdminReleaseCatalogContext)GetProcAddress( m_hModule , "CryptCATAdminReleaseCatalogContext" ) ;
		pWinVerifyTrust = ( _WinVerifyTrust )GetProcAddress( m_hModule , "WinVerifyTrust" ) ;

		if( pCryptCATAdminAcquireContext && pCryptCATAdminReleaseContext && pCryptCATAdminCalcHashFromFileHandle &&
			pCryptCATAdminEnumCatalogFromHash && pCryptCATAdminReleaseCatalogContext && pWinVerifyTrust )
		{
			bApiLoad = APILOAD_SUCCEED;
			return TRUE;
		}

		bApiLoad = APILOAD_FAILED;
	}

	return (bApiLoad == APILOAD_SUCCEED);
}