#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>

BOOL OllyDbg_Exploit()
{
	OutputDebugString(TEXT("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s")); // OllyDbg exploit crasher.
	return FALSE;
}

int main() {
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);


	// WinApi CloseHandle()
	printf("Crashing OllyDbg using OutputDebugString()\r\n");
	if (!OllyDbg_Exploit()) {
		printf("if you see this message on console application, so you are not using OllyDbg or you just patched the executable.\r\n");
	}
	printf("%s", Line);

	getchar();
	return 0;
}