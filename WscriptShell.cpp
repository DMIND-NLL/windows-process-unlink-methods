#include <iostream>
#include <Windows.h>
#include <comdef.h>

bool WscriptShell(const wchar_t* process) {
    CLSID clsidshell;
    if (FAILED(CLSIDFromString(L"{72C24DD5-D70A-438B-8A42-98424B88AFB8}", &clsidshell))) {
        return false;
    }

    LPDISPATCH lpDisp = nullptr;
    if (FAILED(CoCreateInstance(clsidshell, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (LPVOID*)&lpDisp))) {
        return false;
    }

    LPOLESTR pFuncName = (LPOLESTR)L"Run";
    DISPID dispid;
    if (FAILED(lpDisp->GetIDsOfNames(IID_NULL, &pFuncName, 1, LOCALE_SYSTEM_DEFAULT, &dispid))) {
        lpDisp->Release();
        return false;
    }

    VARIANTARG V = {};
    V.vt = VT_BSTR;
    V.bstrVal = _bstr_t(process);

    DISPPARAMS disParams = { &V, NULL, 1, 0 };
    VARIANT pVarResult = {};

    HRESULT hr = lpDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &disParams, &pVarResult, NULL, NULL);

    lpDisp->Release();
    return SUCCEEDED(hr);
}

int main() {
    CoInitializeEx(0, COINIT_MULTITHREADED);
    WscriptShell(L"C:\\Users\\Public\\Documents\\target.exe");
    CoUninitialize();
    return 0;
}