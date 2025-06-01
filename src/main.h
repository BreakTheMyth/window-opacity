#pragma once

#include <windows.h>

void CALLBACK windowMessageHandler(HWINEVENTHOOK hWinEventHook,
                      DWORD event,          HWND hwnd, 
                      LONG  idObject,       LONG idChild,
                      DWORD dwEventThread, DWORD dwmsEventTime);

LRESULT CALLBACK mouseMessageHandler(int    nCode,
                      WPARAM wParam, LPARAM lParam);

void setWindowOpacity(HWND hwnd, BYTE opacity);

void getWindowOpacity(HWND hwnd, WindowInfo *windowInfo);