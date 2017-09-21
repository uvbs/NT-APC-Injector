#include "header.h"

DWORD dwRetProcessId(std::string processname)
{
	HANDLE hProcess;
	PROCESSENTRY32 processEntry32;
	processEntry32.dwSize = sizeof(PROCESSENTRY32);
	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	do {
		if (!strcmp(processEntry32.szExeFile, processname.c_str()))
		{
			CloseHandle(hProcess);
			return processEntry32.th32ProcessID;
		}
	} while (Process32Next(hProcess, &processEntry32));
	CloseHandle(hProcess);
	return 0;
}