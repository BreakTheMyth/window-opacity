#include "timer.h"
#include "config.h"

extern HWINEVENTHOOK windowHook;
extern HHOOK         mouseHook;

extern int isEnable;

UINT_PTR closeProgramTimerID = 0;
UINT_PTR reloadConfigTimerID = 0;

void closeProgram(HWND hwnd,    UINT  uMsg,
              UINT_PTR idEvent, DWORD dwTime) {
    
    KillTimer(NULL, closeProgramTimerID);

    closeProgramTimerID = 0;

    UnhookWinEvent(windowHook);
    UnhookWindowsHookEx(mouseHook);
    
    cleanConfig();
    removeConfigList();

    exit(0);
}

void reloadConfig(HWND hwnd,    UINT  uMsg,
              UINT_PTR idEvent, DWORD dwTime) {
            
    KillTimer(NULL, reloadConfigTimerID);
    
    reloadConfigTimerID = 0;

    isEnable = 1;

    cleanConfig();
    loadConfig();
}
