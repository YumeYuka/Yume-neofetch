#ifndef HERE_HPP
#define HERE_HPP

#include <format>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "sixel.h"
#include "system_info.hpp"
#include "ColorTheme.hpp"
#include "Utils.hpp"
#include "AsciiArt.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace ANSI {
    constexpr const char* Reset = "\033[0m";
    constexpr const char* Bold = "\033[1m";
    constexpr const char* Dim = "\033[2m";
    constexpr const char* Italic = "\033[3m";
    constexpr const char* Underline = "\033[4m";

    constexpr const char* FgBlack = "\033[30m";
    constexpr const char* FgRed = "\033[31m";
    constexpr const char* FgGreen = "\033[32m";
    constexpr const char* FgYellow = "\033[33m";
    constexpr const char* FgBlue = "\033[34m";
    constexpr const char* FgMagenta = "\033[35m";
    constexpr const char* FgCyan = "\033[36m";
    constexpr const char* FgWhite = "\033[37m";

    constexpr const char* FgBrightRed = "\033[91m";
    constexpr const char* FgBrightGreen = "\033[92m";
    constexpr const char* FgBrightYellow = "\033[93m";
    constexpr const char* FgBrightBlue = "\033[94m";
    constexpr const char* FgBrightMagenta = "\033[95m";
    constexpr const char* FgBrightCyan = "\033[96m";
    constexpr const char* FgBrightWhite = "\033[97m";

    inline std::string CursorUp(int n = 1) { return std::format("\033[{}A", n); }
    inline std::string CursorDown(int n = 1) { return std::format("\033[{}B", n); }
    inline std::string CursorRight(int n = 1) { return std::format("\033[{}C", n); }
    inline std::string CursorLeft(int n = 1) { return std::format("\033[{}D", n); }
    inline std::string CursorMove(int row, int col) { return std::format("\033[{};{}H", row, col); }

    constexpr const char* SaveCursor = "\033[s";
    constexpr const char* RestoreCursor = "\033[u";
}

class SixelImage {
    sixel_dither_t* dither = nullptr;
    int char_height = 0;

    std::string buffer;

    static int write_fn(const char* data, int size, void* output_buffer) {
        auto* buf = static_cast<std::string*>(output_buffer);
        buf->append(data, static_cast<size_t>(size));
        return size;
    }

public:
    SixelImage(int colors = 64) {
        sixel_dither_new(&dither, colors, nullptr);
    }

    ~SixelImage() {
        if (dither) sixel_dither_unref(dither);
    }

    SixelImage(const SixelImage&) = delete;
    SixelImage& operator=(const SixelImage&) = delete;

    [[nodiscard]] int get_char_height() const { return char_height; }

    void flush_output() {
        if (!buffer.empty()) {
            std::print("{}", buffer);
            buffer.clear();
        }
    }

    static std::vector<unsigned char> process_alpha(const unsigned char* rgba, int w, int h) {
        std::vector<unsigned char> rgb(static_cast<size_t>(w) * static_cast<size_t>(h) * 3);
        for (int i = 0; i < w * h; ++i) {
            const auto a = rgba[i * 4 + 3];
            const auto idx = static_cast<size_t>(i);
            if (a < 128) {
                rgb[idx * 3 + 0] = 220;
                rgb[idx * 3 + 1] = 220;
                rgb[idx * 3 + 2] = 220;
            } else {
                rgb[idx * 3 + 0] = rgba[idx * 4 + 0];
                rgb[idx * 3 + 1] = rgba[idx * 4 + 1];
                rgb[idx * 3 + 2] = rgba[idx * 4 + 2];
            }
        }
        return rgb;
    }

