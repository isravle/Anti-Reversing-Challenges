#include <stdio.h>
#include <Windows.h>

BOOL WinApi_CloseHandle()
{
	/* APIs making user of the ZwClose syscall (such as CloseHandle, indirectly) 
	can be used to detect a debugger. When a process is debugged, calling ZwClose 
	with an invalid handle will generate a STATUS_INVALID_HANDLE (0xC0000008) exception.
	As with all anti-debugs that rely on information made directly available 
	from the kernel (therefore involving a syscall), the only proper way to bypass 
	the "CloseHandle" anti-debug is to either modify the syscall data from ring3, 
	before it is called, or set up a kernel hook.
	This anti-debug, though extremely powerful, does not seem to be widely used 
	by malicious programs.
	*/

	__try {
		CloseHandle((HANDLE)0x13333337); // Close invalid handle
	}
	__except (STATUS_INVALID_HANDLE) {	// Exception handler
		return TRUE;	// will return TRUE;
	}
}

int main() {
	const char *GoodBoy = "-> Good!\r\n";
	const char *BadBoy = "-> DETECTED!!!!!\r\n";
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);


	// WinApi CloseHandle()
	printf("Checking for debugger using CloseHandle()\r\n");
	if (WinApi_CloseHandle()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);

	getchar();
	return 0;
}