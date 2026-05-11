#include <windows.h>
#include <iostream>

static const IID IID_IHxHelpPaneServer = { 0x8CEC592C, 0x07A1, 0x11D9, {0xB1, 0x5E, 0x00, 0x0D, 0x56, 0xBF, 0xE6, 0xEE} };
static const CLSID CLSID_HxHelpPaneServer = { 0x8CEC58AE, 0x07A1, 0x11D9, {0xB1, 0x5E, 0x00, 0x0D, 0x56, 0xBF, 0xE6, 0xEE} };

struct IHxHelpPaneServer : public IUnknown {
    virtual HRESULT STDMETHODCALLTYPE DisplayTask(BSTR bstrUrl) = 0;
    virtual HRESULT STDMETHODCALLTYPE DisplayContents(BSTR bstrUrl) = 0;
    virtual HRESULT STDMETHODCALLTYPE DisplaySearchResults(BSTR bstrSearchQuery) = 0;
    virtual HRESULT STDMETHODCALLTYPE Execute(LPCWSTR pcUrl) = 0;
};

int main() {
    CoInitialize(NULL);
    IHxHelpPaneServer* pHelpPane = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_HxHelpPaneServer, NULL, CLSCTX_INPROC_SERVER | CLSCTX_LOCAL_SERVER,
                                  IID_IHxHelpPaneServer, (void**)&pHelpPane);

    if (SUCCEEDED(hr) && pHelpPane) {
        pHelpPane->Execute(L"file:///C:/Windows/System32/notepad.exe");
        pHelpPane->Release();
    }

    CoUninitialize();
    return 0;
}