#pragma once
#include "header.h"

#define STATUS_SUCCESS 0x00000000
#define STATUS_UNSUCCESSFUL 0xC0000001
#define STATUS_ACCESS_DENIED 0xC0000022

typedef struct _CLIENT_ID {
	PVOID UniqueProcess;
	PVOID UniqueThread;
}CLIENT_ID, *PCLIENT_ID;

typedef NTSTATUS(NTAPI *pNtOpenProcess)(PHANDLE ProcessHandle, ACCESS_MASK DesiredAccess, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID ClientId);
typedef NTSTATUS(NTAPI *pNtOpenThread)(PHANDLE ThreadHandle, ACCESS_MASK AccessMask, POBJECT_ATTRIBUTES ObjectAttributes, PCLIENT_ID);
typedef NTSTATUS(NTAPI *pNtSuspendThread)(HANDLE ThreadHandle, PULONG SuspendCount);
typedef NTSTATUS(NTAPI *pNtAlertResumeThread)(HANDLE ThreadHandle, PULONG SuspendCount);
typedef NTSTATUS(NTAPI *pNtAllocateVirtualMemory)(HANDLE ProcessHandle, PVOID *BaseAddress, ULONG_PTR ZeroBits, PSIZE_T RegionSize, ULONG AllocationType, ULONG Protect);
typedef NTSTATUS(NTAPI *pNtWriteVirtualMemory)(HANDLE ProcessHandle, PVOID BaseAddress, PVOID Buffer, ULONG NumberOfBytesToWrite, PULONG NumberOfBytesWritten);
typedef NTSTATUS(NTAPI *pNtQueueApcThread)(HANDLE ThreadHandle, PIO_APC_ROUTINE ApcRoutine, PVOID ApcRoutineContext OPTIONAL, PIO_STATUS_BLOCK ApcStatusBlock OPTIONAL, ULONG ApcReserved OPTIONAL);

HANDLE NtOpenProcess(DWORD dwProcessId);
HANDLE NtOpenThread(DWORD dwProcessId, DWORD dwThreadId);