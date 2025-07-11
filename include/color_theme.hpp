#ifndef COLOR_THEME_MERGED_HPP
#define COLOR_THEME_MERGED_HPP

#include <head.hpp>

/**
 * 颜色主题管理器
 * 负责管理各种颜色主题，包括莫奈风格的RGB颜色编码
 */
class ColorTheme {
public:
    // 莫奈风格颜色主题
    struct MonetPalette {
        // 睡莲的蓝与紫 (Water Lilies' Blues & Purples) - 背景色
        char static constexpr const* SOFT_SKY_BLUE_BG       = "\033[48;2;148;172;208m"; // 柔和天蓝
        char static constexpr const* MORNING_MIST_PURPLE_BG = "\033[48;2;176;186;222m"; // 晨雾紫
        char static constexpr const* DEEP_WATER_BLUE_BG     = "\033[48;2;95;123;159m";  // 深水蓝
        char static constexpr const* LIGHT_LILAC_BG         = "\033[48;2;203;195;227m"; // 淡丁香紫

        // 日出与光影 (Sunrise & Light) - 背景色
        char static constexpr const* SOFT_SUNLIGHT_YELLOW_BG = "\033[48;2;243;213;163m"; // 柔和日光黄
        char static constexpr const* CORAL_PINK_ORANGE_BG    = "\033[48;2;238;173;143m"; // 珊瑚粉橙
        char static constexpr const* BRIGHT_CLOUD_WHITE_BG   = "\033[48;2;249;236;219m"; // 亮云白
        char static constexpr const* ROSY_GLOW_BG            = "\033[48;2;216;156;148m"; // 玫瑰色霞光

        // 花园与绿植 (Garden & Greens) - 背景色
        char static constexpr const* SAGE_GREEN_BG       = "\033[48;2;153;180;151m"; // 鼠尾草绿
        char static constexpr const* WILLOW_GREEN_BG     = "\033[48;2;187;204;168m"; // 杨柳绿
        char static constexpr const* OLIVE_GRAY_GREEN_BG = "\033[48;2;126;149;123m"; // 橄榄灰绿
        char static constexpr const* MINT_CREAM_BG       = "\033[48;2;204;221;203m"; // 薄荷乳白

        // 干草堆与大地 (Haystacks & Earth) - 背景色
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

        // 重置颜色
        char static constexpr const* RESET = "\033[0m";
    };

private:
    // 当前会话的随机颜色（保证一致性）
    std::string static currentSessionColor;
    bool static isSessionColorInitialized;

public:
    // 初始化随机颜色种子
    void static InitRandomSeed() { srand(static_cast<unsigned int>(time(nullptr))); }

    // 获取当前会话的随机颜色（前景色）
    std::string static GetSessionColor() {
        if (!isSessionColorInitialized) {
            InitRandomSeed();
            currentSessionColor       = GetRandomMonetColorFG();
            isSessionColorInitialized = true;
        }
        return currentSessionColor;
    }

    // 为文本添加当前会话颜色
    std::string static ColorizeText(std::string const& text) {
        return GetSessionColor() + text + MonetPalette::RESET;
    }

    // 为关键字添加颜色（如 "OS:", "CPU:" 等）
    std::string static ColorizeKeyValue(std::string const& keyword, std::string const& value) {
        return ColorizeText(keyword) + " " + value;
    }

    // 获取莫奈风格的第一排颜色块字符串
    std::string static GetMonetFirstRow() {
        std::ostringstream oss;
        oss << MonetPalette::SOFT_SKY_BLUE_BG << "    "        // 柔和天蓝
            << MonetPalette::MORNING_MIST_PURPLE_BG << "    "  // 晨雾紫
            << MonetPalette::DEEP_WATER_BLUE_BG << "    "      // 深水蓝
            << MonetPalette::LIGHT_LILAC_BG << "    "          // 淡丁香紫
            << MonetPalette::SOFT_SUNLIGHT_YELLOW_BG << "    " // 柔和日光黄
            << MonetPalette::CORAL_PINK_ORANGE_BG << "    "    // 珊瑚粉橙
            << MonetPalette::BRIGHT_CLOUD_WHITE_BG << "    "   // 亮云白
            << MonetPalette::ROSY_GLOW_BG << "    "            // 玫瑰色霞光
            << MonetPalette::RESET;
        return oss.str();
    }

    // 获取莫奈风格的第二排颜色块字符串
    std::string static GetMonetSecondRow() {
        std::ostringstream oss;
        oss << MonetPalette::SAGE_GREEN_BG << "    "       // 鼠尾草绿
            << MonetPalette::WILLOW_GREEN_BG << "    "     // 杨柳绿
            << MonetPalette::OLIVE_GRAY_GREEN_BG << "    " // 橄榄灰绿
            << MonetPalette::MINT_CREAM_BG << "    "       // 薄荷乳白
            << MonetPalette::STRAW_YELLOW_BG << "    "     // 麦秆黄
            << MonetPalette::LINEN_BG << "    "            // 亚麻色
            << MonetPalette::TAUPE_BG << "    "            // 灰褐色
            << MonetPalette::DAMP_EARTH_BROWN_BG << "    " // 湿土棕
            << MonetPalette::RESET;
        return oss.str();
    }

    // 创建单个颜色块
    std::string static CreateColorBlock(std::string const& colorCode, int width = 4) {
        std::ostringstream oss;
        oss << colorCode << std::string(width, ' ') << MonetPalette::RESET;
        return oss.str();
    }

    // 获取所有可用的颜色主题名称
    std::vector<std::string> static GetAvailableThemes() {
        return {"Monet", "Classic", "Rainbow", "Grayscale"};
    }

    // 随机获取一种颜色主题名称
    std::string static GetRandomTheme() {
        std::vector<std::string> themes = GetAvailableThemes();
        return themes[rand() % themes.size()];
    }

    // 随机获取一种莫奈风格的颜色编码（前景色）
    std::string static GetRandomMonetColorFG() {
        // 莫奈调色板所有前景色编码
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

// 静态成员变量定义
inline std::string ColorTheme::currentSessionColor = "";
inline bool ColorTheme::isSessionColorInitialized = false;

#endif // COLOR_THEME_MERGED_HPP
