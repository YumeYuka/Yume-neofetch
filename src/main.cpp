#include <head.hpp>

#include "Config.hpp"
#include "Utils.hpp"
#include "ascii_art.hpp"
#include "color_theme.hpp"
#include "system_info.hpp"

std::string ws2s(std::wstring const& wstr) { return Utils::WStringToString(wstr); }

std::wstring s2ws(std::string const& str) { return Utils::StringToWString(str); }

std::string StripAnsiCodes(std::string const& str) { return Utils::StripAnsiCodes(str); }

int GetDisplayWidth(std::string const& str) { return Utils::GetDisplayWidth(str); }

std::string TruncateToWidth(std::string const& str, int maxWidth) {
    return Utils::TruncateToWidth(str, maxWidth);
}

std::string PadToWidth(std::string const& str, int width) { return Utils::PadToWidth(str, width); }

void InitApp() {
    Utils::SetConsoleUTF8();
    Utils::EnableVirtualTerminalProcessing();
}

std::string CreateProgressBar(double percentage, int width = 20) {
    std::string bar    = "-=[";
    int         filled = static_cast<int>(percentage * width / 100.0);
    for (int i = 0; i < width; i++)
        if (i < filled) bar += "/";
        else bar += " ";
    bar += " ]=-";
    return bar;
}

std::vector<std::string> GetSystemInfoLines() {
    std::vector<std::string> lines;

    try {
        SystemInfoCollector info;

        // 用户名和计算机名
        std::string userName     = ws2s(info.GetUserName());
        std::string computerName = ws2s(info.GetComputerName());

        // 用户
        lines.push_back(ColorTheme::ColorizeText(userName + "@" + computerName));
        lines.emplace_back(userName.length() + computerName.length() + 1, '-');

        // 系统
        lines.push_back(ColorTheme::ColorizeKeyValue("OS:", ws2s(info.GetOSName())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Build:", ws2s(info.GetOSVersion())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Uptime:", ws2s(info.GetUptime())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Resolution:", ws2s(info.GetResolution())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Terminal:", "PowerShell"));
        lines.push_back(ColorTheme::ColorizeKeyValue("CPU:", ws2s(info.GetCPUModel())));
        lines.push_back(ColorTheme::ColorizeKeyValue("GPU:", ws2s(info.GetGPUModel())));

        // 内存
        double memUsage = info.GetMemoryUsagePercent();
        lines.push_back(ColorTheme::ColorizeKeyValue(
            "Memory:", ws2s(info.GetMemoryInfo()) + " (" + std::to_string(static_cast<int>(memUsage))
                           + "% in use)"));

        // 磁盘
        lines.push_back(ColorTheme::ColorizeKeyValue("Disk:", ws2s(info.GetDiskInfo())));
        lines.emplace_back("");

        // 内存进度条
        lines.push_back(ColorTheme::ColorizeKeyValue("Mem%: ", CreateProgressBar(memUsage)));
        lines.emplace_back("");

        // 磁盘
        double diskUsage = info.GetDiskUsagePercent();
        lines.push_back(ColorTheme::ColorizeKeyValue("Disk%:", CreateProgressBar(diskUsage)));

        lines.emplace_back(""); // 第16行空行
        lines.emplace_back(""); // 第17行 - 将在这里显示第一排颜色块
        lines.emplace_back(""); // 第18行 - 将在这里显示第二排颜色块
        lines.emplace_back(""); // 第19行空行

    } catch (std::exception const& e) { lines.push_back("Error: " + std::string(e.what())); }

    return lines;
}

int main(int argc, char* argv[]) {
    try {
        InitApp();

        if (argc == 3 && std::string(argv[1]) == "--set-logo") {
            std::string newLogoName = argv[2];
            Config::setLogoName(newLogoName);
            return 0;
        } else if (argc == 2 && std::string(argv[1]) == "--list-logos") {
            AsciiArt                  art;
            std::vector<std::wstring> logos = art.GetAvailableLogos();
            std::cout << "Available logos:\n";
            for (auto const& logo : logos) std::cout << "- " << Utils::ws2s(logo) << "\n";
            return 0;
        } else if (argc == 2 && std::string(argv[1]) == "--version") {
            std::cout << "Yume-neofetch Version: 1.0.0\n"
                      << "author: YumeYuka\n"
                      << "Compiler: ";
#if defined(_MSC_VER)
            std::cout << "MSVC";
#elif defined(__MINGW32__)
            std::cout << "MinGW";
#elif defined(__clang__)
            std::cout << "Clang";
#else
            std::cout << "unknown";
#endif
            std::cout << "\nBuild time: " << __DATE__ << " " << __TIME__ << "\n";
            return 0;
        } else if (argc == 2 && (std::string(argv[1]) == "--help" || std::string(argv[1]) == "-h")) {
            std::cout << "Yume-neofetch help:\n";
            std::cout << "  --set-logo <logo>    Set the displayed logo\n";
            std::cout << "  --list-logos         List all available logos\n";
            std::cout << "  --version            Show current version number\n";
            std::cout << "  --help, -h           Show help information\n";
            return 0;
        }
        std::string logoName = Config::getLogoName();

        AsciiArt art;
        auto     artLines = art.GetColoredArtByName(s2ws(logoName));

        auto infoLines = GetSystemInfoLines();

        size_t maxLines = std::max(artLines.size(), infoLines.size());

        int const LEFT_WIDTH = 40; // 固定宽度，防止logo影响右侧
        int const SPACING    = 10; // 左右间距

        for (size_t i = 0; i < maxLines; i++) {
            std::string leftPart;
            if (i < artLines.size()) leftPart = ws2s(artLines[i]);
            leftPart = TruncateToWidth(leftPart, LEFT_WIDTH);
            leftPart = PadToWidth(leftPart, LEFT_WIDTH);

            std::string rightPart;
            if (i < infoLines.size()) rightPart = infoLines[i];

            if (i == 16) rightPart = ColorTheme::GetMonetFirstRow();
            else if (i == 17) rightPart = ColorTheme::GetMonetSecondRow();

            std::cout << leftPart << std::string(SPACING, ' ') << rightPart << std::endl;
        }

    } catch (std::exception const& e) {
        std::cerr << "发生错误: " << e.what() << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}
