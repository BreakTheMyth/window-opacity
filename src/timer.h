#pragma once

#include <windows.h>

void CALLBACK closeProgram(HWND hwnd,    UINT  uMsg,
                       UINT_PTR idEvent, DWORD dwTime);

void CALLBACK reloadConfig(HWND hwnd,    UINT  uMsg,
                       UINT_PTR idEvent, DWORD dwTime);