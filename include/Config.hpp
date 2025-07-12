#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <head.hpp>

class Config {
public:
    std::string static getLogoName();
    void static setLogoName(std::string const& logoName);

private:
    void static createDefaultConfig();
};

#endif // CONFIG_HPP
