#include "system_info.hpp"
#include <head.hpp>


#ifdef __GNUC__
extern "C" {
const GUID IID_IDXGIFactory = {
    0x7b7166ec, 0x21c7, 0x44ae, {0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69}
};
}
#endif

SystemInfoCollector::SystemInfoCollector() {}

SystemInfoCollector::~SystemInfoCollector() {}

SystemInfo SystemInfoCollector::GetSystemInfo() {
    SystemInfo info;
    info.osName             = GetOSName();
    info.osVersion          = GetOSVersion();
    info.cpuModel           = GetCPUModel();
    info.userName           = GetUserName();
    info.computerName       = GetComputerName();
    info.uptime             = GetUptime();
    info.resolution         = GetResolution();
    info.gpuModel           = GetGPUModel();
    info.memoryInfo         = GetMemoryInfo();
    info.diskInfo           = GetDiskInfo();
    info.memoryUsagePercent = GetMemoryUsagePercent();
    info.diskUsagePercent   = GetDiskUsagePercent();
    return info;
}

std::wstring SystemInfoCollector::GetOSName() {
    std::wstring build = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

    if (!build.empty()) {
        int buildNumber = _wtoi(build.c_str());

        if (buildNumber >= 22000) return L"Windows 11";
    }

    std::wstring osName = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ProductName");

    if (osName.empty()) osName = L"Windows";

    return osName;
}

std::wstring SystemInfoCollector::GetOSVersion() {
    std::wstring build = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

    std::wstring displayVersion = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"DisplayVersion");

    std::wstring ubr =
        GetRegistryValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"UBR");

    if (!build.empty()) {
        int buildNumber = _wtoi(build.c_str());

        if (buildNumber >= 22000) {
            if (!displayVersion.empty())
                if (displayVersion.find(L"Dev") != std::wstring::npos || buildNumber > 26000)
                    return L"Dev (" + build + L")";
                else return displayVersion + L" (" + build + L")";
            else if (buildNumber > 26000) return L"Dev (" + build + L")";
            else return L"21H2 (" + build + L")"; // Windows 11 默认版本
        } else if (buildNumber >= 10240 && buildNumber < 22000) {
            if (!displayVersion.empty()) return displayVersion;
            else if (buildNumber >= 19041) return L"2004";
            else if (buildNumber >= 18363) return L"1909";
            else if (buildNumber >= 18362) return L"1903";
            else if (buildNumber >= 17763) return L"1809";
            else if (buildNumber >= 17134) return L"1803";
            else if (buildNumber >= 16299) return L"1709";
            else if (buildNumber >= 15063) return L"1703";
            else if (buildNumber >= 14393) return L"1607";
            else if (buildNumber >= 10586) return L"1511";
            else return L"1507";
        } else {
            return L"Dev (" + build + L")";
        }
    }

    return L"Unknown Build";
}

std::wstring SystemInfoCollector::GetCPUModel() {
    std::wstring cpuName =
        GetRegistryValue(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                         L"ProcessorNameString");

    if (cpuName.empty()) cpuName = L"Unknown CPU";

    return cpuName;
}

std::wstring SystemInfoCollector::GetUserName() {
    wchar_t username[256];
    DWORD   size = sizeof(username) / sizeof(username[0]);

    if (GetUserNameW(username, &size)) return std::wstring(username);

    return L"Unknown User";
}

std::wstring SystemInfoCollector::GetComputerName() {
    wchar_t computerName[256];
    DWORD   size = sizeof(computerName) / sizeof(computerName[0]);

    if (GetComputerNameW(computerName, &size)) return std::wstring(computerName);

    return L"Unknown Computer";
}

std::wstring SystemInfoCollector::GetUptime() {
    ULONGLONG uptime = GetTickCount64();
    return FormatUptime(uptime);
}

std::wstring SystemInfoCollector::GetResolution() {
    std::wstring result = L"";

    HDC hdc = GetDC(NULL);
    if (hdc) {
        int screenWidth  = GetDeviceCaps(hdc, HORZRES);
        int screenHeight = GetDeviceCaps(hdc, VERTRES);

        int physicalWidth  = GetDeviceCaps(hdc, DESKTOPHORZRES);
        int physicalHeight = GetDeviceCaps(hdc, DESKTOPVERTRES);

        ReleaseDC(NULL, hdc);

        if (physicalWidth > 0 && physicalHeight > 0
            && (physicalWidth != screenWidth || physicalHeight != screenHeight)) {
            screenWidth  = physicalWidth;
            screenHeight = physicalHeight;
        }

        result = std::to_wstring(screenWidth) + L"x" + std::to_wstring(screenHeight);
    } else {
        int screenWidth  = GetSystemMetrics(SM_CXSCREEN);
        int screenHeight = GetSystemMetrics(SM_CYSCREEN);
        result           = std::to_wstring(screenWidth) + L"x" + std::to_wstring(screenHeight);
    }

    DEVMODEW devMode;
    ZeroMemory(&devMode, sizeof(devMode));
    devMode.dmSize = sizeof(devMode);

    if (EnumDisplaySettingsW(NULL, ENUM_CURRENT_SETTINGS, &devMode)) {
        if (devMode.dmDisplayFrequency > 0 && devMode.dmDisplayFrequency != 1)
            result += L" @" + std::to_wstring(devMode.dmDisplayFrequency) + L"Hz";
    }

    return result;
}

