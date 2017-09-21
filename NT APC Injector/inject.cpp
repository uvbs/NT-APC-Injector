#include "header.h"
using namespace std;

BOOL ApcInjectDll(HANDLE ProcessHandle, BOOL bSuspendWake, char *DllPath)
{
	NTSTATUS NtStatus;
	DWORD dwProcessId = GetProcessId(ProcessHandle);
	PVOID pvDllMemory = 0; // used later for memory allocation
	SIZE_T sDllLength = strlen(DllPath); // calculation for DLL path lenth
	SIZE_T NumberOfBytesWritten = 0; // used for memory write

	// check if the process handle exists
	if (!ProcessHandle || !dwProcessId)
	{
		return FALSE;
	}

	// array of function addresses
	FARPROC fpAddresses[6] = {
		GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"),
		GetProcAddress(GetModuleHandle("ntdll.dll"), "NtAllocateVirtualMemory"),
		GetProcAddress(GetModuleHandle("ntdll.dll"), "NtWriteVirtualMemory"),
		GetProcAddress(GetModuleHandle("ntdll.dll"), "NtSuspendThread"),
		GetProcAddress(GetModuleHandle("ntdll.dll"), "NtAlertResumeThread"),
		GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueueApcThread")
	};

	// loop through the array
	for (FARPROC &fpAddress : fpAddresses)
	{
		if (!fpAddress) // address couldn't be obtained
		{
			// return nothing
			return 0;
		}
	}

	// setup for NTAPI functions
	pNtAllocateVirtualMemory fNtAllocateVirtualMemory = (pNtAllocateVirtualMemory)fpAddresses[1];
	pNtWriteVirtualMemory fNtWriteVirtualMemory = (pNtWriteVirtualMemory)fpAddresses[2];
	pNtSuspendThread fNtSuspendThread = (pNtSuspendThread)fpAddresses[3];
	pNtAlertResumeThread fNtAlertResumeThread = (pNtAlertResumeThread)fpAddresses[4];
	pNtQueueApcThread fNtQueueApcThread = (pNtQueueApcThread)fpAddresses[5];

	// allocate memory with NtAllocateVirtualMemory
	if (NT_SUCCESS(fNtAllocateVirtualMemory(ProcessHandle, &pvDllMemory, NULL, &sDllLength, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE)))
	{

		// write to allocated memory with NtWriteVirtualMemory . store DLL file path
		if (NT_SUCCESS(fNtWriteVirtualMemory(ProcessHandle, pvDllMemory, DllPath, sDllLength, NULL)))
		{

			try {

				HANDLE ThreadHandle, hThreadHandle;
				THREADENTRY32 ThreadEntry32;
				ThreadEntry32.dwSize = sizeof(THREADENTRY32);
				hThreadHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);

				// loop threads
				do {
					// check if the thread is within the target process we want to inject into
					if (ThreadEntry32.th32OwnerProcessID == dwProcessId)
					{
						// open thread handle with NtOpenThread
						ThreadHandle = NtOpenThread(dwProcessId, ThreadEntry32.th32ThreadID);

						// check if handle could be opened
						if (ThreadHandle)
						{

							if (bSuspendWake)
							{
								NtStatus = fNtSuspendThread(ThreadHandle, NULL);
								if (!NT_SUCCESS(NtStatus))
								{
									goto cleanup;
								}
							}

							NtStatus = fNtQueueApcThread(ThreadHandle, (PIO_APC_ROUTINE)fpAddresses[0], pvDllMemory, NULL, NULL);

							if(bSuspendWake)
							{
								NtStatus = fNtAlertResumeThread(ThreadHandle, NULL);
								if (!NT_SUCCESS(NtStatus))
								{
									goto cleanup;
								}
							}
						}

						// cleanup
						CloseHandle(ThreadHandle);
					}
				} while (Thread32Next(hThreadHandle, &ThreadEntry32)); 
			}
			catch (...)
			{
				goto cleanup;
			}
		}
	}

	// cleanup
cleanup:
	if (pvDllMemory)
	{
		VirtualFreeEx(ProcessHandle, pvDllMemory, sDllLength, MEM_RELEASE);
	}
	return FALSE;
}