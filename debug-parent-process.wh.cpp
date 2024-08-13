// ==WindhawkMod==
// @id              debug-parent-process
// @name            [DEBUG] Parent Process
// @description     Determine PID and path of parent process for a process
// @version         1.0.0
// @author          aubymori
// @github          https://github.com/aubymori
// @include         explorer.exe
// ==/WindhawkMod==

#include <tlhelp32.h>
#include <psapi.h>

BOOL Wh_ModInit(void)
{
    Wh_Log(L"Init");
    DWORD pid = GetCurrentProcessId();
    Wh_Log(L"PID: %u", pid);

    HANDLE h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32W pe = { 0 };
    pe.dwSize = sizeof(PROCESSENTRY32W);
    DWORD ppid = 0;
    if (Process32FirstW(h, &pe))
    {
        do
        {
            if (pe.th32ProcessID == pid)
            {
                Wh_Log(L"Parent PID: %u", pe.th32ParentProcessID);
                ppid = pe.th32ParentProcessID;
                break;
            }
        }
        while (Process32NextW(h, &pe));
    }
    CloseHandle(h);

    if (ppid)
    {
        HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, ppid);
        if (process)
        {
            WCHAR szProcess[MAX_PATH];
            if (GetModuleFileNameExW(process, NULL, szProcess, MAX_PATH))
            {
                Wh_Log(L"Parent path: %s", szProcess);
            }
            CloseHandle(process);
        }
    }
    return TRUE;
}