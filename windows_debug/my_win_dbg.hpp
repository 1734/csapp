#pragma once

#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <winternl.h>
#include <string>
#include <iostream>

using namespace std;

// NtQueryInformationProcess in NTDLL.DLL
typedef NTSTATUS(NTAPI* pfnNtQueryInformationProcess)(
    IN	HANDLE ProcessHandle,
    IN	PROCESSINFOCLASS ProcessInformationClass,
    OUT	PVOID ProcessInformation,
    IN	ULONG ProcessInformationLength,
    OUT	PULONG ReturnLength	OPTIONAL
    );

HMODULE sm_LoadNTDLLFunctions(OUT pfnNtQueryInformationProcess* fun_ptr);
void sm_FreeNTDLLFunctions(IN HMODULE hNtDll, OUT pfnNtQueryInformationProcess* fun_ptr);
BOOL get_process_cmdline(IN HANDLE hProcess, IN pfnNtQueryInformationProcess gNtQueryInformationProcess, OUT wstring& cmdline);
