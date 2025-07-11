#include "system_info.hpp"
#include <windows.h>
#include <psapi.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <pdh.h>
#include <pdhmsg.h>
#include <winternl.h>
#include <ntstatus.h>
#include <dxgi.h>
#include <algorithm>


// 为MinGW编译器定义DXGI接口ID
#ifdef __GNUC__
extern "C" {
    const GUID IID_IDXGIFactory = {0x7b7166ec, 0x21c7, 0x44ae, {0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69}};
}
#endif

SystemInfoCollector::SystemInfoCollector() {
    // 构造函数
}

SystemInfoCollector::~SystemInfoCollector() {
    // 析构函数
}

SystemInfo SystemInfoCollector::GetSystemInfo() {
    SystemInfo info;
    info.osName = GetOSName();
    info.osVersion = GetOSVersion();
    info.cpuModel = GetCPUModel();
    info.userName = GetUserName();
    info.computerName = GetComputerName();
    info.uptime = GetUptime();
    info.resolution = GetResolution();
    info.gpuModel = GetGPUModel();
    info.memoryInfo = GetMemoryInfo();
    info.diskInfo = GetDiskInfo();
    info.memoryUsagePercent = GetMemoryUsagePercent();
    info.diskUsagePercent = GetDiskUsagePercent();
    return info;
}

std::wstring SystemInfoCollector::GetOSName() {
    // 首先检查是否是Windows 11
    std::wstring build = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

    if (!build.empty()) {
        int buildNumber = _wtoi(build.c_str());

        // Windows 11的构建号从22000开始
        if (buildNumber >= 22000) {
            return L"Windows 11";
        }
    }

    // 如果不是Windows 11，获取ProductName
    std::wstring osName = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ProductName");

    if (osName.empty()) {
        osName = L"Windows";
    }

    return osName;
}

std::wstring SystemInfoCollector::GetOSVersion() {
    // 获取更准确的Windows版本信息
    std::wstring build = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

    std::wstring displayVersion = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"DisplayVersion");

    std::wstring ubr = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"UBR");

    if (!build.empty()) {
        int buildNumber = _wtoi(build.c_str());

        // 检查版本号是否合理
        if (buildNumber >= 22000) {
            // Windows 11 (包括所有版本和预览版)
            if (!displayVersion.empty()) {
                // 如果是Dev分支，显示Dev (构建号)
                if (displayVersion.find(L"Dev") != std::wstring::npos || buildNumber > 26000) {
                    return L"Dev (" + build + L")";
                } else {
                    return displayVersion + L" (" + build + L")";
                }
            } else {
                // 对于预览版或开发版，显示 Dev (构建号)
                if (buildNumber > 26000) {
                    return L"Dev (" + build + L")";
                } else {
                    return L"21H2 (" + build + L")";  // Windows 11 默认版本
                }
            }
        } else if (buildNumber >= 10240 && buildNumber < 22000) {
            // Windows 10
            if (!displayVersion.empty()) {
                return displayVersion;
            } else {
                // 根据构建号推断Windows 10版本
                if (buildNumber >= 19041) return L"2004";
                else if (buildNumber >= 18363) return L"1909";
                else if (buildNumber >= 18362) return L"1903";
                else if (buildNumber >= 17763) return L"1809";
                else if (buildNumber >= 17134) return L"1803";
                else if (buildNumber >= 16299) return L"1709";
                else if (buildNumber >= 15063) return L"1703";
                else if (buildNumber >= 14393) return L"1607";
                else if (buildNumber >= 10586) return L"1511";
                else return L"1507";
            }
        } else {
            // 版本号异常或者是很新的开发版本
            return L"Dev (" + build + L")";
        }
    }

    return L"Unknown Build";
}

std::wstring SystemInfoCollector::GetCPUModel() {
    std::wstring cpuName = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", L"ProcessorNameString");

    if (cpuName.empty()) {
        cpuName = L"Unknown CPU";
    }

    return cpuName;
}

