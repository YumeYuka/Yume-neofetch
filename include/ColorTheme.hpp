#ifndef COLOR_THEME_HPP
#define COLOR_THEME_HPP

#include <string>
#include <vector>
#include <array>
#include <format>
#include <cstdlib>
#include <ctime>
#include <random>

class ColorTheme {
public:
    struct MonetPalette {
        static constexpr const char* SOFT_SKY_BLUE_BG       = "\033[48;2;148;172;208m";
        static constexpr const char* MORNING_MIST_PURPLE_BG = "\033[48;2;176;186;222m";
        static constexpr const char* DEEP_WATER_BLUE_BG     = "\033[48;2;95;123;159m";
        static constexpr const char* LIGHT_LILAC_BG         = "\033[48;2;203;195;227m";

        static constexpr const char* SOFT_SUNLIGHT_YELLOW_BG = "\033[48;2;243;213;163m";
        static constexpr const char* CORAL_PINK_ORANGE_BG    = "\033[48;2;238;173;143m";
        static constexpr const char* BRIGHT_CLOUD_WHITE_BG   = "\033[48;2;249;236;219m";
        static constexpr const char* ROSY_GLOW_BG            = "\033[48;2;216;156;148m";

        static constexpr const char* SAGE_GREEN_BG       = "\033[48;2;153;180;151m";
        static constexpr const char* WILLOW_GREEN_BG     = "\033[48;2;187;204;168m";
        static constexpr const char* OLIVE_GRAY_GREEN_BG = "\033[48;2;126;149;123m";
        static constexpr const char* MINT_CREAM_BG       = "\033[48;2;204;221;203m";

        static constexpr const char* STRAW_YELLOW_BG     = "\033[48;2;223;195;152m";
        static constexpr const char* LINEN_BG            = "\033[48;2;196;171;142m";
        static constexpr const char* TAUPE_BG            = "\033[48;2;172;147;120m";
        static constexpr const char* DAMP_EARTH_BROWN_BG = "\033[48;2;141;126;111m";

        static constexpr const char* SOFT_SKY_BLUE_FG        = "\033[38;2;148;172;208m";
        static constexpr const char* MORNING_MIST_PURPLE_FG  = "\033[38;2;176;186;222m";
        static constexpr const char* DEEP_WATER_BLUE_FG      = "\033[38;2;95;123;159m";
        static constexpr const char* LIGHT_LILAC_FG          = "\033[38;2;203;195;227m";
        static constexpr const char* SOFT_SUNLIGHT_YELLOW_FG = "\033[38;2;243;213;163m";
        static constexpr const char* CORAL_PINK_ORANGE_FG    = "\033[38;2;238;173;143m";
        static constexpr const char* BRIGHT_CLOUD_WHITE_FG   = "\033[38;2;249;236;219m";
        static constexpr const char* ROSY_GLOW_FG            = "\033[38;2;216;156;148m";
        static constexpr const char* SAGE_GREEN_FG           = "\033[38;2;153;180;151m";
        static constexpr const char* WILLOW_GREEN_FG         = "\033[38;2;187;204;168m";
        static constexpr const char* OLIVE_GRAY_GREEN_FG     = "\033[38;2;126;149;123m";
        static constexpr const char* MINT_CREAM_FG           = "\033[38;2;204;221;203m";
        static constexpr const char* STRAW_YELLOW_FG         = "\033[38;2;223;195;152m";
        static constexpr const char* LINEN_FG                = "\033[38;2;196;171;142m";
        static constexpr const char* TAUPE_FG                = "\033[38;2;172;147;120m";
        static constexpr const char* DAMP_EARTH_BROWN_FG     = "\033[38;2;141;126;111m";

        static constexpr const char* RESET = "\033[0m";
    };

private:
    static std::string currentSessionColor;
    static bool isSessionColorInitialized;
    static std::mt19937 rng;

    static void InitRandomSeed() {
        rng.seed(std::random_device{}());
    }

public:
    static std::string GetSessionColor() {
        if (!isSessionColorInitialized) {
            InitRandomSeed();
            currentSessionColor       = GetRandomMonetColorFG();
            isSessionColorInitialized = true;
        }
        return currentSessionColor;
    }

