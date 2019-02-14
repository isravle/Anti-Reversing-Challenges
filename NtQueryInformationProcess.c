#include <stdio.h>
#include <stdbool.h>
#include <Windows.h>


BOOL NtQueryInformationProcess_ProcessDebugPort()
{
	/* Retrieves information about the specified process.
	When you use CheckRemoteDebuggerPresent we accually call NtQueryInformationProcess function and ProcessInformationClass flag set to 7==ProcessDebugPort

	7/ProcessDebugPort = Retrieves a DWORD_PTR value that is the port number of the debugger for the process.
	*/

	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)(IN  HANDLE, IN  UINT, OUT PVOID, IN ULONG, OUT PULONG);

	const int ProcessDebugPort = 7;

	pNtQueryInformationProcess NtQueryInfoProcess = NULL;

	NTSTATUS Status;
	DWORD IsRemotePresent = 0;

	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));

	NtQueryInfoProcess = (pNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");

	Status = NtQueryInfoProcess(GetCurrentProcess(), ProcessDebugPort, &IsRemotePresent, sizeof(unsigned long), NULL);
	if (Status == 0x00000000 && IsRemotePresent != 0)
		return TRUE;
	else
		return FALSE;
}


BOOL NtQueryInformationProcess_ProcessDebugFlags()
{
	/* Retrieves information about the specified process.
	ProcessDebugFlags is an undocumented class that can be passed to the NtQueryProcessInformation function, the function will return the inverse of EPROCESS->NoDebugInherit 
	Using NtQueryInformationProcess is called with the ProcessDebugFlags class, the function will return the inverse of EPROCESS->NoDebugInherit.
	if dubegged, NoDebugInherit==0
	else
	NoDebugInherit=1
	*/

	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)(IN  HANDLE, IN  UINT, OUT PVOID, IN ULONG, OUT PULONG);

	const int ProcessDebugFlags = 0x1f;

	pNtQueryInformationProcess NtQueryInfoProcess = NULL;

	NTSTATUS Status;
	DWORD NoDebugInherit = 0;

	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));

	NtQueryInfoProcess = (pNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");

	Status = NtQueryInfoProcess(GetCurrentProcess(), ProcessDebugFlags, &NoDebugInherit, sizeof(DWORD), NULL);
	if (Status != 0x00000000)
		return FALSE;
	if (NoDebugInherit == FALSE)
		return TRUE;
	else
		return FALSE;
}


BOOL NtQueryInformationProcess_ProcessDebugObject()
{
	/* Retrieves information about the specified process.
	This function uses NtQueryInformationProcess to try to retrieve a handle to the current process's debug object handle.
	if function return True process under debuggerd */

	typedef NTSTATUS(WINAPI *pNtQueryInformationProcess)(IN  HANDLE, IN  UINT, OUT PVOID, IN ULONG, OUT PULONG);

	const int ProcessDebugObjectHandle = 0x1e;
	pNtQueryInformationProcess NtQueryInfoProcess = NULL;

	NTSTATUS Status;
	HANDLE hDebugObject = NULL;

	HMODULE hNtDll = LoadLibrary(TEXT("ntdll.dll"));

	NtQueryInfoProcess = (pNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");

	Status = NtQueryInfoProcess(GetCurrentProcess(), ProcessDebugObjectHandle, &hDebugObject, sizeof(DWORD), NULL);

	if (Status != 0x00000000)
		return false;

	if (hDebugObject)
		return true;
	else
		return false;
}


int main() {
	const char *GoodBoy = "-> Good!\r\n";
	const char *BadBoy = "-> DETECTED!!!!!\r\n";
	const char *Line = "___________________________________________________\r\n";
	printf("isravle-AHL - Anti-Reverse Engineering \r\n");
	printf("%s", Line);



	// NtQueryInformationProcess->ProcessDebugPort
	printf("Checking for debugger using NtQueryInformationProcess->ProcessDebugPort\r\n");
	if (NtQueryInformationProcess_ProcessDebugPort()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);


	// NtQueryInformationProcess->ProcessDebugFlags
	printf("Checking for debugger using NtQueryInformationProcess->ProcessDebugFlags\r\n");
	if (NtQueryInformationProcess_ProcessDebugFlags()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);


	// NtQueryInformationProcess->ProcessDebugObject
	printf("Checking for debugger using NtQueryInformationProcess->ProcessDebugObject\r\n");
	if (NtQueryInformationProcess_ProcessDebugObject()) {
		printf("%s", BadBoy);
	}
	else {
		printf("%s", GoodBoy);
	}
	printf("%s", Line);


	getchar();
	return 0;
}