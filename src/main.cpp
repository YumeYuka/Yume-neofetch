#include "head.hpp"
#include "Utils.hpp"
#include "ascii_art.hpp"
#include "system_info.hpp"
#include "color_theme.hpp"
#include <regex>


// 工具函数：移除ANSI转义序列
std::string StripAnsiCodes(const std::string& str) {
    // 正则表达式匹配ANSI转义序列
    const std::regex ansi_re(R"(\x1B\[[0-?]*[ -/]*[@-~])");
    return std::regex_replace(str, ansi_re, "");
}

// 工具函数：将 std::wstring 转为 std::string（UTF-8编码）
std::string ws2s(const std::wstring& wstr) {
    if (wstr.empty()) return "";
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), NULL, 0, NULL, NULL);
    std::string strTo(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
    return strTo;
}

// 初始化应用环境
void InitApp() {
    // 设置控制台为UTF-8编码，支持彩色输出
    Utils::SetConsoleUTF8();
    Utils::EnableVirtualTerminalProcessing();
}

// 创建内存/磁盘使用率进度条
std::string CreateProgressBar(double percentage, int width = 20) {
    std::string bar = "-=[ ";
    int filled = static_cast<int>(percentage * width / 100.0);
    for (int i = 0; i < width; i++) {
        if (i < filled) {
            bar += "/";
        } else {
            bar += " ";
        }
    }
    bar += " ]=-";
    return bar;
}

// 获取系统信息行
std::vector<std::string> GetSystemInfoLines() {
    std::vector<std::string> lines;

    try {
        SystemInfoCollector info;

        // 获取用户名和计算机名
        std::string userName = ws2s(info.GetUserName());
        std::string computerName = ws2s(info.GetComputerName());

        // 用户信息行
        lines.push_back(ColorTheme::ColorizeText(userName + "@" + computerName));
        lines.push_back(std::string(userName.length() + computerName.length() + 1, '-'));

        // 系统信息 - 关键字着色
        lines.push_back(ColorTheme::ColorizeKeyValue("OS:", ws2s(info.GetOSName())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Build:", ws2s(info.GetOSVersion())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Uptime:", ws2s(info.GetUptime())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Resolution:", ws2s(info.GetResolution())));
        lines.push_back(ColorTheme::ColorizeKeyValue("Terminal:", "PowerShell"));
        lines.push_back(ColorTheme::ColorizeKeyValue("CPU:", ws2s(info.GetCPUModel())));
        lines.push_back(ColorTheme::ColorizeKeyValue("GPU:", ws2s(info.GetGPUModel())));

        // 内存信息 - 关键字着色
        double memUsage = info.GetMemoryUsagePercent();
        lines.push_back(ColorTheme::ColorizeKeyValue("Memory:", ws2s(info.GetMemoryInfo()) + " (" + std::to_string(static_cast<int>(memUsage)) + "% in use)"));

        // 磁盘信息 - 关键字着色
        lines.push_back(ColorTheme::ColorizeKeyValue("Disk:", ws2s(info.GetDiskInfo())));
        lines.push_back(""); // 空行

        // 内存进度条 - 关键字着色
        lines.push_back(ColorTheme::ColorizeKeyValue("Mem%: ", CreateProgressBar(memUsage)));
        lines.push_back(""); // 空行

        // 磁盘进度条 - 关键字着色
        double diskUsage = info.GetDiskUsagePercent();
        lines.push_back(ColorTheme::ColorizeKeyValue("Disk%:", CreateProgressBar(diskUsage)));

        // 在第17、18行添加颜色块（空行用于布局）
        lines.push_back(""); // 第16行空行
        lines.push_back(""); // 第17行 - 将在这里显示第一排颜色块
        lines.push_back(""); // 第18行 - 将在这里显示第二排颜色块
        lines.push_back(""); // 第19行空行

    } catch (const std::exception& e) {
        lines.push_back("Error: " + std::string(e.what()));
    }

    return lines;
}

/**
 * 程序入口 - 实现neofetch风格的左右分栏布局
 */
int main() {
    try {
        // 初始化控制������境
        InitApp();

        // 获取ASCII艺术 - 使用带颜色的版本
        AsciiArt art;
        auto artLines = art.GetColoredOSArt();

        // 获取系统信息
        auto infoLines = GetSystemInfoLines();

        // 计算最大行数
        size_t maxLines = std::max(artLines.size(), infoLines.size());

        // 设置左侧ASCII艺术的宽度（用于对齐）
        const int leftWidth = 35;

        // 左右分栏输出
        for (size_t i = 0; i < maxLines; i++) {
            // 左侧：ASCII艺术
            std::string leftPart;
            if (i < artLines.size()) {
                leftPart = ws2s(artLines[i]);
            }

            // 移除ANSI代码以计算真实长度
            std::string strippedLeftPart = StripAnsiCodes(leftPart);

            // 确保左侧宽度一致
            if (strippedLeftPart.length() < leftWidth) {
                leftPart += std::string(leftWidth - strippedLeftPart.length(), ' ');
            } else if (strippedLeftPart.length() > leftWidth) {
                // 注意：这里的裁剪可能不完美，因为它没有考虑多字节字符
                // 但对于当前的ASCII艺术来说是足够的
                leftPart = leftPart.substr(0, leftWidth);
            }

            // 右侧：系统信息
            std::string rightPart;
            if (i < infoLines.size()) {
                rightPart = infoLines[i];
            }

            // 在第17、18行（索引16、17）添加颜色块
            if (i == 17) {
                // 第17行：第一排颜色块 - 莫奈风格（睡莲蓝紫 + 日出光影）
                rightPart = ColorTheme::GetMonetFirstRow();
            } else if (i == 18) {
                // 第18行：第二排颜色块 - 莫奈风格（花园绿植 + 干草堆大地）
                rightPart = ColorTheme::GetMonetSecondRow();
            }

            // 输出完整行
            std::cout << leftPart << "            " << rightPart << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "发生错误: " << e.what() << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}
