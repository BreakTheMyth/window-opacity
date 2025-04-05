#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "head/config.h"
#include "head/main.h"

char *g_title[]  = {WINDOW_TITLE};
char *g_class[]  = {WINDOW_CLASS};
BYTE g_opacity[] = {WINDOW_OPACITY};
BYTE g_focus[]   = {WINDOW_FOCUS_OPACITY};
int  g_winNum    = sizeof(g_class) / sizeof(char*);
HWND g_preWin;
BYTE g_preWinOpacity;

int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    SetWinEventHook(EVENT_SYSTEM_FOREGROUND,
        EVENT_SYSTEM_FOREGROUND, NULL, winEvent, 0, 0,
        WINEVENT_OUTOFCONTEXT);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }


    return 0;
}

void CALLBACK winEvent(HWINEVENTHOOK hWinEventHook,
    DWORD event,         HWND  hwnd, 
    LONG  idObject,      LONG  idChild,
    DWORD dwEventThread, DWORD dwmsEventTime) {

    if (event != EVENT_SYSTEM_FOREGROUND)
        return;

    if (g_preWin) {
        setWindowOpacity(g_preWin, g_preWinOpacity);

        g_preWin = NULL;
    }
    
    char curTitle[256];
    GetWindowTextA(hwnd, curTitle, sizeof(curTitle));
    char curClass[256];
    RealGetWindowClassA(hwnd, curClass, 256);

    for (int i = 0; i < g_winNum; i++) {
        if (strcmp(curClass, g_class[i])) continue;
        if (g_title[i] && strcmp(curTitle, g_title[i]))
            continue;

        setWindowOpacity(hwnd, g_focus[i]);
        g_preWin = hwnd;
        g_preWinOpacity = g_opacity[i];
    }
    
}

void setWindowOpacity(HWND hwnd, BYTE opacity) {
    SetWindowLong(hwnd, GWL_EXSTYLE,
        GetWindowLong(hwnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    SetLayeredWindowAttributes(hwnd, 0, opacity, LWA_ALPHA);
}