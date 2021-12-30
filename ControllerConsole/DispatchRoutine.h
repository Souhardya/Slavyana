#pragma once

int DispatchRoutineEntry();

int ShowDispatch(IN int lpInstalled,IN BOOL bIsShowNotice);

int RoutineBegin(void);

int RoutineStop(void);

int RoutineControl(void);

int RoutineInstall(void);

int RoutineRemove(void);

int RoutineConfig(void);

int RoutineHelp(void);

int RoutineExit(void);