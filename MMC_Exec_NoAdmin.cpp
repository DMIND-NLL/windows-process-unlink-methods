#include <iostream>
#include <Windows.h>
#include <comdef.h>

// 使用 CLSID 方式（非管理员，Win10 可用）
int main() {
    CoInitializeEx(0, COINIT_MULTITHREADED);

    CLSID clsid;
    CLSIDFromString(L"{c08afd90-f2a1-11d1-8455-00a0c91f3880}", &clsid);

    LPDISPATCH lpDisp = nullptr;
    CoCreateInstance(clsid, NULL, CLSCTX_ALL, IID_IDispatch, (void**)&lpDisp);

    if (lpDisp) {
        // 此处可进一步调用 ShellExecute 等，具体实现可根据需求扩展
        std::cout << "MMC Application 创建成功" << std::endl;
        lpDisp->Release();
    }

    CoUninitialize();
    return 0;
}