#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>


BOOL NtSetInformationThread_ThreadHideFromDebugger()
{
	/* Calling NtSetInformationThread will attempt with ThreadInformationClass set to x11 (ThreadHideFromDebugger)
	to hide a thread from the debugger, Passing NULL for hThread will cause the function to hide the thread the
	function is running in. Also, the function returns false on failure and true on success. When  the  function
	is called, the thread will continue  to run but a debugger will no longer receive any events related  to  that  thread. 
	-
	In Windows 2000, the guys behind Windows introduced a new class to be passed into NtSetInformationThread, and it was named HideThreadFromDebugger. It is the first anti-debugging API implemented by Windows, and is very powerful. The class prevents debuggers from receiving events from any thread that has had NtSetInformationThread with the HideThreadFromDebugger class called on it. These events include breakpoints, and the exiting of the program if it is called on the main thread of an application. Here is the HideThread function:
	*/
	typedef NTSTATUS(WINAPI *pNtSetInformationThread)(IN HANDLE, IN UINT, IN PVOID, IN ULONG);

	const int ThreadHideFromDebugger = 0x11;
	pNtSetInformationThread NtSetInformationThread = NULL;

	NTSTATUS Status;
	BOOL IsBeingDebug = FALSE;

	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));
	NtSetInformationThread = (pNtSetInformationThread)GetProcAddress(hNtDll, "NtSetInformationThread");
	Status = NtSetInformationThread(GetCurrentThread(), ThreadHideFromDebugger, NULL, 0);

	if (Status)
		IsBeingDebug = TRUE;

	return IsBeingDebug;
}


int main() {
	const char *Junk = "Try to debug this loop\r\n";
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);


	// NtSetInformationThread->HideThreadFromDebugger
	printf("Checking for debugger using NtSetInformationThread->HideThreadFromDebugger\r\n");
	NtSetInformationThread_ThreadHideFromDebugger();

	while (true)
	{
		printf("%s", Junk);
		Sleep(1500);
	}

	getchar();
	return 0;
}