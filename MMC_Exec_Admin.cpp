#include <iostream>
#include <Windows.h>
#include <comdef.h>

bool MMCExec(const wchar_t* process, const wchar_t* args) {
    CLSID CLSID_MMCApplication;
    CLSIDFromString(L"{49b2791a-b1ae-4c90-9b8e-e860ba07f889}", &CLSID_MMCApplication);

    LPDISPATCH lpDisp = nullptr;
    CoCreateInstance(CLSID_MMCApplication, NULL, CLSCTX_ALL, IID_IDispatch, (PVOID*)&lpDisp);

    // Document
    DISPID dispid;
    LPOLESTR name = (LPOLESTR)L"Document";
    lpDisp->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

    VARIANT res;
    DISPPARAMS params = { NULL, NULL, 0, 0 };
    lpDisp->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &params, &res, NULL, NULL);

    LPDISPATCH lpDoc = res.pdispVal;

    // ActiveView
    name = (LPOLESTR)L"ActiveView";
    lpDoc->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid);
    lpDoc->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &params, &res, NULL, NULL);

    LPDISPATCH lpView = res.pdispVal;

    // ExecuteShellCommand
    name = (LPOLESTR)L"ExecuteShellCommand";
    lpView->GetIDsOfNames(IID_NULL, &name, 1, LOCALE_SYSTEM_DEFAULT, &dispid);

    VARIANTARG argsV[4] = {};
    argsV[0].vt = VT_BSTR; argsV[0].bstrVal = _bstr_t(L"Minimized");
    argsV[1].vt = VT_BSTR; argsV[1].bstrVal = _bstr_t(args);
    argsV[2].vt = VT_BSTR; argsV[2].bstrVal = _bstr_t(L"C:\\");
    argsV[3].vt = VT_BSTR; argsV[3].bstrVal = _bstr_t(process);

    DISPPARAMS dp = { argsV, NULL, 4, 0 };
    lpView->Invoke(dispid, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, NULL, NULL, NULL);

    lpView->Release();
    lpDoc->Release();
    lpDisp->Release();
    return true;
}

int main() {
    CoInitializeEx(0, COINIT_MULTITHREADED);
    MMCExec(L"calc.exe", L"");
    CoUninitialize();
    return 0;
}