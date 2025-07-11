#ifndef ASCII_ART_MERGED_HPP
#define ASCII_ART_MERGED_HPP

#include <head.hpp>
#include "color_theme.hpp"

// ASCII艺术生成器类 - 头文件声明和实现合并
class AsciiArt {
public:
    AsciiArt() {
        // 构造函数
    }

    ~AsciiArt() {
        // 析构函数
    }

    // 获取操作系统对应的ASCII艺术
    std::vector<std::wstring> GetOSArt() {
        // 根据操作系统版本返回对应的ASCII艺术
        return GetWindowsArt();
    }

    // 获取带颜色的操作系统ASCII艺术
    std::vector<std::wstring> GetColoredOSArt() {
        // 根据操作系统版本返回带颜色的ASCII艺术
        return GetColoredWindowsArt();
    }

    // 获取Windows 10/11的ASCII艺术
    std::vector<std::wstring> GetWindowsArt() {
        // Windows标志性的四个方块ASCII艺术
        std::vector<std::wstring> art = {
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"                                  ",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll"};

        return art;
    }

    // 获取带颜色的Windows ASCII艺术
    std::vector<std::wstring> GetColoredWindowsArt() {
        // 获取原始ASCII艺术
        std::vector<std::wstring> art = GetWindowsArt();

        // 为每一行添加颜色
        std::vector<std::wstring> coloredArt;
        for (auto const& line : art) coloredArt.push_back(ColorizeAsciiLine(line));

        return coloredArt;
    }

private:
    // 检测Windows版本
    bool IsWindows11() {
        // 检测是否为Windows 11
        HKEY hKey;
        if (RegOpenKeyExW(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0,
                          KEY_READ, &hKey)
            == ERROR_SUCCESS) {
            wchar_t build[256];
            DWORD   dataSize = sizeof(build);
            DWORD   dataType;

            if (RegQueryValueExW(hKey, L"CurrentBuild", NULL, &dataType, (LPBYTE)build, &dataSize)
                == ERROR_SUCCESS) {
                int buildNumber = _wtoi(build);
                RegCloseKey(hKey);
                return buildNumber >= 22000; // Windows 11的构建号从22000开始
            }

            RegCloseKey(hKey);
        }

        return false;
    }

    std::wstring DetectOSVersion() {
        if (IsWindows11()) return L"Windows 11";
        else return L"Windows 10";
    }

    // 将ASCII艺术行添加颜色
    std::wstring ColorizeAsciiLine(std::wstring const& line) {
        // 获取当前会话的颜色（与系统信息保持一致）
        std::string sessionColor = ColorTheme::GetSessionColor();
        std::string resetColor   = ColorTheme::MonetPalette::RESET;

        // 将颜色编码从string转换为wstring
        std::wstring colorPrefix;
        std::wstring colorSuffix;

        // 转换颜色编码为宽字符
        int colorLen = MultiByteToWideChar(CP_UTF8, 0, sessionColor.c_str(), -1, nullptr, 0);
        if (colorLen > 0) {
            colorPrefix.resize(colorLen - 1);
            MultiByteToWideChar(CP_UTF8, 0, sessionColor.c_str(), -1, &colorPrefix[0], colorLen);
        }

        int resetLen = MultiByteToWideChar(CP_UTF8, 0, resetColor.c_str(), -1, nullptr, 0);
        if (resetLen > 0) {
            colorSuffix.resize(resetLen - 1);
            MultiByteToWideChar(CP_UTF8, 0, resetColor.c_str(), -1, &colorSuffix[0], resetLen);
        }

        // 只为非空行添加颜色（保持空行不变）
        if (line.find_first_not_of(L' ') != std::wstring::npos) return colorPrefix + line + colorSuffix;
        else return line; // 空行保持原样
    }
};

#endif // ASCII_ART_MERGED_HPP
