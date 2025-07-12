#ifndef UTILS_HPP
#define UTILS_HPP

#include <head.hpp>

/**
 * 工具函数类 - 提供各种实用功能
 *
 * 这个类包含了项目中需要的各种工具函数，包括：
 * - 控制台UTF-8设置
 * - 字符串处理和转换
 * - ANSI颜色代码处理
 * - 文本对齐和格式化
 */
class Utils {
public:
    /**
     * 设置控制台为UTF-8编码
     * 确保中文和特殊字符能正确显示
     */
    static void SetConsoleUTF8() {
        // 设置控制台输入输出为UTF-8
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        // 设置C++标准库的区域设置
        std::locale::global(std::locale(""));
    }

    /**
     * 启用Windows终端的虚拟终端处理
     * 用于支持ANSI颜色代码
     */
    static void EnableVirtualTerminalProcessing() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    /**
     * 将宽字符串转换为UTF-8字符串
     *
     * @param wstr 输入的宽字符串
     * @return 转换后的UTF-8字符串
     */
    static std::string WStringToString(const std::wstring& wstr) {
        if (wstr.empty()) return "";

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
                                            static_cast<int>(wstr.size()),
                                            nullptr, 0, nullptr, nullptr);
        std::string result(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(),
                           static_cast<int>(wstr.size()),
                           &result[0], size_needed, nullptr, nullptr);
        return result;
    }

    // 为了兼容性，添加简短的别名函数
    static std::string ws2s(const std::wstring& wstr) {
        return WStringToString(wstr);
    }

    /**
     * 将UTF-8字符串转换为宽字符串
     *
     * @param str 输入的UTF-8字符串
     * @return 转换后的宽字符串
     */
    static std::wstring StringToWString(const std::string& str) {
        if (str.empty()) return L"";

        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(),
                                            static_cast<int>(str.size()),
                                            nullptr, 0);
        std::wstring result(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(),
                           static_cast<int>(str.size()),
                           &result[0], size_needed);
        return result;
    }

    // 为了兼容性，添加简短的别名函数
    static std::wstring s2ws(const std::string& str) {
        return StringToWString(str);
    }

    /**
     * 移除字符串中的ANSI转义序列
     * 用于计算字符串的真实显示宽度
     *
     * @param str 包含ANSI代码的字符串
     * @return 移除ANSI代码后的纯文本字符串
     */
    static std::string StripAnsiCodes(const std::string& str) {
        std::string result;
        bool inEscape = false;

        for (char c : str) {
            if (inEscape) {
                if (c == 'm') inEscape = false;
            } else if (c == '\033') {
                inEscape = true;
            } else {
                result += c;
            }
        }
        return result;
    }

    /**
     * 计算字符串的显示宽度（不包含ANSI代码）
     *
     * @param str 输入字符串
     * @return 字符串的真实显示宽度
     */
    static int GetDisplayWidth(const std::string& str) {
        std::string stripped = StripAnsiCodes(str);
        return static_cast<int>(stripped.length());
    }

    /**
     * 智能截断字符串到指定宽度（保留颜色代码）
     *
     * @param str 输入字符串
     * @param maxWidth 最大宽度
     * @return 截断后的字符串
     */
    static std::string TruncateToWidth(const std::string& str, int maxWidth) {
        std::string stripped = StripAnsiCodes(str);
        if (static_cast<int>(stripped.length()) <= maxWidth) {
            return str; // 不需要截断
        }

        // 需要截断：找到截断点并重新构建字符串
        std::string result;
        int visibleCount = 0;
        bool inEscape = false;

        for (size_t i = 0; i < str.length() && visibleCount < maxWidth; i++) {
            char c = str[i];
            result += c;

            if (c == '\033') {
                inEscape = true;
            } else if (inEscape && c == 'm') {
                inEscape = false;
            } else if (!inEscape) {
                visibleCount++;
            }
        }

        return result;
    }

    /**
     * 将字符串填充到指定宽度（右侧补空格）
     *
     * @param str 输入字符串
     * @param width 目标宽度
     * @return 填充后的字符串
     */
    static std::string PadToWidth(const std::string& str, int width) {
        int currentWidth = GetDisplayWidth(str);
        if (currentWidth >= width) {
            return str;
        }
        return str + std::string(width - currentWidth, ' ');
    }

    /**
     * 去除字符串前后的空白字符
     *
     * @param str 输入字符串
     * @return 去除空白后的字符串
     */
    static std::string Trim(const std::string& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";

        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, (last - first + 1));
    }

    /**
     * 检查文件是否存在
     *
     * @param filename 文件路径
     * @return 文件是否存在
     */
    static bool FileExists(const std::string& filename) {
        DWORD attrib = GetFileAttributesA(filename.c_str());
        return (attrib != INVALID_FILE_ATTRIBUTES &&
                !(attrib & FILE_ATTRIBUTE_DIRECTORY));
    }
};

#endif // UTILS_HPP
