#ifndef UTILS_HPP
#define UTILS_HPP

#include <head.hpp>

class Utils {
public:
    void static SetConsoleUTF8() {
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCP(CP_UTF8);

        std::locale::global(std::locale(""));
    }

    void static EnableVirtualTerminalProcessing() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut == INVALID_HANDLE_VALUE) return;

        DWORD dwMode = 0;
        if (!GetConsoleMode(hOut, &dwMode)) return;

        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }

    std::string static WStringToString(std::wstring const& wstr) {
        if (wstr.empty()) return "";

        int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()),
                                              nullptr, 0, nullptr, nullptr);
        std::string result(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), &result[0],
                            size_needed, nullptr, nullptr);
        return result;
    }

    std::string static ws2s(std::wstring const& wstr) { return WStringToString(wstr); }

    std::wstring static StringToWString(std::string const& str) {
        if (str.empty()) return L"";

        int size_needed =
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), nullptr, 0);
        std::wstring result(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), static_cast<int>(str.size()), &result[0],
                            size_needed);
        return result;
    }

    std::wstring static s2ws(std::string const& str) { return StringToWString(str); }

    std::string static StripAnsiCodes(std::string const& str) {
        std::string result;
        bool        inEscape = false;

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

    int static GetDisplayWidth(std::string const& str) {
        std::string stripped = StripAnsiCodes(str);
        return static_cast<int>(stripped.length());
    }


    std::string static TruncateToWidth(std::string const& str, int maxWidth) {
        std::string stripped = StripAnsiCodes(str);
        if (static_cast<int>(stripped.length()) <= maxWidth) return str; // 不需要截断

        std::string result;
        int         visibleCount = 0;
        bool        inEscape     = false;

        for (size_t i = 0; i < str.length() && visibleCount < maxWidth; i++) {
            char c = str[i];
            result += c;

            if (c == '\033') inEscape = true;
            else if (inEscape && c == 'm') inEscape = false;
            else if (!inEscape) visibleCount++;
        }

        return result;
    }
    std::string static PadToWidth(std::string const& str, int width) {
        int currentWidth = GetDisplayWidth(str);
        if (currentWidth >= width) return str;
        return str + std::string(width - currentWidth, ' ');
    }

    std::string static Trim(std::string const& str) {
        size_t first = str.find_first_not_of(" \t\n\r");
        if (first == std::string::npos) return "";

        size_t last = str.find_last_not_of(" \t\n\r");
        return str.substr(first, last - first + 1);
    }

    bool static FileExists(std::string const& filename) {
        DWORD attrib = GetFileAttributesA(filename.c_str());
        return attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY);
    }
};

#endif // UTILS_HPP
