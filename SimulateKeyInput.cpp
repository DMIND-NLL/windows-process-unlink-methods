#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <thread>

void SimulateKey(WORD vk) {
    INPUT inp[2] = {};
    inp[0].type = INPUT_KEYBOARD;
    inp[0].ki.wVk = vk;
    inp[1].type = INPUT_KEYBOARD;
    inp[1].ki.wVk = vk;
    inp[1].ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(2, inp, sizeof(INPUT));
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
}

int main() {
    ShellExecuteA(NULL, "open", "C:\\Users\\Public\\Documents", NULL, NULL, SW_SHOWNORMAL);
    Sleep(1200);

    HWND hWnd = FindWindowA("CabinetWClass", NULL);
    SetForegroundWindow(hWnd);
    Sleep(300);

    SimulateKey(VK_TAB);
    Sleep(200);

    // 输入文件名示例
    const char* filename = "TranscodeBridge.exe";
    for (const char* p = filename; *p; ++p) {
        WORD vk = VkKeyScanA(*p) & 0xFF;
        INPUT ip = { INPUT_KEYBOARD };
        ip.ki.wVk = vk;
        SendInput(1, &ip, sizeof(INPUT));
        ip.ki.dwFlags = KEYEVENTF_KEYUP;
        SendInput(1, &ip, sizeof(INPUT));
        Sleep(50);
    }

    SimulateKey(VK_RETURN);
    return 0;
}