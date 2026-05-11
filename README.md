# windows-process-unlink-methods
本项目为Windows平台下的进程创建与父进程伪造技术演示项目，专注于通过多种 COM 接口、WMI、MMC、属性伪造等方式创建子进程，从而打破传统的进程父子关系链，实现进程隐藏与安全软件绕过。

## 项目结构
windows-process-unlink-methods/
- [README.md](README.md)
- [WscriptShell.cpp](WscriptShell.cpp)
- [WMIExec.cpp](WMIExec.cpp)
- [IHxInteractiveUser.cpp](IHxInteractiveUser.cpp)
- [IHxHelpPaneServer.cpp](IHxHelpPaneServer.cpp)
- [MMC_Exec_Admin.cpp](MMC_Exec_Admin.cpp)
- [MMC_Exec_NoAdmin.cpp](MMC_Exec_NoAdmin.cpp)
- [ParentSpoof_CreateProcess.cpp](ParentSpoof_CreateProcess.cpp)
- [ParentSpoof_NtCreateUserProcess.cpp](ParentSpoof_NtCreateUserProcess.cpp)
- [SimulateKeyInput.cpp](SimulateKeyInput.cpp)


## 各文件对应方法说明

| 文件                                    | 技术名称                                             | 备注                       |
| --------------------------------------- | ---------------------------------------------------- | -------------------------- |
| **WscriptShell.cpp**                    | WScript.Shell COM Run                                | 稳定，推荐                 |
| **WMIExec.cpp**                         | WMI Win32_Process Create                             | 稳定，推荐                 |
| **IHxInteractiveUser.cpp**              | IHxInteractiveUser                                   | 父进程不可见               |
| **IHxHelpPaneServer.cpp**               | IHxHelpPaneServer                                    | 父进程不可见               |
| **MMC_Exec_Admin.cpp**                  | MMC20.Application (管理员)                           | 需要管理员权限             |
| **MMC_Exec_NoAdmin.cpp**                | MMC CLSID 方式 (非管理员)                            | Win11 可能失效，Win10 可用 |
| **ParentSpoof_CreateProcess.cpp**       | CreateProcess + PROC_THREAD_ATTRIBUTE_PARENT_PROCESS | **强烈推荐**，最干净       |
| **ParentSpoof_NtCreateUserProcess.cpp** | NtCreateUserProcess + 属性伪造                       | 更底层，可控性强           |
| **SimulateKeyInput.cpp**                | 模拟键盘输入启动                                     | 动静大，不推荐             |


### 0x01、通过Wscript.Shell创建进程（WscriptShell.cpp）
`Wscript.Shell` 是 Windows 脚本宿主*提供的一个常用 COM 对象
<img width="714" height="850" alt="image" src="https://github.com/user-attachments/assets/3b7e93ca-9a83-4654-a349-f00b756d64b9" />

### 0x02、通过 WMI 创建进程（WMIExec.cpp）
WMI 是 Windows 提供的 统一系统管理接口，允许脚本和程序查询/管理操作系统资源。

### 0x03、通过IHxInteractiveUser创建进程（IHxInteractiveUser.cpp）
接口是`IHxInteractiveUser`，`CLSID`为`8CEC58E7-07A1-11D9-B15E-000D56BFE6EE`，`IID`为`8CEC595B-07A1-11D9-B15E-000D56BFE6EE`
可见打开的Notepad.exe，显示找不到其父进程：
<img width="798" height="960" alt="image" src="https://github.com/user-attachments/assets/75938c30-987e-4a9d-9c03-dc03d40041e0" />
<img width="1059" height="118" alt="image" src="https://github.com/user-attachments/assets/6cd53415-0310-480e-af1d-788f40cd5a8c" />

### 0x04、通过IHxHelpPaneServer创建进程（IHxHelpPaneServer.cpp）
接口是`IHxHelpPaneServer`，`CLSID`为`8CEC58AE-07A1-11D9-B15E-000D56BFE6EE`，`IID`为`8CEC592C-07A1-11D9-B15E-000D56BFE6EE

### 0x05、通过 mmc.exe 创建进程（MMC_Exec_Admin.cpp）
powershell代码，需要*管理员权限*执行：
```
$com = [activator]::CreateInstance([type]::GetTypeFromProgID("MMC20.Application"))
$com.Document.ActiveView.ExecuteShellCommand('cmd.exe',"C:\\","/c calc.exe","Minimized")
```

### 0x06、通过 mmc.exe 创建进程（MMC_Exec_NoAdmin.cpp）
无需管理员权限，**在win11无法运行**，在win10正常运行

### 0x07、通过CreateProcess伪造父进程（ParentSpoof_CreateProcess.cpp）
**创建新进程时指定“父进程句柄”**
 Windows 8 及以上的 `CreateProcess` 支持通过 `PROC_THREAD_ATTRIBUTE_PARENT_PROCESS` 设置**任意父进程**，让新进程的“父进程”字段为指定进程（比如 `explorer.exe`），而不是你自己的进程。
 <img width="766" height="949" alt="image" src="https://github.com/user-attachments/assets/ce3203c4-a7fd-41ba-b8f4-26d0c4b36628" />

### 0x08、通过NtCreateUserProcess伪造父进程（ParentSpoof_NtCreateUserProcess.cpp）
我们在使用`CreateProcess`创建进程的时候能通过设置特定的参数来达到欺骗的效果，在`NtCreateUserProcess`里面也同样能够做到

### 0x09、模拟按键（SimulateKeyInput.cpp）
进程断链相比于父进程欺骗更加安全，但是在核晶环境下会被禁止模拟键盘的行为，并且动静比较大，用户能感知到



## 使用建议
1. 编译环境：
   - Visual Studio 2022
   - Windows SDK（含 wbemuuid.lib）
   - 部分文件需要以管理员权限运行
2. 注意事项：
   - 部分技术在 Windows 11 上行为有变化
   - 实际使用时建议配合**进程注入**、**DLL 加载**等进一步隐藏
  
## 项目声明
本项目的作者及单位
```
项目名称：windows-process-unlink-methods
项目作者：Hong Deng, Zhiquan Liu
作者单位：暨南大学网络空间安全学院
```