std::wstring SystemInfoCollector::GetUserName() {
    wchar_t username[256];
    DWORD size = sizeof(username) / sizeof(username[0]);

    if (GetUserNameW(username, &size)) {
        return std::wstring(username);
    }

    return L"Unknown User";
}

std::wstring SystemInfoCollector::GetComputerName() {
    wchar_t computerName[256];
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);

    if (GetComputerNameW(computerName, &size)) {
        return std::wstring(computerName);
    }

    return L"Unknown Computer";
}

std::wstring SystemInfoCollector::GetUptime() {
    ULONGLONG uptime = GetTickCount64();
    return FormatUptime(uptime);
}

std::wstring SystemInfoCollector::GetResolution() {
    std::wstring result = L"";

    // 获取主显示器的真实分辨率（不受DPI缩放影响）
    HDC hdc = GetDC(NULL);
    if (hdc) {
        int screenWidth = GetDeviceCaps(hdc, HORZRES);
        int screenHeight = GetDeviceCaps(hdc, VERTRES);

        // 获取物理分辨率（真实像素）
        int physicalWidth = GetDeviceCaps(hdc, DESKTOPHORZRES);
        int physicalHeight = GetDeviceCaps(hdc, DESKTOPVERTRES);

        ReleaseDC(NULL, hdc);

        // 如果物理分辨率可用且与逻辑分辨率不同，使用物理分辨率
        if (physicalWidth > 0 && physicalHeight > 0 &&
            (physicalWidth != screenWidth || physicalHeight != screenHeight)) {
            screenWidth = physicalWidth;
            screenHeight = physicalHeight;
        }

        result = std::to_wstring(screenWidth) + L"x" + std::to_wstring(screenHeight);
    } else {
        // 备用方案：使用GetSystemMetrics
        int screenWidth = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        result = std::to_wstring(screenWidth) + L"x" + std::to_wstring(screenHeight);
    }

    // 获取刷新率
    DEVMODEW devMode;
    ZeroMemory(&devMode, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);

    // 获取当前显示设置
    if (EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        if (devMode.dmDisplayFrequency > 0 && devMode.dmDisplayFrequency != 1) {
            result += L" @" + std::to_wstring(devMode.dmDisplayFrequency) + L"Hz";
        }
    }

    return result;
}

std::wstring SystemInfoCollector::GetGPUModel() {
    // 使用DXGI来获取显卡信息，特别是独立显卡
    IDXGIFactory* pFactory = nullptr;
    std::vector<std::wstring> gpuList;
    std::wstring dedicatedGPU = L"";

    // 创建DXGI工厂
    HRESULT hr = CreateDXGIFactory(IID_IDXGIFactory, (void**)&pFactory);

    if (SUCCEEDED(hr)) {
        // 遍历所有DXGI适配器(显卡)
        IDXGIAdapter* pAdapter = nullptr;
        for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC adapterDesc;
            if (SUCCEEDED(pAdapter->GetDesc(&adapterDesc))) {
                std::wstring gpuName = adapterDesc.Description;

                // 转换为大写以便不区分大小写地检测
                std::wstring upperName = gpuName;
                std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

                // 检查是否为集成显卡
                bool isIntegrated = 
                    upperName.find(L"INTEL") != std::wstring::npos ||
                    upperName.find(L"UHD GRAPHICS") != std::wstring::npos ||
                    upperName.find(L"HD GRAPHICS") != std::wstring::npos ||
                    upperName.find(L"IRIS") != std::wstring::npos ||
                    (upperName.find(L"RADEON") != std::wstring::npos && 
                     upperName.find(L"GRAPHICS") != std::wstring::npos) ||
                    upperName.find(L"780M") != std::wstring::npos;  // 特别排除Radeon 780M集成显卡

                // 只有非集成显卡才添加到列表中
                if (!isIntegrated) {
                    gpuList.push_back(gpuName);

                    // 检查是否是NVIDIA或独立AMD显卡
                    if (upperName.find(L"NVIDIA") != std::wstring::npos || 
                        (upperName.find(L"AMD") != std::wstring::npos && 
                         upperName.find(L"RADEON") != std::wstring::npos && 
                         upperName.find(L"780M") == std::wstring::npos)) { // 排除AMD集成显卡
                        dedicatedGPU = gpuName;
                    }
                }
            }
            pAdapter->Release();
        }
        pFactory->Release();
    }

    // 如果找到了独立NVIDIA或AMD显卡，优先返回
    if (!dedicatedGPU.empty()) {
        return dedicatedGPU;
    }

    // 如果找到了任何非集成显卡，返回第一个
    if (!gpuList.empty()) {
        return gpuList[0];
    }

    // 如果没有找到独立显卡，返回"Unknown GPU"
    return L"Unknown GPU";

    // 回退到注册表方法
    std::wstring gpuName = GetRegistryValue(HKEY_LOCAL_MACHINE,
        L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000", L"DriverDesc");

    // 如果在第一个位置找不到，尝试查询0001
    if (gpuName.empty()) {
        gpuName = GetRegistryValue(HKEY_LOCAL_MACHINE,
            L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0001", L"DriverDesc");
    }

    // 如果所有方法都失败，返回通用信息
    if (gpuName.empty()) {
        gpuName = L"Unknown GPU";
    }

    return gpuName;
}

std::wstring SystemInfoCollector::GetMemoryInfo() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {
        ULONGLONG totalMB = memInfo.ullTotalPhys / (1024 * 1024);
        ULONGLONG usedMB = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024);

        return std::to_wstring(usedMB) + L" MB / " + std::to_wstring(totalMB) + L" MB";
    }

    return L"Unknown Memory";
}

