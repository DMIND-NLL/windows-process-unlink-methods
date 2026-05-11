#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

DWORD FindProcessId(const wchar_t* processName) {
    PROCESSENTRY32W entry = { sizeof(entry) };
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (_wcsicmp(entry.szExeFile, processName) == 0) {
                CloseHandle(snapshot);
                return entry.th32ProcessID;
            }
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    return 0;
}

int main() {
    DWORD explorerPid = FindProcessId(L"explorer.exe");
    HANDLE hExplorer = OpenProcess(PROCESS_ALL_ACCESS, FALSE, explorerPid);

    STARTUPINFOEXW si = { 0 };
    si.StartupInfo.cb = sizeof(si);

    SIZE_T attrListSize = 0;
    InitializeProcThreadAttributeList(NULL, 1, 0, &attrListSize);
    si.lpAttributeList = (LPPROC_THREAD_ATTRIBUTE_LIST)HeapAlloc(GetProcessHeap(), 0, attrListSize);
    InitializeProcThreadAttributeList(si.lpAttributeList, 1, 0, &attrListSize);

    UpdateProcThreadAttribute(si.lpAttributeList, 0, PROC_THREAD_ATTRIBUTE_PARENT_PROCESS,
                              &hExplorer, sizeof(HANDLE), NULL, NULL);

    PROCESS_INFORMATION pi = { 0 };
    wchar_t exePath[] = L"C:\\Users\\Public\\Documents\\TranscodeBridge.exe";

    BOOL res = CreateProcessW(exePath, NULL, NULL, NULL, FALSE,
                              EXTENDED_STARTUPINFO_PRESENT, NULL, NULL,
                              &si.StartupInfo, &pi);

    if (res) {
        printf("进程已启动，父进程为 explorer.exe\n");
    } else {
        printf("CreateProcess 失败: %d\n", GetLastError());
    }

    DeleteProcThreadAttributeList(si.lpAttributeList);
    HeapFree(GetProcessHeap(), 0, si.lpAttributeList);
    CloseHandle(hExplorer);
    if (pi.hProcess) CloseHandle(pi.hProcess);
    if (pi.hThread) CloseHandle(pi.hThread);

    return 0;
}