    bool print_centered(const char* filename, int area_width, int area_height) {
        int w, h, channels;
        unsigned char* pixels = stbi_load(filename, &w, &h, &channels, 4);
        if (!pixels) {
            return false;
        }

        std::vector<unsigned char> rgb_pixels = process_alpha(pixels, w, h);

        int target_w = w;
        int target_h = h;

        if (w > area_width || h > area_height) {
            double scale = std::min(static_cast<double>(area_width) / w, static_cast<double>(area_height) / h);
            target_w = static_cast<int>(w * scale);
            target_h = static_cast<int>(h * scale);
        }

        int offset_x = (area_width - target_w) / 2;
        int offset_y = (area_height - target_h) / 2;

        SIXELSTATUS status;
        std::vector<unsigned char> final_pixels;

        if (target_w != w || target_h != h) {
            final_pixels.resize(static_cast<size_t>(target_w) * static_cast<size_t>(target_h) * 3);
            status = sixel_helper_scale_image(
                final_pixels.data(), rgb_pixels.data(), w, h, SIXEL_PIXELFORMAT_RGB888,
                target_w, target_h, SIXEL_RES_BILINEAR, nullptr
            );
            stbi_image_free(pixels);
            if (SIXEL_FAILED(status)) return false;
        } else {
            final_pixels = std::move(rgb_pixels);
            stbi_image_free(pixels);
        }

        char_height = (area_height + 5) / 6;

        const auto offset_x_chars = offset_x / 9;
        const auto offset_y_chars = offset_y / 6;

        buffer.append(static_cast<size_t>(offset_y_chars), '\n');

        buffer.append(static_cast<size_t>(offset_x_chars), ' ');

        sixel_output_t* output = nullptr;
        sixel_output_new(&output, sixel_write_function(write_fn), &buffer, nullptr);

        sixel_dither_initialize(dither, final_pixels.data(), target_w, target_h,
                               SIXEL_PIXELFORMAT_RGB888,
                               SIXEL_LARGE_AUTO, SIXEL_REP_CENTER_BOX, SIXEL_QUALITY_LOW);
        status = sixel_encode(final_pixels.data(), target_w, target_h, 3, dither, output);

        sixel_output_unref(output);

        if (SIXEL_FAILED(status)) return false;

        return true;
    }

    bool print(const char* filename, int width = 0) {
        int w, h, channels;
        unsigned char* pixels = stbi_load(filename, &w, &h, &channels, 3);
        if (!pixels) {
            return false;
        }

        int target_w = w, target_h = h;
        if (width > 0) {
            target_w = width;
            target_h = static_cast<int>(static_cast<long long>(h) * width / w);
        }

        SIXELSTATUS status;
        sixel_output_t* output = nullptr;
        sixel_output_new(&output, sixel_write_function(write_fn), &buffer, nullptr);

        if (target_w != w || target_h != h) {
            std::vector<unsigned char> resized(static_cast<size_t>(target_w) * static_cast<size_t>(target_h) * 3);
            status = sixel_helper_scale_image(
                resized.data(), pixels, w, h, SIXEL_PIXELFORMAT_RGB888,
                target_w, target_h, SIXEL_RES_BILINEAR, nullptr
            );

            if (!SIXEL_FAILED(status)) {
                sixel_dither_initialize(dither, resized.data(), target_w, target_h,
                                       SIXEL_PIXELFORMAT_RGB888,
                                       SIXEL_LARGE_AUTO, SIXEL_REP_CENTER_BOX, SIXEL_QUALITY_LOW);
                status = sixel_encode(resized.data(), target_w, target_h, 3, dither, output);
            }
        } else {
            sixel_dither_initialize(dither, pixels, w, h, SIXEL_PIXELFORMAT_RGB888,
                                   SIXEL_LARGE_AUTO, SIXEL_REP_CENTER_BOX, SIXEL_QUALITY_LOW);
            status = sixel_encode(pixels, w, h, 3, dither, output);
        }

        stbi_image_free(pixels);
        sixel_output_unref(output);

        if (SIXEL_FAILED(status)) return false;

        char_height = (target_h + 5) / 6;
        buffer.push_back('\n');
        return true;
    }
};

class Here {
public:
    template <typename Fmt, typename... Args>
    static void Print(Fmt&& fmt, Args&&... args) {
        std::print(std::forward<Fmt>(fmt), std::forward<Args>(args)...);
    }

