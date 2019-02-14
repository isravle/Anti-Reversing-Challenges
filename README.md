Hello ALL,

I want to share with you some techniques for anti debugging.

Please feel free to contact me if somthing is not clear or working =]

I attached 7 difference executables:
1.  DebuggerPresent_WinAPI = IsDebuggerPresent, CheckRemoteDebuggerPresent
2.	ManualPEB = PEB BeingDebugged BYTE, PEB NtGlobalFlag Flag
3.	NtQueryInformationProcess = ProcessInformationClass->(ProcessDebugPort==7), ProcessInformationClass->(ProcessDebugFlags == 0x1f)
4.	NtSetInformationThread = ThreadInformationClass->(HideThreadFromDebugger==0x11)
5.	Crash OllyDbg = OutputDebugString
6.	Parent Process = Check (PPID == 'Explorer.exe')
7.	CloseHandle = Invalid CloseHandle + exception handler



Enjoy =]
