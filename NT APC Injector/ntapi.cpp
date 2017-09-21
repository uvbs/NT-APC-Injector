#include "header.h"

HANDLE NtOpenProcess(DWORD dwProcessId)
{
	FARPROC fpNtOpenProcess = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtOpenProcess");
	if (fpNtOpenProcess && dwProcessId != NULL)
	{
		HANDLE ProcessHandle = 0;
		CLIENT_ID ClientId;
		OBJECT_ATTRIBUTES ObjectAttributes;
		InitializeObjectAttributes(&ObjectAttributes, NULL, NULL, NULL, NULL);
		ClientId.UniqueProcess = (PVOID)dwProcessId;
		ClientId.UniqueThread = (PVOID)0;
		pNtOpenProcess fNtOpenProcess = (pNtOpenProcess)fpNtOpenProcess;
		if (NT_SUCCESS(fNtOpenProcess(&ProcessHandle, MAXIMUM_ALLOWED, &ObjectAttributes, &ClientId)))
		{
			return ProcessHandle;
		}
	}
	return 0;
}

HANDLE NtOpenThread(DWORD dwProcessId, DWORD dwThreadId)
{
	FARPROC fpNtOpenThread = GetProcAddress(GetModuleHandle("ntdll.dll"), "NtOpenThread");
	if (fpNtOpenThread)
	{
		HANDLE ThreadHandle = 0;
		OBJECT_ATTRIBUTES ObjectAttributes;
		CLIENT_ID ClientId;
		InitializeObjectAttributes(&ObjectAttributes, NULL, NULL, NULL, NULL);
		ClientId.UniqueProcess = (PVOID)dwProcessId;
		ClientId.UniqueThread = (PVOID)dwThreadId;
		pNtOpenThread fNtOpenThread = (pNtOpenThread)fpNtOpenThread;
		if (NT_SUCCESS(fNtOpenThread(&ThreadHandle, MAXIMUM_ALLOWED, &ObjectAttributes, &ClientId)))
		{
			if (ThreadHandle)
			{
				return ThreadHandle;
			}
		}
	}
	return 0;
}