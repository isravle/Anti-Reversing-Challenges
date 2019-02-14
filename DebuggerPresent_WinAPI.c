#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>


BOOL IsDdbgPresent()
{
	/* Determines whether the calling process is being debugged by a user-mode debugger. 
	mov eax,dword ptr fs:[30]		<-- The Process Environment Block (PEB)
	movzx eax,byte ptr ds:[eax+2]	<--0x002 BYTE BeingDebugged;
	ret
	*/
	return IsDebuggerPresent();
}

BOOL CheckRemoteDbgPresent()
{
	/* Determines whether the specified process is being debugged
	However, we can also use this for checking if our own process is being debugged. it calls the NTDLL export

	CheckRemoteDbgPresent use ZwQueryInformationProcess with the SYSTEM_INFORMATION_CLASS set to 7 (ProcessDebugPort)
	*/

	BOOL IsDbgPresent;
	CheckRemoteDebuggerPresent(GetCurrentProcess(), &IsDbgPresent);
	return IsDbgPresent;
}


int main() {
	const char *GoodBoy = "-> Good!\r\n";
	const char *BadBoy = "-> DETECTED!!!!!\r\n";
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);



	// IsDebuggerPresent
	printf("Checking for debugger using IsDebuggerPresent()\r\n");
	if (IsDdbgPresent()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);

	// NCheckRemoteDebuggerPresent
	printf("Checking for debugger using CheckRemoteDebuggerPresent()\r\n");
	if (CheckRemoteDbgPresent()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);


	getchar();
	return 0;
}