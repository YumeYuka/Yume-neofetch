#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <optional>
#include <string_view>

struct ConfigResult {
    std::string path;
    bool isConfigured;
};

class Config {
public:
    static std::optional<std::string> getImagePath();

    static void setImagePath(std::string_view imagePath);

private:
    static void createDefaultConfig();

    static std::string getConfigPath();

    static std::string getExeDir();
};

#endif // CONFIG_HPP
