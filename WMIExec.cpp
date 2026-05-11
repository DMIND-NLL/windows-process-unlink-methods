#define _WIN32_DCOM
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

bool WMIExec(const wchar_t* process) {
    IWbemLocator* pLoc = NULL;
    IWbemServices* pSvc = NULL;
    IWbemClassObject* pClass = NULL;
    IWbemClassObject* pInParamsDefinition = NULL;
    IWbemClassObject* pClassInstance = NULL;
    IWbemClassObject* pOutParams = NULL;

    if (FAILED(CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc))) {
        return false;
    }

    if (FAILED(pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pSvc))) {
        pLoc->Release();
        return false;
    }

    CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    pSvc->GetObject(_bstr_t(L"Win32_Process"), 0, NULL, &pClass, NULL);
    pClass->GetMethod(_bstr_t(L"Create"), 0, &pInParamsDefinition, NULL);
    pInParamsDefinition->SpawnInstance(0, &pClassInstance);

    VARIANT varCommand;
    varCommand.vt = VT_BSTR;
    varCommand.bstrVal = _bstr_t(process);
    pClassInstance->Put(L"CommandLine", 0, &varCommand, 0);

    pSvc->ExecMethod(_bstr_t(L"Win32_Process"), _bstr_t(L"Create"), 0, NULL, pClassInstance, &pOutParams, NULL);

    pOutParams->Release();
    pClassInstance->Release();
    pInParamsDefinition->Release();
    pClass->Release();
    pSvc->Release();
    pLoc->Release();
    return true;
}

int main() {
    CoInitializeEx(0, COINIT_MULTITHREADED);
    WMIExec(L"C:\\Windows\\System32\\calc.exe");
    CoUninitialize();
    return 0;
}