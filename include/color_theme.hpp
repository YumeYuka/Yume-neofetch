#ifndef COLOR_THEME_MERGED_HPP
#define COLOR_THEME_MERGED_HPP

#include <head.hpp>

/**
 * 颜色主题管理器
 * 负责管理各种颜色主题，包括莫奈风格的RGB颜色编码
 */
class ColorTheme {
public:
    struct MonetPalette {
        char static constexpr const* SOFT_SKY_BLUE_BG       = "\033[48;2;148;172;208m"; // 柔和天蓝
        char static constexpr const* MORNING_MIST_PURPLE_BG = "\033[48;2;176;186;222m"; // 晨雾紫
        char static constexpr const* DEEP_WATER_BLUE_BG     = "\033[48;2;95;123;159m";  // 深水蓝
        char static constexpr const* LIGHT_LILAC_BG         = "\033[48;2;203;195;227m"; // 淡丁香紫

        char static constexpr const* SOFT_SUNLIGHT_YELLOW_BG = "\033[48;2;243;213;163m"; // 柔和日光黄
        char static constexpr const* CORAL_PINK_ORANGE_BG    = "\033[48;2;238;173;143m"; // 珊瑚粉橙
        char static constexpr const* BRIGHT_CLOUD_WHITE_BG   = "\033[48;2;249;236;219m"; // 亮云白
        char static constexpr const* ROSY_GLOW_BG            = "\033[48;2;216;156;148m"; // 玫瑰色霞光

        char static constexpr const* SAGE_GREEN_BG       = "\033[48;2;153;180;151m"; // 鼠尾草绿
        char static constexpr const* WILLOW_GREEN_BG     = "\033[48;2;187;204;168m"; // 杨柳绿
        char static constexpr const* OLIVE_GRAY_GREEN_BG = "\033[48;2;126;149;123m"; // 橄榄灰绿
        char static constexpr const* MINT_CREAM_BG       = "\033[48;2;204;221;203m"; // 薄荷乳白

        char static constexpr const* STRAW_YELLOW_BG     = "\033[48;2;223;195;152m"; // 麦秆黄
        char static constexpr const* LINEN_BG            = "\033[48;2;196;171;142m"; // 亚麻色
        char static constexpr const* TAUPE_BG            = "\033[48;2;172;147;120m"; // 灰褐色
        char static constexpr const* DAMP_EARTH_BROWN_BG = "\033[48;2;141;126;111m"; // 湿土棕

        // 前景色（文字颜色）版本
        char static constexpr const* SOFT_SKY_BLUE_FG        = "\033[38;2;148;172;208m"; // 柔和天蓝
        char static constexpr const* MORNING_MIST_PURPLE_FG  = "\033[38;2;176;186;222m"; // 晨雾紫
        char static constexpr const* DEEP_WATER_BLUE_FG      = "\033[38;2;95;123;159m";  // 深水蓝
        char static constexpr const* LIGHT_LILAC_FG          = "\033[38;2;203;195;227m"; // 淡丁香紫
        char static constexpr const* SOFT_SUNLIGHT_YELLOW_FG = "\033[38;2;243;213;163m"; // 柔和日光黄
        char static constexpr const* CORAL_PINK_ORANGE_FG    = "\033[38;2;238;173;143m"; // 珊瑚粉橙
        char static constexpr const* BRIGHT_CLOUD_WHITE_FG   = "\033[38;2;249;236;219m"; // 亮云白
        char static constexpr const* ROSY_GLOW_FG            = "\033[38;2;216;156;148m"; // 玫瑰色霞光
        char static constexpr const* SAGE_GREEN_FG           = "\033[38;2;153;180;151m"; // 鼠尾草绿
        char static constexpr const* WILLOW_GREEN_FG         = "\033[38;2;187;204;168m"; // 杨柳绿
        char static constexpr const* OLIVE_GRAY_GREEN_FG     = "\033[38;2;126;149;123m"; // 橄榄灰绿
        char static constexpr const* MINT_CREAM_FG           = "\033[38;2;204;221;203m"; // 薄荷乳白
        char static constexpr const* STRAW_YELLOW_FG         = "\033[38;2;223;195;152m"; // 麦秆黄
        char static constexpr const* LINEN_FG                = "\033[38;2;196;171;142m"; // 亚麻色
        char static constexpr const* TAUPE_FG                = "\033[38;2;172;147;120m"; // 灰褐色
        char static constexpr const* DAMP_EARTH_BROWN_FG     = "\033[38;2;141;126;111m"; // 湿土棕

