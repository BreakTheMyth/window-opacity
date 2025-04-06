#include <windows.h>

void CALLBACK winHandler(HWINEVENTHOOK hWinEventHook,
    DWORD event,         HWND  hwnd, 
    LONG  idObject,      LONG  idChild,
    DWORD dwEventThread, DWORD dwmsEventTime);

void setWindowOpacity(HWND hwnd, BYTE opacity);