    static std::string ColorizeText(const std::string& text) {
        return std::format("{}{}{}", GetSessionColor(), text, MonetPalette::RESET);
    }

    static std::string ColorizeKeyValue(const std::string& keyword, const std::string& value) {
        return ColorizeText(keyword) + " " + value;
    }

    static std::string GetMonetFirstRow() {
        constexpr auto makeBlock = [](const char* color) {
            return std::format("{}{:<4}{}", color, "", MonetPalette::RESET);
        };
        return std::format("{}{}{}{}{}{}{}{}",
            makeBlock(MonetPalette::SOFT_SKY_BLUE_BG),
            makeBlock(MonetPalette::MORNING_MIST_PURPLE_BG),
            makeBlock(MonetPalette::DEEP_WATER_BLUE_BG),
            makeBlock(MonetPalette::LIGHT_LILAC_BG),
            makeBlock(MonetPalette::SOFT_SUNLIGHT_YELLOW_BG),
            makeBlock(MonetPalette::CORAL_PINK_ORANGE_BG),
            makeBlock(MonetPalette::BRIGHT_CLOUD_WHITE_BG),
            makeBlock(MonetPalette::ROSY_GLOW_BG)
        );
    }

    static std::string GetMonetSecondRow() {
        constexpr auto makeBlock = [](const char* color) {
            return std::format("{}{:<4}{}", color, "", MonetPalette::RESET);
        };
        return std::format("{}{}{}{}{}{}{}{}",
            makeBlock(MonetPalette::SAGE_GREEN_BG),
            makeBlock(MonetPalette::WILLOW_GREEN_BG),
            makeBlock(MonetPalette::OLIVE_GRAY_GREEN_BG),
            makeBlock(MonetPalette::MINT_CREAM_BG),
            makeBlock(MonetPalette::STRAW_YELLOW_BG),
            makeBlock(MonetPalette::LINEN_BG),
            makeBlock(MonetPalette::TAUPE_BG),
            makeBlock(MonetPalette::DAMP_EARTH_BROWN_BG)
        );
    }

    static std::string CreateColorBlock(const std::string& colorCode, int width = 4) {
        return std::format("{}{:<{}}{}", colorCode, "", width, MonetPalette::RESET);
    }

    static std::vector<std::string> GetAvailableThemes() {
        return {"Monet", "Classic", "Rainbow", "Grayscale"};
    }

    static std::string GetRandomTheme() {
        const auto themes = GetAvailableThemes();
        std::uniform_int_distribution<size_t> dist(0, themes.size() - 1);
        return themes[dist(rng)];
    }

    static std::string GetRandomMonetColorFG() {
        constexpr std::array<const char*, 16> colors = {
            MonetPalette::SOFT_SKY_BLUE_FG,
            MonetPalette::MORNING_MIST_PURPLE_FG,
            MonetPalette::DEEP_WATER_BLUE_FG,
            MonetPalette::LIGHT_LILAC_FG,
            MonetPalette::SOFT_SUNLIGHT_YELLOW_FG,
            MonetPalette::CORAL_PINK_ORANGE_FG,
            MonetPalette::BRIGHT_CLOUD_WHITE_FG,
            MonetPalette::ROSY_GLOW_FG,
            MonetPalette::SAGE_GREEN_FG,
            MonetPalette::WILLOW_GREEN_FG,
            MonetPalette::OLIVE_GRAY_GREEN_FG,
            MonetPalette::MINT_CREAM_FG,
            MonetPalette::STRAW_YELLOW_FG,
            MonetPalette::LINEN_FG,
            MonetPalette::TAUPE_FG,
            MonetPalette::DAMP_EARTH_BROWN_FG
        };
        std::uniform_int_distribution<size_t> dist(0, colors.size() - 1);
        return colors[dist(rng)];
    }
};

inline std::string ColorTheme::currentSessionColor;
inline bool ColorTheme::isSessionColorInitialized = false;
inline std::mt19937 ColorTheme::rng = std::mt19937(std::random_device{}());

#endif // COLOR_THEME_HPP
