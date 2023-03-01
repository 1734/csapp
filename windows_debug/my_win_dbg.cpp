// win_dbg.cpp: 定义应用程序的入口点。
//

#include "my_win_dbg.hpp"

// Load NTDLL Library and get entry address for NtQueryInformationProcess
HMODULE sm_LoadNTDLLFunctions(OUT pfnNtQueryInformationProcess* fun_ptr)
{
	HMODULE hNtDll = LoadLibrary(_T("ntdll.dll"));
	if (hNtDll == NULL) return NULL;

	*fun_ptr = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	if (fun_ptr == NULL) {
		FreeLibrary(hNtDll);
		return NULL;
	}
	return hNtDll;
}

// Unloads the NTDLL.DLL and resets the pfnNtQueryInformationProcess variable
void sm_FreeNTDLLFunctions(IN HMODULE hNtDll, OUT pfnNtQueryInformationProcess* fun_ptr)
{
	if (hNtDll)
		FreeLibrary(hNtDll);
	*fun_ptr = NULL;
}

BOOL get_process_cmdline(IN HANDLE hProcess, IN pfnNtQueryInformationProcess gNtQueryInformationProcess, OUT wstring& cmdline)
{
    if (!hProcess) {
        std::cout << "[ERROR]: hProcess is NULL!\n";
        return FALSE;
    }
    BOOL result;
    HANDLE hHeap = GetProcessHeap();
    PROCESS_BASIC_INFORMATION* pbi_ptr = (PROCESS_BASIC_INFORMATION*)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(PROCESS_BASIC_INFORMATION));
    if (!pbi_ptr) {
        std::cout << "[ERROR]: Fail to allocate heap space for pbi_ptr.\n";
        return FALSE;
    }
    LONG status = gNtQueryInformationProcess(hProcess,
        ProcessBasicInformation,
        pbi_ptr,
        sizeof(PROCESS_BASIC_INFORMATION),
        NULL);

    //PPEB ppeb = (PPEB)((PVOID*)pbi_ptr)[1];
    PPEB ppeb = pbi_ptr->PebBaseAddress;
    if (!ppeb) {
        std::cout << "[ERROR]: ppeb is NULL.\n";
        return FALSE;
    }
    //PPEB ppebCopy = (PPEB)malloc(sizeof(PEB));
    PPEB ppebCopy = (PPEB)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(PEB));
    if (!ppebCopy) {
        std::cout << "[ERROR]: Fail to allocate heap space for ppebCopy.\n";
        return FALSE;
    }
    result = ReadProcessMemory(hProcess,
        ppeb,
        ppebCopy,
        sizeof(PEB),
        NULL);

    PRTL_USER_PROCESS_PARAMETERS pRtlProcParam = ppebCopy->ProcessParameters;
    //PRTL_USER_PROCESS_PARAMETERS pRtlProcParamCopy = (PRTL_USER_PROCESS_PARAMETERS)malloc(sizeof(RTL_USER_PROCESS_PARAMETERS));
    PRTL_USER_PROCESS_PARAMETERS pRtlProcParamCopy = (PRTL_USER_PROCESS_PARAMETERS)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, sizeof(RTL_USER_PROCESS_PARAMETERS));
    if (!pRtlProcParamCopy) {
        std::cout << "[ERROR]: Fail to allocate heap space for pRtlProcParamCopy.\n";
        return FALSE;
    }
    result = ReadProcessMemory(hProcess,
        pRtlProcParam,
        pRtlProcParamCopy,
        sizeof(RTL_USER_PROCESS_PARAMETERS),
        NULL);

    PWSTR wBuffer = pRtlProcParamCopy->CommandLine.Buffer;
    USHORT len = pRtlProcParamCopy->CommandLine.Length;
    //PWSTR wBufferCopy = (PWSTR)malloc(len);
    PWSTR wBufferCopy = (PWSTR)HeapAlloc(hHeap, HEAP_ZERO_MEMORY, len);
    if (!wBufferCopy) {
        std::cout << "[ERROR]: Fail to allocate heap space for wBufferCopy.\n";
        return FALSE;
    }
    result = ReadProcessMemory(hProcess,
        wBuffer,
        wBufferCopy, // command line goes here
        len,
        NULL);

    cmdline = wstring(wBufferCopy);
    return result;
}
