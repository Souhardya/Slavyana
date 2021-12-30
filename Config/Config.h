#pragma once



//
//Define
//



//
//Install
//
#define CONF_SoftwareRegName L"Slavyana"
#define CONF_SandBoxNameLength 16



//
//Registry [SandBox]
//
#define CONF_SoftwareReg_SandBox L"SandBox"

#define CONF_SoftwareReg_SandBox_View L"View"
#define CONF_SoftwareReg_SandBox_Restore L"Restore"
#define CONF_SoftwareReg_SandBox_CleanUp L"CleanUp"
#define CONF_SoftwareReg_SandBox_Begin L"Begin"
#define CONF_SoftwareReg_SandBox_Stop L"Stop"
#define CONF_SoftwareReg_SandBox_File L"File"
#define CONF_SoftwareReg_SandBox_Limit L"Limit"
#define CONF_SoftwareReg_SandBox_Resource L"Resource"
#define CONF_SoftwareReg_SandBox_Software L"Software"

#define CONF_SoftwareReg_SandBox_Registry L"Registry"

#define CONF_SoftwareReg_SandBox_SYNC L"SYNC"
#define CONF_SoftwareReg_SandBox_SYNC_PROC L"PROC"

#define CONF_SoftwareReg_SandBox_HKEY_CLASSES_ROOT L"ROOT"
#define CONF_SoftwareReg_SandBox_HKEY_CURRENT_USER L"USER"
#define CONF_SoftwareReg_SandBox_HKEY_LOCAL_MACHINE L"MACHINE"
#define CONF_SoftwareReg_SandBox_HKEY_USERS L"USER"
#define CONF_SoftwareReg_SandBox_HKEY_CURRENT_CONFIG L"CONFIG"

#define CONF_SoftwareReg_SandBox_DeletedFileMark 122913408005200000//1990.07.02-05:20:00

#define CONF_SandBox_RegHive_FileName L"RegHive.hiv"
#define CONF_SandBox_RegHive_UpdateName L"UpdateRegHive.hiv"

#define CONF_SandBox_Drive_Name L"Drive"



//
//Registry [Config]
//
#define CONF_SoftwareReg_Config L"Config"
#define CONF_SoftwareReg_Config_RestorePath L"RestorePath"



//
//Launcher
//
#define CONF_Launcher_FileName L"Launcher.exe"

#define CONF_Launcher_Startup_TimeOut 500

#define CONF_Launcher_StartupPath_ValueName L"StartupPath"
#define CONF_Launcher_StartupInfo_ValueName L"StartupInfo"



//
//HookPort
//
#define CONF_HookPort_FileName L"HookPort.dll"
#define CONF_HookPort_MaxProcessCount 256

#define MAX_NAME MAX_PATH*2



//
//CloneAPI
//
#define CONF_CloneAPI_DirectoryName L"CloneAPI"

#define CONF_CloneAPI_CloneKERNEL32 L"clone_kernel32.dll"
#define CONF_CloneAPI_CloneFLTLIB L"clone_fltlib.dll"
#define CONF_CloneAPI_CloneSHELL32 L"clone_shell32.dll"
#define CONF_CloneAPI_CloneADVAPI32 L"clone_advapi32.dll"