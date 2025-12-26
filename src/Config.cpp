#include "Config.hpp"
#include <fstream>
#include <sstream>
#include <print>
#include <windows.h>

std::string Config::getExeDir() {
    char path[MAX_PATH]{};
    GetModuleFileNameA(nullptr, path, MAX_PATH);
    const auto exePath = std::string(path);
    return exePath.substr(0, exePath.find_last_of("\\/"));
}

std::string Config::getConfigPath() {
    return getExeDir() + "/config.yaml";
}

std::optional<std::string> Config::getImagePath() {
    const auto configPath = getConfigPath();

    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {try {
            createDefaultConfig();
        } catch (const std::exception&) {
            }
        return std::nullopt;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.starts_with("image_path:") || line.starts_with("image_path=")) {
            const auto colonPos = line.find(':');
            const auto equalPos = line.find('=');
            const auto delimPos = (colonPos != std::string::npos) ? colonPos : equalPos;

            if (delimPos != std::string::npos && delimPos + 1 < line.length()) {
                auto imagePath = line.substr(delimPos + 1);
                const auto start = imagePath.find_first_not_of(" \t");
                const auto end = imagePath.find_last_not_of(" \t");
                if (start != std::string::npos && end != std::string::npos) {
                    imagePath = imagePath.substr(start, end - start + 1);
                }
                configFile.close();
                return imagePath;
            }
        }
    }

    configFile.close();
    return std::nullopt;
}

void Config::setImagePath(std::string_view imagePath) {
    const auto configPath = getConfigPath();

    const auto content = std::format(
        "# Yume Configuration File\n"
        "# Set custom image path (relative or absolute)\n"
        "image_path: {}\n",
        imagePath
    );

    std::ofstream configFile(configPath);
    if (configFile.is_open()) {
        configFile << content;
        configFile.close();
        std::println("Image path set to: {} and saved to config.yaml", imagePath);
    } else {
        std::println(stderr, "Error saving config.yaml: Unable to write file");
    }
}

void Config::createDefaultConfig() {
    const auto configPath = getConfigPath();

    std::ofstream configFile(configPath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to create config file");
    }

    configFile << "# Yume Configuration File\n";
    configFile << "#\n";
    configFile << "# Image Path Configuration\n";
    configFile << "#   Set the path to your avatar/image file\n";
    configFile << "#   Can be relative to exe directory or absolute path\n";
    configFile << "#\n";
    configFile << "image_path: ./avatar\n";

    configFile.close();
}
