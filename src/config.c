#include <stdio.h>
#include <string.h>
#include "config.h"
#include "main.h"

const char *defaultConfig =
"# Notepad\n\
Notepad 0.8 0.9\n\
    *\n\
\n\
# Explorer\n\
CabinetWClass 0.8 0.9\n\
    *\n\
\n\
# Taskbar\n\
Shell_TrayWnd 0.9 0.9\n\
    *\n\
\n\
# Start Menu\n\
Windows.UI.Core.CoreWindow 0.9 0.9\n\
    *\n\
\n\
# Firefox\n\
MozillaWindowClass 0.8 0.9\n\
    *\n\
\n\
# vscode, QQ, Typora\n\
Chrome_WidgetWin_1 0.8 0.9\n\
    \"Visual Studio Code\"\n\
    QQ 0.9 0.9\n\
    Typora\n";

ConfigList *list;

WindowInfo *initWindowInfo(char *title,   char *class,
                           float opacity, float focusOpacity) {

    WindowInfo *windowInfo = malloc(sizeof(WindowInfo));

    sprintf(windowInfo->title, title);
    sprintf(windowInfo->class, class);

    windowInfo->originOpacity = 255;
    windowInfo->isInitOpacity = 0;
    windowInfo->opacity       = opacity;
    windowInfo->focusOpacity  = focusOpacity;

    return windowInfo;
}

void removeWindowInfo(WindowInfo **windowInfo) {
    free(*windowInfo);

    *windowInfo = NULL;
}

ConfigList *initConfigList() {
    ConfigList *list = malloc(sizeof(ConfigList));

    if (!list) {
        fprintf(stderr, "Failed to init config");
        return NULL;
    }

    list->length = 0;
    list->size   = 4;
    list->data   = malloc(list->size * sizeof(WindowInfo *));

    if (!list->data) {
        fprintf(stderr, "Failed to init data of config");
        free(list);
        return NULL;
    }

    return list;
}

void removeConfigList() {
    for (int i = 0; i < list->length; i++) {
        removeWindowInfo(&list->data[i]);
    }
    
    free(list->data);
    free(list);

    list = NULL;
}

void appendItem(WindowInfo *item) {
    if (list->length == list->size) {
        list->size *= 2;
        WindowInfo **newData = realloc(list->data,
            list->size * sizeof(WindowInfo *));
        if (!newData) {
            fprintf(stderr, "Failed to update config");
            return;
        }
        list->data = newData;
    }
    list->data[list->length++] = item;
}

void loadConfig() {
    FILE *file = fopen("window-opacity.conf", "r");

    if (!file) {
        file = fopen("window-opacity.conf", "w");
        if (!file) {
            fprintf(stderr, "Cannot create \"window-opacity.conf\"");
            exit(1);
        }

        fprintf(file, defaultConfig);
        fclose(file);

        file = fopen("window-opacity.conf", "r");
        if (!file) {
            fprintf(stderr, "Cannot open \"window-opacity.conf\"");
            exit(1);
        }
    }

    char  title[256];
    char  class[256];
    float opacity;
    float focusOpacity;

    char line[1024];
    while (fscanf(file, "%[^\n]", line) != EOF) {
        fgetc(file);

        if (*line == '#') continue;

        if (*line != ' ') {
            *class        = '\0';
             opacity      = 1;
             focusOpacity = 1;

            if (*line == '*') {
                sscanf(line, "* %f %f",
                    &opacity, &focusOpacity);
            } else if (*line == '\"') {
                sscanf(line, "\"%[^\"]\" %f %f",
                    class, &opacity, &focusOpacity);
            } else {
                sscanf(line, "%s %f %f",
                    class, &opacity, &focusOpacity);
            }
        } else {
            sscanf(line, " %[^\n]", line);

            *title = '\0';

            float curOpacity      = 0;
            float curFocusOpacity = 0;

            if (*line == '*') {
                sscanf(line, "* %f %f",
                    &curOpacity, &curFocusOpacity);
            } else if (*line == '\"') {
                sscanf(line, "\"%[^\"]\" %f %f",
                    title, &curOpacity, &curFocusOpacity);
            } else {
                sscanf(line, "%s %f %f",
                    title, &curOpacity, &curFocusOpacity);
            }

            // printf("class=%s\ttitle=%s\topacity=%f\tfocus=%f\n",
            //     *class           ? class           : NULL,
            //     *title           ? title           : NULL,
            //      curOpacity      ? curOpacity      : opacity,
            //      curFocusOpacity ? curFocusOpacity : focusOpacity);

            WindowInfo *windowInfo;
            windowInfo = initWindowInfo(title, class,
                curOpacity      ? curOpacity      : opacity,
                curFocusOpacity ? curFocusOpacity : focusOpacity);

            appendItem(windowInfo);
        }
    }

    fclose(file);

    EnumWindows(loadForEachWindow, 0);
}

void cleanConfig() {
    EnumWindows(unloadForEachWindow, 0);

    removeConfigList();

    list = initConfigList();
}

BOOL loadForEachWindow(HWND hwnd, LPARAM lParam) {
    char title[256];
    char class[256];

    RealGetWindowClassA(hwnd, class, 256);
    GetWindowTextA(hwnd, title, 256);

    for (int i = 0; i < list->length; i++) {
        WindowInfo *windowInfo = list->data[i];

        if (strcmp(class, windowInfo->class) || 
           !strstr(title, windowInfo->title))
            continue;
        
        getWindowOpacity(hwnd, windowInfo);
        
        setWindowOpacity(hwnd, 
            windowInfo->originOpacity * windowInfo->opacity);

        // printf("hwnd=%p\tclass=%s\ttitle=%s\topacity=%dx%f\n", 
        //     hwnd, class, title,
        //     windowInfo->originOpacity,
        //     windowInfo->opacity);

        break;
    }

    return TRUE;
}

BOOL unloadForEachWindow(HWND hwnd, LPARAM lParam) {
    char title[256];
    char class[256];

    RealGetWindowClassA(hwnd, class, 256);
    GetWindowTextA(hwnd, title, 256);

    for (int i = 0; i < list->length; i++) {
        WindowInfo *windowInfo = list->data[i];

        if (strcmp(class, windowInfo->class) || 
           !strstr(title, windowInfo->title))
            continue;
        
        setWindowOpacity(hwnd, windowInfo->originOpacity);

        break;
    }

    return TRUE;
}
