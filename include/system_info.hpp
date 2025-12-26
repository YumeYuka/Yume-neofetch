#ifndef SYSTEM_INFO_HPP
#define SYSTEM_INFO_HPP

#include <string>
#include <vector>
#include <windows.h>

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

class SystemInfoCollector {
public:
    SystemInfoCollector() = default;
    ~SystemInfoCollector() = default;

    static SystemInfo GetSystemInfo();
    static std::wstring GetOSName();
    static std::wstring GetOSVersion();
    static std::wstring GetCPUModel();
    static std::wstring GetUserName();
    static std::wstring GetComputerName();
    static std::wstring GetUptime();
    static std::wstring GetResolution();
    static std::wstring GetGPUModel();
    static  std::wstring GetMemoryInfo();
    static  std::wstring GetDiskInfo();
    static  double GetMemoryUsagePercent();
     static double GetDiskUsagePercent();

private:
    static  std::wstring GetRegistryValue(HKEY hKey, std::wstring_view subKey, std::wstring_view valueName);
    static std::wstring FormatBytes(ULONGLONG bytes);
    static std::wstring FormatUptime(ULONGLONG milliseconds);
};

#endif // SYSTEM_INFO_HPP
