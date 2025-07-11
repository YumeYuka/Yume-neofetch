#pragma once
#include <head.hpp>

namespace Utils {
    // 设置控制台为UTF-8编码
    inline void SetConsoleUTF8() {
        SetConsoleOutputCP(CP_UTF8);
    }
    // 启用虚拟终端处理，支持彩色输出
    inline void EnableVirtualTerminalProcessing() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    // 字符串转换函数
    std::wstring StringToWString(std::string const& str);
    std::string  WStringToString(std::wstring const& wstr);

    // 格式化函数
    std::wstring FormatFileSize(ULONGLONG bytes);
    std::wstring FormatPercentage(double percentage);
    std::wstring FormatTime(DWORD milliseconds);

    // 系统工具函数
    bool         IsWindows11();
    bool         IsWindows10();
    std::wstring GetWindowsVersion();

    // 控制台工具函数
    void  SetConsoleUTF8();
    void  EnableVirtualTerminalProcessing();
    COORD GetConsoleSize();

    // 错误处理函数
    std::wstring GetLastErrorString();
    void         LogError(std::wstring const& message);
    void         LogInfo(std::wstring const& message);
}
