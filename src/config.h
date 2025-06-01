#pragma once

#include <windows.h>

typedef struct WindowInfo {
    char  title[256];
    char  class[256];
    BYTE  originOpacity;
    int   isInitOpacity;
    float opacity;
    float focusOpacity;
} WindowInfo;

WindowInfo *initWindowInfo(char *title,   char *class,
                           float opacity, float focusOpacity);

void removeWindowInfo(WindowInfo **windowInfo);

typedef struct ConfigList {
    WindowInfo **data;
    int          size;
    int          length;
} ConfigList;


ConfigList *initConfigList();

void removeConfigList();

void appendItem(WindowInfo *item);

void loadConfig();

void cleanConfig();

BOOL CALLBACK loadForEachWindow(HWND hwnd, LPARAM lParam);

BOOL CALLBACK unloadForEachWindow(HWND hwnd, LPARAM lParam);