        char static constexpr const* RESET = "\033[0m";
    };

private:
    std::string static currentSessionColor;
    bool static isSessionColorInitialized;

public:
    void static InitRandomSeed() { srand(static_cast<unsigned int>(time(nullptr))); }

    std::string static GetSessionColor() {
        if (!isSessionColorInitialized) {
            InitRandomSeed();
            currentSessionColor       = GetRandomMonetColorFG();
            isSessionColorInitialized = true;
        }
        return currentSessionColor;
    }

    std::string static ColorizeText(std::string const& text) {
        return GetSessionColor() + text + MonetPalette::RESET;
    }

    std::string static ColorizeKeyValue(std::string const& keyword, std::string const& value) {
        return ColorizeText(keyword) + " " + value;
    }

    std::string static GetMonetFirstRow() {
        std::ostringstream oss;
        oss << MonetPalette::SOFT_SKY_BLUE_BG << "    " << MonetPalette::RESET
            << MonetPalette::MORNING_MIST_PURPLE_BG << "    " << MonetPalette::RESET
            << MonetPalette::DEEP_WATER_BLUE_BG << "    " << MonetPalette::RESET
            << MonetPalette::LIGHT_LILAC_BG << "    " << MonetPalette::RESET
            << MonetPalette::SOFT_SUNLIGHT_YELLOW_BG << "    " << MonetPalette::RESET
            << MonetPalette::CORAL_PINK_ORANGE_BG << "    " << MonetPalette::RESET
            << MonetPalette::BRIGHT_CLOUD_WHITE_BG << "    " << MonetPalette::RESET
            << MonetPalette::ROSY_GLOW_BG << "    " << MonetPalette::RESET;
        return oss.str();
    }

    std::string static GetMonetSecondRow() {
        std::ostringstream oss;
        oss << MonetPalette::SAGE_GREEN_BG << "    " << MonetPalette::RESET
            << MonetPalette::WILLOW_GREEN_BG << "    " << MonetPalette::RESET
            << MonetPalette::OLIVE_GRAY_GREEN_BG << "    " << MonetPalette::RESET
            << MonetPalette::MINT_CREAM_BG << "    " << MonetPalette::RESET
            << MonetPalette::STRAW_YELLOW_BG << "    " << MonetPalette::RESET << MonetPalette::LINEN_BG
            << "    " << MonetPalette::RESET << MonetPalette::TAUPE_BG << "    " << MonetPalette::RESET
            << MonetPalette::DAMP_EARTH_BROWN_BG << "    " << MonetPalette::RESET;
        return oss.str();
    }

    std::string static CreateColorBlock(std::string const& colorCode, int width = 4) {
        std::ostringstream oss;
        oss << colorCode << std::string(width, ' ') << MonetPalette::RESET;
        return oss.str();
    }

    std::vector<std::string> static GetAvailableThemes() {
        return {"Monet", "Classic", "Rainbow", "Grayscale"};
    }

    std::string static GetRandomTheme() {
        std::vector<std::string> themes = GetAvailableThemes();
        return themes[rand() % themes.size()];
    }

    std::string static GetRandomMonetColorFG() {
        std::vector<char const*> colors = {MonetPalette::SOFT_SKY_BLUE_FG,
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
                                           MonetPalette::DAMP_EARTH_BROWN_FG};
        return colors[rand() % colors.size()];
    }
};

std::string inline ColorTheme::currentSessionColor = "";
bool inline ColorTheme::isSessionColorInitialized  = false;

#endif // COLOR_THEME_MERGED_HPP
