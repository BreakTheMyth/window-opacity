#include <windows.h>

char *g_title[]  = {WINDOW_TITLE};
char *g_class[]  = {WINDOW_CLASS};
BYTE g_opacity[] = {WINDOW_OPACITY};
BYTE g_focus[]   = {WINDOW_FOCUS_OPACITY};
int  g_winNum    = sizeof(g_class) / sizeof(char*);
HWND g_preWin;
BYTE g_preWinOpacity;

void CALLBACK winHandler(HWINEVENTHOOK hWinEventHook,
    DWORD event,         HWND  hwnd, 
    LONG  idObject,      LONG  idChild,
    DWORD dwEventThread, DWORD dwmsEventTime);

void setWindowOpacity(HWND hwnd, BYTE opacity);