    template <typename Fmt, typename... Args>
    static void PrintLine(Fmt&& fmt, Args&&... args) {
        std::print(std::forward<Fmt>(fmt), std::forward<Args>(args)...);
        std::print("\n");
    }

    static void PrintHeader(const char* title) {
        std::print("\033[1;36m=== {} ===\033[0m\n", title);
    }

    static void PrintTable(const std::vector<std::pair<std::string, std::string>>& rows) {
        for (const auto& [key, value] : rows) {
            std::print("\033[1;33m{:<16}\033[0m : {}\n", key, value);
        }
    }

    static bool PrintImage(const char* filename, int width = 0) {
        SixelImage img;
        return img.print(filename, width);
    }

    static std::string CreateProgressBar(double percentage, int width = 20) {
        std::string bar    = "-=[";
        int         filled = static_cast<int>(percentage * width / 100.0);
        for (int i = 0; i < width; i++)
            if (i < filled) bar += "/";
            else bar += " ";
        bar += " ]=-";
        return bar;
    }

    static std::vector<std::string> GetSystemInfoLines() {
        std::vector<std::string> lines;

        try {
            std::string userName     = Utils::ws2s(SystemInfoCollector::GetUserName());
            std::string computerName = Utils::ws2s(SystemInfoCollector::GetComputerName());

            lines.emplace_back("");

            lines.push_back(ColorTheme::ColorizeText(userName + "@" + computerName));
            lines.emplace_back(userName.length() + computerName.length() + 1, '-');

            lines.push_back(ColorTheme::ColorizeKeyValue("OS:", Utils::ws2s(SystemInfoCollector::GetOSName())));
            lines.push_back(ColorTheme::ColorizeKeyValue("Build:", Utils::ws2s(SystemInfoCollector::GetOSVersion())));
            lines.push_back(ColorTheme::ColorizeKeyValue("Uptime:", Utils::ws2s(SystemInfoCollector::GetUptime())));
            lines.push_back(ColorTheme::ColorizeKeyValue("Resolution:", Utils::ws2s(SystemInfoCollector::GetResolution())));
            lines.push_back(ColorTheme::ColorizeKeyValue("Terminal:", "PowerShell"));
            lines.push_back(ColorTheme::ColorizeKeyValue("CPU:", Utils::ws2s(SystemInfoCollector::GetCPUModel())));
            lines.push_back(ColorTheme::ColorizeKeyValue("GPU:", Utils::ws2s(SystemInfoCollector::GetGPUModel())));

            double memUsage = SystemInfoCollector::GetMemoryUsagePercent();
            lines.push_back(ColorTheme::ColorizeKeyValue(
                "Memory:", Utils::ws2s(SystemInfoCollector::GetMemoryInfo()) + " (" + std::to_string(static_cast<int>(memUsage))
                               + "% in use)"));

            lines.push_back(ColorTheme::ColorizeKeyValue("Disk:", Utils::ws2s(SystemInfoCollector::GetDiskInfo())));
            lines.emplace_back("");

            lines.push_back(ColorTheme::ColorizeKeyValue("Mem%: ", CreateProgressBar(memUsage)));
            lines.emplace_back("");

            double diskUsage = SystemInfoCollector::GetDiskUsagePercent();
            lines.push_back(ColorTheme::ColorizeKeyValue("Disk%:", CreateProgressBar(diskUsage)));

            lines.emplace_back("");
            lines.emplace_back("");

            lines.emplace_back("");
            lines.emplace_back("");

            lines.emplace_back("");

        } catch (std::exception const& e) { lines.push_back("Error: " + std::string(e.what())); }

        return lines;
    }

    static bool FileExists(const char* path) {
        if (FILE* f = fopen(path, "rb")) {
            fclose(f);
            return true;
        }
        return false;
    }

