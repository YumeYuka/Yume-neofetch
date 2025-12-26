#ifndef ASCII_ART_HPP
#define ASCII_ART_HPP

#include <string>
#include <vector>
#include <windows.h>
#include "ColorTheme.hpp"
#include <algorithm>
#include <ranges>

class AsciiArt {
public:
    AsciiArt() = default;
    ~AsciiArt() = default;

    static std::vector<std::wstring> GetArtByName([[maybe_unused]] const std::wstring& logoName) {
        return GetWindowsArt();
    }

    static std::vector<std::wstring> GetColoredArtByName(const std::wstring& logoName) {
        const auto art = GetArtByName(logoName);
        std::vector<std::wstring> coloredArt;
        coloredArt.reserve(art.size());
        std::ranges::transform(art, std::back_inserter(coloredArt),
            [](const std::wstring& line) { return ColorizeAsciiLine(line); });
        return coloredArt;
    }

    static std::vector<std::wstring> GetOSArt() {
        return GetWindowsArt();
    }

    static std::vector<std::wstring> GetColoredOSArt() {
        const auto art = GetWindowsArt();
        std::vector<std::wstring> coloredArt;
        coloredArt.reserve(art.size());
        std::ranges::transform(art, std::back_inserter(coloredArt),
            [](const std::wstring& line) { return ColorizeAsciiLine(line); });
        return coloredArt;
    }

    static std::vector<std::wstring> GetAvailableLogos() {
        return {L"windows"};
    }

    static std::vector<std::wstring> GetWindowsArt() {
        return {
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"                                   ",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll"
        };
    }

    static std::vector<std::wstring> GetColoredArtByOS(const std::wstring& osName) {
        return GetColoredArtByName(osName);
    }

    static std::vector<std::wstring> GetArtByOS(const std::wstring& osName) {
        return GetArtByName(osName);
    }

private:
    static auto ConvertToWide(const std::string& str) -> std::wstring {
        const auto len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        if (len <= 0) return L"";
        std::wstring result(static_cast<size_t>(len - 1), L'\0');
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, result.data(), len);
        return result;
    }

    static std::wstring ColorizeAsciiLine(const std::wstring& line) {
        const auto colorPrefix = ConvertToWide(ColorTheme::GetSessionColor());
        const auto colorSuffix = ConvertToWide(ColorTheme::MonetPalette::RESET);
        return colorPrefix + line + colorSuffix;
    }
};

#endif // ASCII_ART_HPP
