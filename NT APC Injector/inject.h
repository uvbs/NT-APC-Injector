#pragma once
#include "header.h"

BOOL ApcInjectDll(HANDLE ProcessHandle, BOOL bSuspendWake, char *DllPath);