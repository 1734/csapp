#include "my_win_dbg.hpp"

int main(int argc, TCHAR* argv[])
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (argc != 2)
    {
        printf("Usage: %s [cmdline]\n", argv[0]);
        return -1;
    }
    printf("The cmdline: %s\n", argv[1]);
    // Start the child process. 
    if (!CreateProcess(
        NULL,   // No module name (use command line)
        argv[1],        // Command line
        NULL,           // Process handle not inheritable
        NULL,           // Thread handle not inheritable
        FALSE,          // Set handle inheritance to FALSE
        //DEBUG_PROCESS,  // No creation flags
        DEBUG_PROCESS,
        NULL,           // Use parent's environment block
        NULL,           // Use parent's starting directory 
        &si,            // Pointer to STARTUPINFO structure
        &pi)           // Pointer to PROCESS_INFORMATION structure
        )
    {
        std::cout << "CreateProcess failed (" << GetLastError() << ").\n";
        return -1;
    }

    // Wait until child process exits.
    //WaitForSingleObject(pi.hProcess, INFINITE);

    // Close process and thread handles. 
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    pfnNtQueryInformationProcess gNtQueryInformationProcess;
    if (!sm_LoadNTDLLFunctions(&gNtQueryInformationProcess)) {
        std::cout << "Can not find NtQueryInformationProcess.\n";
        exit(1);
    };
    DEBUG_EVENT debug_event = { 0 };
    bool continue_flag = TRUE;
    int process_level = 0;
    while (continue_flag)
    {
        if (!WaitForDebugEvent(&debug_event, INFINITE))
            return -1;
        switch (debug_event.dwDebugEventCode)
        {
            case CREATE_PROCESS_DEBUG_EVENT:
            {
                ++process_level;
                TCHAR buffer[MAX_PATH];
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, debug_event.dwProcessId);
                if (!hProcess) {
                    _tprintf(_T("Cannot obtain active process name.\n"));
                    break;
                }
                DWORD len = MAX_PATH;
                BOOL ret = QueryFullProcessImageName(hProcess, 0, buffer, &len);
                if (!ret) {
                    _tprintf(_T("Cannot obtain active process name.\n"));
                    break;
                }
                _tprintf(_T("Active process: %s\n"), buffer);

                wstring cmdline;
                get_process_cmdline(hProcess, gNtQueryInformationProcess, cmdline);
                std::wcout << "Active process command line: " << cmdline << std::endl;
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
                wstring cmdline;
                get_process_cmdline(hProcess, gNtQueryInformationProcess, cmdline);
                std::wcout << "Exit process command line: " << cmdline << std::endl;
                CloseHandle(hProcess);
                if (!ret) {
                    _tprintf(_T("Cannot obtain exit process name.\n"));
                    break;
                }
                _tprintf(_T("Exit process: %s\n"), buffer);
                break;
            }
            case CREATE_THREAD_DEBUG_EVENT:
            {
                std::cout << "CREATE_THREAD_DEBUG_EVENT\n";
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, debug_event.dwProcessId);
                if (!hProcess) {
                    _tprintf(_T("Cannot obtain active process name.\n"));
                    break;
                }
                wstring cmdline;
                get_process_cmdline(hProcess, gNtQueryInformationProcess, cmdline);
                std::wcout << "Active thread command line: " << cmdline << std::endl;
                CloseHandle(hProcess);
                break;
            }
            case EXIT_THREAD_DEBUG_EVENT:
            {
                std::cout << "EXIT_THREAD_DEBUG_EVENT\n";
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, debug_event.dwProcessId);
                if (!hProcess) {
                    _tprintf(_T("Cannot obtain active process name.\n"));
                    break;
                }
                wstring cmdline;
                get_process_cmdline(hProcess, gNtQueryInformationProcess, cmdline);
                std::wcout << "Exit thread command line: " << cmdline << std::endl;
                CloseHandle(hProcess);
                break;
            }
            //default:
            //{
            //    _tprintf(_T("Got to default.\n"));
            //    break;
            //}
        }
        ContinueDebugEvent(debug_event.dwProcessId,
            debug_event.dwThreadId,
            DBG_CONTINUE);
    }
    return 0;
}
