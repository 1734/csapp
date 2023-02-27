#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <winternl.h>

#include "NTProcessInfo.h"

extern pfnNtQueryInformationProcess gNtQueryInformationProcess;

void _tmain(int argc, TCHAR* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (argc != 2)
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return;
    }

    // Start the child process. 
    if (!CreateProcess(NULL,   // No module name (use command line)
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        TRUE,          // Set handle inheritance to FALSE
        DEBUG_PROCESS,              // No creation flags
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return;
    }

    //// Wait until child process exits.
    //WaitForSingleObject(pi.hProcess, INFINITE);

    //// Close process and thread handles. 
    //CloseHandle(pi.hProcess);
    //CloseHandle(pi.hThread);
    if (!sm_LoadNTDLLFunctions()) {
        printf("Can not find NtQueryInformationProcess.\n");
        exit(1);
    };
    DEBUG_EVENT debug_event = { 0 };
    bool continue_flag = TRUE;
    int process_level = 0;
    while(continue_flag)
    {
        if (!WaitForDebugEvent(&debug_event, INFINITE))
            return;
        switch (debug_event.dwDebugEventCode)
        {
            case CREATE_PROCESS_DEBUG_EVENT:
            {
                _tprintf(_T("Active process0: %ws\n"), GetCommandLineW());
                ++process_level;
                TCHAR buffer[MAX_PATH];
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, TRUE, debug_event.dwProcessId);
                if (!hProcess) {
                    _tprintf(_T("Cannot obtain active process name.\n"));
                    break;
                }
                DWORD len = MAX_PATH;
                BOOL ret = QueryFullProcessImageName(hProcess, 0, buffer, &len);
                //DWORD len = MAX_PATH;
                //BOOL ret = QueryFullProcessImageName(pi.hProcess, 0, buffer, &len);
                if (!ret) {
                    _tprintf(_T("Cannot obtain active process name.\n"));
                    break;
                }
                _tprintf(_T("Active process1: %s\n"), buffer);

                PROCESS_BASIC_INFORMATION pbi;
                gNtQueryInformationProcess(hProcess, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);
                _tprintf(_T("Active process1.5: %s\n"), "aaa");
                smPROCESSINFO* ppi = new smPROCESSINFO;
                sm_GetNtProcessInfo(debug_event.dwProcessId, ppi);
                CloseHandle(hProcess);
                break;
            }
            case EXIT_PROCESS_DEBUG_EVENT:
            {
                --process_level;
                if (process_level == 0) {
                    continue_flag = FALSE;
                }
                TCHAR buffer[MAX_PATH];
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, debug_event.dwProcessId);
                if (!hProcess) {
                    _tprintf(_T("Cannot obtain exit process name.\n"));
                    break;
                }
                DWORD len = MAX_PATH;
                BOOL ret = QueryFullProcessImageName(hProcess, 0, buffer, &len);
                CloseHandle(hProcess);
                if (!ret) {
                    _tprintf(_T("Cannot obtain exit process name.\n"));
                    break;
                }
                _tprintf(_T("Exit process: %s\n"), buffer);
                break;
            }
            default:
            {
                //_tprintf(_T("Got to default.\n"));
                break;
            }
        }
        ContinueDebugEvent(debug_event.dwProcessId,
            debug_event.dwThreadId,
            DBG_CONTINUE);
    }
}

