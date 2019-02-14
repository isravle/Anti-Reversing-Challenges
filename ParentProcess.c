#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>


// Struct for pbi
typedef struct _PROCESS_BASIC_INFORMATION {
	PVOID Reserved1;
	void* PebBaseAddress;
	PVOID Reserved2[2];
	ULONG_PTR UniqueProcId;
	ULONG_PTR ParentProcId;
} PROCESS_BASIC_INFORMATION;


// This function returns true if the parent process of
// the current running process is Explorer.exe
inline DWORD GetExplorerPIDbyShellWindow()
{
	DWORD PID = 0;
	GetWindowThreadProcessId(GetShellWindow(), &PID);
	return PID;
}

// GetParentProcessId will use the NtQueryInformationProcess function
// exported by NtDll to retrieve the parent process id for the current 
// process and if for some reason it doesn't work, it returns 0
DWORD GetParentProcessId()
{
	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)
		(HANDLE, UINT, PVOID, ULONG, PULONG);

	NTSTATUS Status = 0;
	PROCESS_BASIC_INFORMATION pbi;
	ZeroMemory(&pbi, sizeof(PROCESS_BASIC_INFORMATION));

	// Get NtQueryInformationProcess
	pNtQueryInformationProcess NtQIP = (pNtQueryInformationProcess)
		GetProcAddress(
			GetModuleHandle(TEXT("ntdll.dll")), "NtQueryInformationProcess");

	Status = NtQIP(GetCurrentProcess(), 0, (void*)&pbi,
		sizeof(PROCESS_BASIC_INFORMATION), 0);

	if (Status != 0x00000000)
		return 0;
	else
		return pbi.ParentProcId;
}


bool IsParentExplorerExe()
{
	DWORD PPID = GetParentProcessId();
	DWORD ExplorerPID = GetExplorerPIDbyShellWindow();
	if (PPID == ExplorerPID)
		return FALSE;
	else
		return TRUE;
}

int main() {
	const char *GoodBoy = "-> Good!\r\n";
	const char *BadBoy = "-> DETECTED!!!!!\r\n";
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);


	printf("Checking for debugger using Parent Process\r\n");
	if (IsParentExplorerExe()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);


	getchar();
	return 0;
}