#include <Windows.h>
#include "ntdll.h"
#pragma comment(lib, "ntdll")

int main() {
    UNICODE_STRING NtImagePath, CurrentDirectory, CommandLine;
    RtlInitUnicodeString(&NtImagePath, (PWSTR)L"\\??\\C:\\Windows\\System32\\notepad.exe");
    RtlInitUnicodeString(&CurrentDirectory, (PWSTR)L"C:\\Windows\\System32");
    RtlInitUnicodeString(&CommandLine, (PWSTR)L"notepad.exe");

    PRTL_USER_PROCESS_PARAMETERS ProcessParameters = NULL;
    RtlCreateProcessParametersEx(&ProcessParameters, &NtImagePath, NULL, &CurrentDirectory,
                                 &CommandLine, NULL, NULL, NULL, NULL, NULL,
                                 RTL_USER_PROCESS_PARAMETERS_NORMALIZED);

    PS_CREATE_INFO CreateInfo = { 0 };
    CreateInfo.Size = sizeof(CreateInfo);
    CreateInfo.State = PsCreateInitialState;

    PPS_ATTRIBUTE_LIST AttributeList = (PPS_ATTRIBUTE_LIST)RtlAllocateHeap(RtlProcessHeap(), HEAP_ZERO_MEMORY, sizeof(PS_ATTRIBUTE) * 3);
    AttributeList->TotalLength = sizeof(PS_ATTRIBUTE_LIST);

    AttributeList->Attributes[0].Attribute = PS_ATTRIBUTE_IMAGE_NAME;
    AttributeList->Attributes[0].Size = NtImagePath.Length;
    AttributeList->Attributes[0].Value = (ULONG_PTR)NtImagePath.Buffer;

    // 可在此处继续添加父进程伪造等属性

    HANDLE hProcess = NULL, hThread = NULL;
    NtCreateUserProcess(&hProcess, &hThread, PROCESS_ALL_ACCESS, THREAD_ALL_ACCESS,
                        NULL, NULL, 0, 0, ProcessParameters, &CreateInfo, AttributeList);

    RtlFreeHeap(RtlProcessHeap(), 0, AttributeList);
    RtlDestroyProcessParameters(ProcessParameters);
    return 0;
}