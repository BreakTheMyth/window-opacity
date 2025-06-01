#include <string.h>
#include <stdio.h>
#include "timer.h"
#include "config.h"
#include "main.h"

extern UINT_PTR closeProgramTimerID;
extern UINT_PTR reloadConfigTimerID;

extern ConfigList *list;

HWND        curWin     = NULL;
WindowInfo *curWinInfo = NULL;

HWINEVENTHOOK windowHook;
HHOOK         mouseHook;

int isEnable = 1;

int main() {
    list = initConfigList();

    loadConfig();

    windowHook = SetWinEventHook(EVENT_SYSTEM_FOREGROUND,
        EVENT_SYSTEM_FOREGROUND, NULL, windowMessageHandler, 0, 0,
        WINEVENT_OUTOFCONTEXT);
    
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseMessageHandler, NULL, 0);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void CALLBACK windowMessageHandler(HWINEVENTHOOK hWinEventHook,
                      DWORD event,          HWND hwnd, 
                      LONG  idObject,       LONG idChild,
                      DWORD dwEventThread, DWORD dwmsEventTime) {

    if (event != EVENT_SYSTEM_FOREGROUND || !isEnable)
        return;

    if (curWin) {
        setWindowOpacity(curWin,
            curWinInfo->opacity *
            curWinInfo->originOpacity);

        curWin = NULL;
        curWinInfo = NULL;
    }
    
    char title[256];
    char class[256];

    GetWindowTextA(hwnd, title, 256);
    RealGetWindowClassA(hwnd, class, 256);

    for (int i = 0; i < list->length; i++) {
        WindowInfo *windowInfo = list->data[i];

        if (strcmp(class, windowInfo->class) || 
           !strstr(title, windowInfo->title))
            continue;

        if (!windowInfo->isInitOpacity)
            getWindowOpacity(hwnd, windowInfo);

        setWindowOpacity(hwnd, 
            windowInfo->focusOpacity *
            windowInfo->originOpacity);

        curWin = hwnd;
        curWinInfo = windowInfo;

        break;
    }
    
}

LRESULT mouseMessageHandler(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode < 0) goto next;

    static int isLeftButtonPress  = 0;
    static int isRightButtonPress = 0;
    static int isCounting         = 0;

    switch (wParam) {
        case WM_LBUTTONDOWN:
            isLeftButtonPress = 1;
            break;
        case WM_RBUTTONDOWN:
            isRightButtonPress = 1;
            break;
        case WM_LBUTTONUP:
            isLeftButtonPress = 0;
            break;
        case WM_RBUTTONUP:
            isRightButtonPress = 0;
            break;
    }

    if (isLeftButtonPress && isRightButtonPress) {
        if (isCounting) goto next;
        isCounting = 1;

        if (reloadConfigTimerID == 0) {
            reloadConfigTimerID = SetTimer(NULL, 0, 2000, reloadConfig);
        }
        if (closeProgramTimerID == 0) {
            closeProgramTimerID = SetTimer(NULL, 0, 5000, closeProgram);
        }

        printf("Start counting\n");
    } else {
        if (!isCounting) goto next;
        isCounting = 0;

        if (reloadConfigTimerID && closeProgramTimerID) {
            isEnable = !isEnable;
            if (isEnable) {
                loadConfig();
            } else {
                cleanConfig();
            }
        }

        if (reloadConfigTimerID != 0) {
            KillTimer(NULL, reloadConfigTimerID);

            reloadConfigTimerID = 0;
        }
        if (closeProgramTimerID != 0) {
            KillTimer(NULL, closeProgramTimerID);

            closeProgramTimerID = 0;
        }

        printf("Stop counting\n");
    }

next:
    return CallNextHookEx(mouseHook, nCode, wParam, lParam);
}

void setWindowOpacity(HWND hwnd, BYTE opacity) {
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    SetLayeredWindowAttributes(hwnd, 0, opacity, LWA_ALPHA);
}

void getWindowOpacity(HWND hwnd, WindowInfo *windowInfo) {
    DWORD flags;

    GetLayeredWindowAttributes(hwnd, NULL,
        &windowInfo->originOpacity, &flags);

    windowInfo->isInitOpacity = 1;
}