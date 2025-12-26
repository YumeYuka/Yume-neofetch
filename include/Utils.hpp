#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <string_view>
#include <locale>
#include <windows.h>
#include <format>

class Utils {
public:
    static void SetConsoleUTF8() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);
        setlocale(LC_ALL, "");
    }

    static void EnableVirtualTerminalProcessing() {
        const auto hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    static std::string WStringToString(std::wstring_view wstr) {
        if (wstr.empty()) return "";

        const auto size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()),
                                                      nullptr, 0, nullptr, nullptr);
        std::string result(static_cast<size_t>(size_needed), '\0');
        WideCharToMultiByte(CP_UTF8, 0, wstr.data(), static_cast<int>(wstr.size()), result.data(),
                            size_needed, nullptr, nullptr);
        return result;
    }

    static std::string ws2s(std::wstring_view wstr) { return WStringToString(wstr); }

    static std::wstring StringToWString(std::string_view str) {
        if (str.empty()) return L"";

        const auto size_needed =
            MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), nullptr, 0);
        std::wstring result(static_cast<size_t>(size_needed), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(),
                            size_needed);
        return result;
    }

    static std::wstring s2ws(std::string_view str) { return StringToWString(str); }

    static std::string StripAnsiCodes(std::string_view str) {
        std::string result;
        bool inEscape = false;

        for (const char c : str) {
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

    static int GetDisplayWidth(std::string_view str) {
        const auto stripped = StripAnsiCodes(str);
        return static_cast<int>(stripped.length());
    }

    static std::string TruncateToWidth(std::string_view str, int maxWidth) {
        const auto stripped = StripAnsiCodes(str);
        if (static_cast<int>(stripped.length()) <= maxWidth) return std::string(str);

        std::string result;
        int visibleCount = 0;
        bool inEscape = false;

        for (size_t i = 0; i < str.length() && visibleCount < maxWidth; ++i) {
            const char c = str[i];
            result += c;

            if (c == '\033') inEscape = true;
            else if (inEscape && c == 'm') inEscape = false;
            else if (!inEscape) ++visibleCount;
        }

        return result;
    }

    static std::string PadToWidth(std::string_view str, int width) {
        const auto currentWidth = GetDisplayWidth(str);
        if (currentWidth >= width) return std::string(str);
        return std::format("{}{:>{}}", str, "", width - currentWidth);
    }

    static std::string Trim(std::string_view str) {
        const auto first = str.find_first_not_of(" \t\n\r");
        if (first == std::string_view::npos) return "";

        const auto last = str.find_last_not_of(" \t\n\r");
        return std::string(str.substr(first, last - first + 1));
    }

    static bool FileExists(std::string_view filename) {
        const std::string fname(filename);
        const auto attrib = GetFileAttributesA(fname.c_str());
        return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
    }
};

#endif