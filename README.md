# NT APC Injector

bonjour

This project demonstrates how you can use Asynchronous Procedure Calls (APC) to inject a DLL into another process; this project uses the NTAPI functions instead of Win32 API (NtQueueApcThread instead of QueueUserAPC and NtOpenThread instead of OpenThread). There is also an additional feature which supports suspending the thread (NtSuspendThread) prior to the APC injection and then resuming the thread with NtAlertResumeThread (wakes it up for APC to be executed - all optional though).

I am not responsible for how you use this project, use it responsibly please.

Merci bien