std::wstring SystemInfoCollector::GetGPUModel() {
    IDXGIFactory*             pFactory = nullptr;
    std::vector<std::wstring> gpuList;
    std::wstring              dedicatedGPU = L"";

    HRESULT hr = CreateDXGIFactory(IID_IDXGIFactory, (void**)&pFactory);

    if (SUCCEEDED(hr)) {
        IDXGIAdapter* pAdapter = nullptr;
        for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC adapterDesc;
            if (SUCCEEDED(pAdapter->GetDesc(&adapterDesc))) {
                std::wstring gpuName = adapterDesc.Description;

                std::wstring upperName = gpuName;
                std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

                bool isIntegrated =
                    upperName.find(L"INTEL") != std::wstring::npos
                    || upperName.find(L"UHD GRAPHICS") != std::wstring::npos
                    || upperName.find(L"HD GRAPHICS") != std::wstring::npos
                    || upperName.find(L"IRIS") != std::wstring::npos
                    || (upperName.find(L"RADEON") != std::wstring::npos
                        && upperName.find(L"GRAPHICS") != std::wstring::npos)
                    || upperName.find(L"780M") != std::wstring::npos; // 特别排除Radeon 780M集成显卡

                if (!isIntegrated) {
                    gpuList.push_back(gpuName);

                    if (upperName.find(L"NVIDIA") != std::wstring::npos
                        || (upperName.find(L"AMD") != std::wstring::npos
                            && upperName.find(L"RADEON") != std::wstring::npos
                            && upperName.find(L"780M") == std::wstring::npos)) { // 排除AMD集成显卡
                        dedicatedGPU = gpuName;
                    }
                }
            }
            pAdapter->Release();
        }
        pFactory->Release();
    }

    if (!dedicatedGPU.empty()) return dedicatedGPU;

    if (!gpuList.empty()) return gpuList[0];

    return L"Unknown GPU";
}

std::wstring SystemInfoCollector::GetMemoryInfo() {
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {
        ULONGLONG totalMB = memInfo.ullTotalPhys / (1024 * 1024);
        ULONGLONG usedMB  = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024);

        return std::to_wstring(usedMB) + L" MB / " + std::to_wstring(totalMB) + L" MB";
    }

    return L"Unknown Memory";
}

std::wstring SystemInfoCollector::GetDiskInfo() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        double totalGB = static_cast<double>(totalBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
        double freeGB  = static_cast<double>(totalFreeBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);

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

    if (GlobalMemoryStatusEx(&memInfo)) return static_cast<double>(memInfo.dwMemoryLoad);

    return 0.0;
}

double SystemInfoCollector::GetDiskUsagePercent() {
    ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;

    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        double usedBytes   = static_cast<double>(totalBytes.QuadPart - totalFreeBytes.QuadPart);
        double totalBytesD = static_cast<double>(totalBytes.QuadPart);

        return (usedBytes / totalBytesD) * 100.0;
    }

    return 0.0;
}

std::wstring SystemInfoCollector::GetRegistryValue(HKEY hKey, std::wstring const& subKey,
                                                   std::wstring const& valueName) {
    HKEY hSubKey;
    if (RegOpenKeyExW(hKey, subKey.c_str(), 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) return L"";

    wchar_t data[1024];
    DWORD   dataSize = sizeof(data);
    DWORD   dataType;

    if (RegQueryValueExW(hSubKey, valueName.c_str(), NULL, &dataType, (LPBYTE)data, &dataSize)
        == ERROR_SUCCESS) {
        if (dataType == REG_SZ) {
            RegCloseKey(hSubKey);
            return std::wstring(data);
        }
    }

    RegCloseKey(hSubKey);
    return L"";
}

std::wstring SystemInfoCollector::FormatBytes(ULONGLONG bytes) {
    wchar_t const* units[]   = {L"B", L"KB", L"MB", L"GB", L"TB"};
    int            unitIndex = 0;
    double         size      = static_cast<double>(bytes);

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
    ULONGLONG hours   = minutes / 60;
    ULONGLONG days    = hours / 24;

    hours %= 24;
    minutes %= 60;

    std::wostringstream oss;

    oss << days << L" days, " << hours << L" hours, " << minutes << L" minutes";

    return oss.str();
}
