#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>


BOOL Manual_PEB_BeingDebugged()
{
	/* WinAPI of IsDebuggerPresent perform the same actions,
	Access to PEB of current process and reads a byte value of BeingDebugged. */

	char IsDbg = 0;
	__asm {
		mov eax, fs:[30h] // Location of PEB Struct
		mov al, [eax + 2h] // location of 0x00 2BYTE BeingDebugged;
		mov IsDbg, al
	}

	return IsDbg;
}


BOOL PEB_NtGlobalFlag()
{
	/* NtGlobalFlag is a value inside the process PEB which contains many flags which points if the process is being debugged.
	if process is not being debugged, the NtGlobalFlag value  == 0x0,
	else, the following flags are set, 0x10+0x20+0x40==0x70
	Flags:
	FLG_HEAP_ENABLE_TAIL_CHECK (0x10),
	FLG_HEAP_ENABLE_FREE_CHECK(0x20),
	FLG_HEAP_VALIDATE_PARAMETERS(0x40),
	IF value == 0x70, proc under debugger.*/


	unsigned long NtGlobalFlags = 0;
	__asm
	{
		mov eax, fs:[30h] // Location of PEB Struct
		mov eax, [eax + 68h] // location of 0x068 DWORD NtGlobalFlag;
		mov NtGlobalFlags, eax
	}

	if (NtGlobalFlags==0x70)
		return TRUE;

	else
		return FALSE;
}


int main() {
	const char *GoodBoy = "-> Good!\r\n";
	const char *BadBoy = "-> DETECTED!!!!!\r\n";
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);



	// PEB->BeingDebugged
	printf("Checking for debugger using PEB->BeingDebugged\r\n");
	if (Manual_PEB_BeingDebugged()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);


	// PEB->PEB_NtGlobalFlag
	printf("Checking for debugger using PEB->PEB_NtGlobalFlag\r\n");
	if (PEB_NtGlobalFlag()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);



	getchar();
	return 0;
}