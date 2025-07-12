#include "Config.hpp"
#include <head.hpp>
#include <filesystem>
#include <windows.h>


static std::string GetExeDir() {
    char path[MAX_PATH] = {0};
    GetModuleFileNameA(NULL, path, MAX_PATH);
    std::string exePath(path);
    return exePath.substr(0, exePath.find_last_of("\\/"));
}

std::string Config::getLogoName() {
    std::string configPath = GetExeDir() + "/config.yaml";
    std::string logoName = "windows";

    std::ifstream configFile(configPath);
    if (!configFile.is_open()) {
        std::cerr << "config.yaml not found. Creating with default logo.\n";
        try {
            createDefaultConfig();
        } catch (const std::exception& e) {
            std::cerr << "Error creating config.yaml: " << e.what() << "\n";
        }
        return logoName;
    }

    std::string line;
    while (std::getline(configFile, line)) {
        if (line.empty() || line[0] == '#') continue;

        if (line.find("logo:") == 0 || line.find("logo=") == 0) {
            size_t colonPos = line.find(':');
            size_t equalPos = line.find('=');
            size_t delimPos = (colonPos != std::string::npos) ? colonPos : equalPos;

            if (delimPos != std::string::npos && delimPos + 1 < line.length()) {
                logoName = line.substr(delimPos + 1);
                logoName.erase(0, logoName.find_first_not_of(" \t"));
                logoName.erase(logoName.find_last_not_of(" \t") + 1);
                break;
            }
        }
    }

    configFile.close();
    return logoName;
}

void Config::setLogoName(const std::string& logoName) {
    std::string configPath = GetExeDir() + "/config.yaml";

    std::ostringstream newContent;
    newContent << "# Yume-neofetch Configuration File\n";
    newContent << "# Available logos: windows, arch, debian, ubuntu, fedora, macos, mint, opensuse, gentoo\n";
    newContent << "logo: " << logoName << "\n";

    std::ofstream configFile(configPath);
    if (configFile.is_open()) {
        configFile << newContent.str();
        configFile.close();
        std::cout << "Logo set to: " << logoName << " and saved to config.yaml\n";
    } else {
        std::cerr << "Error saving config.yaml: Unable to write file\n";
    }
}

void Config::createDefaultConfig() {
    std::string configPath = GetExeDir() + "/config.yaml";

    std::ofstream configFile(configPath);
    if (!configFile.is_open()) {
        throw std::runtime_error("Unable to create config file");
    }

    configFile << "# Yume-neofetch Configuration File\n";
    configFile << "# This file controls the ASCII art logo displayed\n";
    configFile << "# Available logos: windows, arch, debian, ubuntu, fedora, macos, mint, opensuse, gentoo\n";
    configFile << "\n";
    configFile << "# Current logo setting\n";
    configFile << "logo: windows\n";
    configFile << "\n";
    configFile << "# Example: Change to different logo\n";
    configFile << "# logo: arch\n";
    configFile << "# logo: ubuntu\n";
    configFile << "# logo: debian\n";

    configFile.close();
}