std::wstring SystemInfoCollector::GetDiskInfo() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        double totalGB = static_cast<double>(totalBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
        double freeGB = static_cast<double>(totalFreeBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);

        std::wostringstream oss;
        oss << std::fixed << std::setprecision(2);
        oss << L"C:\\ " << totalGB << L" GB (" << freeGB << L" GB free)";

        return oss.str();
    }

    return L"Unknown Disk";
}

double SystemInfoCollector::GetMemoryUsagePercent() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {
        return static_cast<double>(memInfo.dwMemoryLoad);
    }

    return 0.0;
}

double SystemInfoCollector::GetDiskUsagePercent() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        double usedBytes = static_cast<double>(totalBytes.QuadPart - totalFreeBytes.QuadPart);
        double totalBytesD = static_cast<double>(totalBytes.QuadPart);

        return (usedBytes / totalBytesD) * 100.0;
    }

    return 0.0;
}

// 私有辅助方法实现
std::wstring SystemInfoCollector::GetRegistryValue(HKEY hKey, const std::wstring& subKey, const std::wstring& valueName) {
    HKEY hSubKey;
    if (RegOpenKeyExW(hKey, subKey.c_str(), 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) {
        return L"";
    }

    wchar_t data[1024];
    DWORD dataSize = sizeof(data);
    DWORD dataType;

    if (RegQueryValueExW(hSubKey, valueName.c_str(), NULL, &dataType, (LPBYTE)data, &dataSize) == ERROR_SUCCESS) {
        if (dataType == REG_SZ) {
            RegCloseKey(hSubKey);
            return std::wstring(data);
        }
    }

    RegCloseKey(hSubKey);
    return L"";
}

std::wstring SystemInfoCollector::FormatBytes(ULONGLONG bytes) {
    const wchar_t* units[] = {L"B", L"KB", L"MB", L"GB", L"TB"};
    int unitIndex = 0;
    double size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        unitIndex++;
    }

    std::wostringstream oss;
    oss << std::fixed << std::setprecision(2) << size << L" " << units[unitIndex];
    return oss.str();
}

std::wstring SystemInfoCollector::FormatUptime(ULONGLONG milliseconds) {
    ULONGLONG seconds = milliseconds / 1000;
    ULONGLONG minutes = seconds / 60;
    ULONGLONG hours = minutes / 60;
    ULONGLONG days = hours / 24;

    hours %= 24;
    minutes %= 60;

    std::wostringstream oss;

    // 始终显示完整的天/小时/分钟格式
    oss << days << L" days, " << hours << L" hours, " << minutes << L" minutes";

    return oss.str();
}