    static void PrintNeofetchASCII() {
        Utils::SetConsoleUTF8();
        Utils::EnableVirtualTerminalProcessing();

        const auto infoLines = GetSystemInfoLines();
        const auto logoLines = AsciiArt::GetColoredOSArt();

        std::string buffer;
        buffer.reserve(8192);

        for (size_t infoIdx = 0; infoIdx < infoLines.size(); ++infoIdx) {
            constexpr int GAP = 4;
            constexpr int LEFT_PADDING = 4;
            if (infoIdx == 0) {
                buffer.push_back('\n');
                continue;
            }

            const size_t logoIdx = infoIdx - 1;

            buffer.append(LEFT_PADDING, ' ');

            if (logoIdx < logoLines.size()) {
                std::string logoLine = Utils::ws2s(logoLines[logoIdx]);
                buffer.append(logoLine);
            } else {
                constexpr int LOGO_WIDTH = 40;
                buffer.append(LOGO_WIDTH, ' ');
            }

            buffer.append(GAP, ' ');
            if (infoIdx == 18) {
                buffer += ColorTheme::GetMonetFirstRow();
            } else if (infoIdx == 19) {
                buffer += ColorTheme::GetMonetSecondRow();
            }
            else {
                buffer += infoLines[infoIdx];
            }
            buffer.push_back('\n');
        }

        std::print("{}\n", buffer);
        std::print("\n");
    }

    static void PrintNeofetch(const char* image_path, [[maybe_unused]] int image_width) {
        Utils::SetConsoleUTF8();
        Utils::EnableVirtualTerminalProcessing();

        if (!FileExists(image_path)) {
            PrintNeofetchASCII();
            return;
        }

        const auto infoLines = GetSystemInfoLines();

        constexpr int AREA_WIDTH = 300;
        constexpr int AREA_HEIGHT = 300;

        constexpr auto area_char_width = (AREA_WIDTH + 8) / 9;
        constexpr auto gap = 10;
        constexpr auto left_column_width = area_char_width + gap;

        std::string rightBuffer;
        rightBuffer.reserve(4096);

        for (size_t i = 0; i < infoLines.size(); ++i) {
            rightBuffer.append(left_column_width, ' ');
            if (i == 18) {
                rightBuffer += ColorTheme::GetMonetFirstRow();
            } else if (i == 19) {
                rightBuffer += ColorTheme::GetMonetSecondRow();
            } else {
                rightBuffer += infoLines[i];
            }
            rightBuffer.push_back('\n');
        }

        std::print("{}", rightBuffer);

        std::print("{}", ANSI::SaveCursor);

        std::print("{}", ANSI::CursorUp(static_cast<int>(infoLines.size())));
        std::print("{}", ANSI::CursorLeft(1000));
        std::print("{}", ANSI::CursorRight(4));

        std::print("{}", ANSI::CursorDown(1));

        SixelImage img;
        img.print_centered(image_path, AREA_WIDTH, AREA_HEIGHT);
        img.flush_output();

        std::print("{}", ANSI::CursorDown(1));

        std::print("{}{}", ANSI::RestoreCursor, ANSI::Reset);
    }

    static void PrintNeofetch(
        const char* image_path,
        const int image_width,
        const std::vector<std::pair<std::string, std::string>>& info,
        const char* title_color = ANSI::FgCyan,
        const char* key_color = ANSI::FgBrightGreen,
        const char* separator = ": "
    ) {
        SixelImage img;

        std::print("{}", ANSI::SaveCursor);
        img.print(image_path, image_width);

        const auto img_height = img.get_char_height();
        constexpr auto gap = 4;

        std::print("{}{}", ANSI::CursorUp(img_height + 1), ANSI::CursorRight(image_width / 2 + gap));

        for (size_t i = 0; i < info.size(); ++i) {
            const auto& [key, value] = info[i];

            std::print("{}{}{}{}{}{}{}\n",
                title_color, ANSI::Bold, key, ANSI::Reset,
                separator,
                key_color, value, ANSI::Reset);

            if (i < info.size() - 1 && i < static_cast<size_t>(img_height)) {
                std::print("{}", ANSI::CursorRight(image_width / 2 + gap));
            }
        }

        std::print("{}", ANSI::Reset);
    }
};

#endif
