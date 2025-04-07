# 简介

该项目通过调用 Windows API 单独设置每个窗口的透明度

# 预览

![preview.jpg](preview.jpg)

# 依赖

```powershell
winget install winlibs
winget install ezwinports.make
winget install Microsoft.Git
```

# 安装

```powershell
git clone https://github.com/BreakTheMyth/window-opacity.git
cd window-opacity
make install
```

# 卸载

```powershell
make uninstall
```

# 使用

1. 打开 head/config.h 文件，添加需要的配置，删除不需要的配置 (可使用 spy++ 等工具获取窗口类型和标题，标题可以模糊一点，只要包含特征性字符串，或者使用NULL匹配所有的标题，透明度的设置范围：[0, 255])

```c
// head/config.h                                       记事本      VSCode                QQ                    任务栏           资源管理器        开始菜单                       Firefox             
#define WINDOW_TITLE         /*窗口的标题*/             NULL,      "Visual Studio Code", "QQ",                 NULL,            NULL,            NULL,                         NULL                 
#define WINDOW_CLASS         /*窗口的类名*/             "Notepad", "Chrome_WidgetWin_1", "Chrome_WidgetWin_1", "Shell_TrayWnd", "CabinetWClass", "Windows.UI.Core.CoreWindow", "MozillaWindowClass" 
#define WINDOW_OPACITY       /*窗口失去焦点时的透明度*/  192,       192,                  192,                  224,             192,             224,                          192                  
#define WINDOW_FOCUS_OPACITY /*窗口获得焦点时的透明度*/  224,       224,                  224,                  224,             224,             224,                          224                  
```

2. 修改完后进行调试

```powershell
make run
```

3. 结束调试重新安装

```powershell
make stop
make install
```

4. 程序会在下次开机时启动
