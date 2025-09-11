#include <windows.h>
#include <iostream>
#define IS_DOWN 1
#define IS_UP 0

int real_AD[2] = { IS_UP, IS_UP }; 
int virtual_AD[2] = { IS_UP, IS_UP };
int real_WS[2] = { IS_UP, IS_UP }; 
int virtual_WS[2] = { IS_UP, IS_UP };
HHOOK kbhook;
void SendKeyEvent(WORD key, bool down) {
    INPUT input = { 0 };
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    if (!down) input.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &input, sizeof(INPUT));
}
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
    if (nCode >= HC_ACTION && !(kb->flags & LLKHF_INJECTED)) {
        int key = kb->vkCode;
        if (key == 'A' || key == 'D') {
            int idx = (key == 'A') ? 0 : 1;
            int opp_idx = 1 - idx;
            if (wParam == WM_KEYDOWN) {
                real_AD[idx] = IS_DOWN;
                virtual_AD[idx] = IS_DOWN;
                if (real_AD[opp_idx] == IS_DOWN && virtual_AD[opp_idx] == IS_DOWN) {
                    SendKeyEvent((opp_idx == 0) ? 'A' : 'D', false);
                    virtual_AD[opp_idx] = IS_UP;
                }
            }
            else if (wParam == WM_KEYUP) {
                real_AD[idx] = IS_UP;
                virtual_AD[idx] = IS_UP;
                if (real_AD[opp_idx] == IS_DOWN) {
                    SendKeyEvent((opp_idx == 0) ? 'A' : 'D', true);
                    virtual_AD[opp_idx] = IS_DOWN;
                }
            }
        }   
        if (key == 'W' || key == 'S') {
            int idx = (key == 'W') ? 0 : 1;
            int opp_idx = 1 - idx;
            if (wParam == WM_KEYDOWN) {
                real_WS[idx] = IS_DOWN;
                virtual_WS[idx] = IS_DOWN;
                if (real_WS[opp_idx] == IS_DOWN && virtual_WS[opp_idx] == IS_DOWN) {
                    SendKeyEvent((opp_idx == 0) ? 'W' : 'S', false);
                    virtual_WS[opp_idx] = IS_UP;
                }
            }
            else if (wParam == WM_KEYUP) {
                real_WS[idx] = IS_UP;
                virtual_WS[idx] = IS_UP;
                if (real_WS[opp_idx] == IS_DOWN) {
                    SendKeyEvent((opp_idx == 0) ? 'W' : 'S', true);
                    virtual_WS[opp_idx] = IS_DOWN;
                }
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
int main() {
    std::cout << "Success!" << std::endl;
    std::cout << "The program is running in the background." << std::endl;
    kbhook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnhookWindowsHookEx(kbhook);
    return 0;
}