#include "header.h"
using namespace std;

int main()
{
	HANDLE ProcessHandle = NtOpenProcess(dwRetProcessId("notepad.exe"));
	if (ProcessHandle)
	{
		ApcInjectDll(ProcessHandle, TRUE, "C:\\opcode.dll");
	}
	getchar();
	return 0;
}