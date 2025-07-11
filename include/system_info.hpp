#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP

#include <string>
#include <vector>
#include <windows.h>

// 系统信息结构体
struct SystemInfo {
    std::wstring osName;
    std::wstring osVersion;
    std::wstring cpuModel;
    std::wstring userName;
    std::wstring computerName;
    std::wstring uptime;
    std::wstring resolution;
    std::wstring gpuModel;
    std::wstring memoryInfo;
    std::wstring diskInfo;
    double memoryUsagePercent;
    double diskUsagePercent;
};

// 系统信息收集器类
class SystemInfoCollector {
public:
    SystemInfoCollector();
    ~SystemInfoCollector();

    // 获取完整系统信息
    SystemInfo GetSystemInfo();

    // 获取具体信息的方法
    std::wstring GetOSName();
    std::wstring GetOSVersion();
    std::wstring GetCPUModel();
    std::wstring GetUserName();
    std::wstring GetComputerName();
    std::wstring GetUptime();
    std::wstring GetResolution();
    std::wstring GetGPUModel();
    std::wstring GetMemoryInfo();
    std::wstring GetDiskInfo();
    double GetMemoryUsagePercent();
    double GetDiskUsagePercent();

private:
    // 私有辅助方法
    std::wstring GetRegistryValue(HKEY hKey, const std::wstring& subKey, const std::wstring& valueName);
    std::wstring FormatBytes(ULONGLONG bytes);
    std::wstring FormatUptime(ULONGLONG milliseconds);
};

#endif // SYSTEM_INFO_HPP
