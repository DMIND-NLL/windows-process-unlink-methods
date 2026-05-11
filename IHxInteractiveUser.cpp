#include <windows.h>
#include <iostream>
#include <comdef.h>

static const IID IID_IHxInteractiveUser = { 0x8CEC595B, 0x07A1, 0x11D9, {0xB1, 0x5E, 0x00, 0x0D, 0x56, 0xBF, 0xE6, 0xEE} };
static const CLSID CLSID_HxInteractiveUser = { 0x8CEC58E7, 0x07A1, 0x11D9, {0xB1, 0x5E, 0x00, 0x0D, 0x56, 0xBF, 0xE6, 0xEE} };

struct IHxInteractiveUser : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE Execute(LPCWSTR pcUrl) = 0;
};

int main() {
    CoInitialize(NULL);
    IHxInteractiveUser* pHxUser = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_HxInteractiveUser, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,
                                  IID_IHxInteractiveUser, (void**)&pHxUser);

    if (SUCCEEDED(hr) && pHxUser) {
        pHxUser->Execute(L"file:///C:/Windows/System32/notepad.exe");
        pHxUser->Release();
    }

    CoUninitialize();
    return 0;
}