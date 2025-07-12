#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <head.hpp>

/**
 * 配置管理类 - 零依赖实现
 *
 * 这个类负责管理Yume-neofetch的配置文件，主要用于设置ASCII艺术logo
 * 使用简单的键值对格式，完全不依赖yaml-cpp，减小程序体积
 *
 * 配置文件格式：
 * # 注释行
 * logo: windows
 *
 * 支持的logo类型：
 * - windows (默认)
 * - arch
 * - debian
 * - ubuntu
 * - fedora
 * - macos
 * - mint
 * - opensuse
 * - gentoo
 */
class Config {
public:
    /**
     * 获取当前配置的logo名称
     *
     * @return 返回logo名称字符串，如果配置文件不存在或解析失败，返回默认值"windows"
     */
    static std::string getLogoName();

    /**
     * 设置logo名称并保存到配置文件
     *
     * @param logoName 要设置的logo名称
     */
    static void setLogoName(const std::string& logoName);

private:
    /**
     * 创建默认配置文件
     *
     * @throws std::runtime_error 如果无法创建配置文件
     */
    static void createDefaultConfig();
};

#endif // CONFIG_HPP
