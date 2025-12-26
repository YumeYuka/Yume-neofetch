#include "system_info.hpp"
#include <format>
#include <array>

#ifdef __GNUC__
extern "C" {
const GUID IID_IDXGIFactory = {
    0x7b7166ec, 0x21c7, 0x44ae, {0xb2, 0x1a, 0xc9, 0xae, 0x32, 0x1a, 0xe3, 0x69}
};
}
#endif

#include <dxgi.h>

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
    const auto build = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

    if (!build.empty()) {
        const auto buildNumber = _wtoi(build.c_str());
        if (buildNumber >= 22000) return L"Windows 11";
    }

    auto osName = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"ProductName");

    if (osName.empty()) osName = L"Windows";
    return osName;
}

std::wstring SystemInfoCollector::GetOSVersion() {
    const auto build = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CurrentBuild");

    const auto displayVersion = GetRegistryValue(
        HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"DisplayVersion");

    const auto ubr =
        GetRegistryValue(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"UBR");

    if (!build.empty()) {
        const auto buildNumber = _wtoi(build.c_str());

        if (buildNumber >= 22000) {
            if (!displayVersion.empty()) {
                if (displayVersion.contains(L"Dev") || buildNumber > 26000)
                    return L"Dev (" + build + L")";
                return displayVersion + L" (" + build + L")";
            }
            if (buildNumber > 26000) return L"Dev (" + build + L")";
            return L"21H2 (" + build + L")";
        }

        if (buildNumber >= 10240 && buildNumber < 22000) {
            if (!displayVersion.empty()) return displayVersion;
            if (buildNumber >= 19041) return L"2004";
            if (buildNumber >= 18363) return L"1909";
            if (buildNumber >= 18362) return L"1903";
            if (buildNumber >= 17763) return L"1809";
            if (buildNumber >= 17134) return L"1803";
            if (buildNumber >= 16299) return L"1709";
            if (buildNumber >= 15063) return L"1703";
            if (buildNumber >= 14393) return L"1607";
            if (buildNumber >= 10586) return L"1511";
            return L"1507";
        }
        return L"Dev (" + build + L")";
    }

    return L"Unknown Build";
}

std::wstring SystemInfoCollector::GetCPUModel() {
    auto cpuName =
        GetRegistryValue(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
                         L"ProcessorNameString");

    if (cpuName.empty()) cpuName = L"Unknown CPU";
    return cpuName;
}

std::wstring SystemInfoCollector::GetUserName() {
    wchar_t username[256]{};
    DWORD size = sizeof(username) / sizeof(username[0]);

    if (GetUserNameW(username, &size)) return std::wstring(username);
    return L"Unknown User";
}

std::wstring SystemInfoCollector::GetComputerName() {
    wchar_t computerName[256]{};
    DWORD size = sizeof(computerName) / sizeof(computerName[0]);

    if (GetComputerNameW(computerName, &size)) return {computerName};
    return L"Unknown Computer";
}

std::wstring SystemInfoCollector::GetUptime() {
    const auto uptime = GetTickCount64();
    return FormatUptime(uptime);
}

std::wstring SystemInfoCollector::GetResolution() {
    const auto hdc = GetDC(nullptr);
    if (!hdc) {
        const auto screenWidth = GetSystemMetrics(SM_CXSCREEN);
        const auto screenHeight = GetSystemMetrics(SM_CYSCREEN);
        return std::format(L"{}x{}", screenWidth, screenHeight);
    }

    const auto screenWidth = GetDeviceCaps(hdc, HORZRES);
    const auto screenHeight = GetDeviceCaps(hdc, VERTRES);

    const auto physicalWidth = GetDeviceCaps(hdc, DESKTOPHORZRES);
    const auto physicalHeight = GetDeviceCaps(hdc, DESKTOPVERTRES);

    ReleaseDC(nullptr, hdc);

    auto finalWidth = screenWidth;
    auto finalHeight = screenHeight;

    if (physicalWidth > 0 && physicalHeight > 0
        && (physicalWidth != screenWidth || physicalHeight != screenHeight)) {
        finalWidth = physicalWidth;
        finalHeight = physicalHeight;
    }

    auto result = std::format(L"{}x{}", finalWidth, finalHeight);

    DEVMODEW devMode{};
    devMode.dmSize = sizeof(devMode);

    if (EnumDisplaySettingsW(nullptr, ENUM_CURRENT_SETTINGS, &devMode)) {
        if (devMode.dmDisplayFrequency > 0 && devMode.dmDisplayFrequency != 1)
            result += std::format(L" @ {}Hz", devMode.dmDisplayFrequency);
    }

    return result;
}

std::wstring SystemInfoCollector::GetGPUModel() {
    IDXGIFactory* pFactory = nullptr;
    std::vector<std::wstring> gpuList;
    std::wstring dedicatedGPU;

    const auto hr = CreateDXGIFactory(IID_IDXGIFactory, reinterpret_cast<void**>(&pFactory));

    if (SUCCEEDED(hr)) {
        IDXGIAdapter* pAdapter = nullptr;
        for (UINT i = 0; pFactory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC adapterDesc{};
            if (SUCCEEDED(pAdapter->GetDesc(&adapterDesc))) {
                const auto gpuName = std::wstring(adapterDesc.Description);

                auto upperName = gpuName;
                std::transform(upperName.begin(), upperName.end(), upperName.begin(), ::toupper);

                const auto isIntegrated =
                    upperName.contains(L"INTEL")
                    || upperName.contains(L"UHD GRAPHICS")
                    || upperName.contains(L"HD GRAPHICS")
                    || upperName.contains(L"IRIS")
                    || (upperName.contains(L"RADEON") && upperName.contains(L"GRAPHICS"))
                    || upperName.contains(L"780M");

                if (!isIntegrated) {
                    gpuList.push_back(gpuName);

                    if (upperName.contains(L"NVIDIA")
                        || (upperName.contains(L"AMD")
                            && upperName.contains(L"RADEON")
                            && !upperName.contains(L"780M"))) {
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
    MEMORYSTATUSEX memInfo{};
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) {
        const auto totalMB = memInfo.ullTotalPhys / (1024 * 1024);
        const auto usedMB = (memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024);
        return std::format(L"{} MB / {} MB", usedMB, totalMB);
    }

    return L"Unknown Memory";
}

std::wstring SystemInfoCollector::GetDiskInfo() {
    ULARGE_INTEGER freeBytesAvailable{}, totalBytes{}, totalFreeBytes{};

    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        const auto totalGB = static_cast<double>(totalBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
        const auto freeGB = static_cast<double>(totalFreeBytes.QuadPart) / (1024.0 * 1024.0 * 1024.0);
        return std::format(L"C:\\ {:.2f} GB ({:.2f} GB free)", totalGB, freeGB);
    }

    return L"Unknown Disk";
}

double SystemInfoCollector::GetMemoryUsagePercent() {
    MEMORYSTATUSEX memInfo{};
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);

    if (GlobalMemoryStatusEx(&memInfo)) return static_cast<double>(memInfo.dwMemoryLoad);
    return 0.0;
}

double SystemInfoCollector::GetDiskUsagePercent() {
    ULARGE_INTEGER freeBytesAvailable{}, totalBytes{}, totalFreeBytes{};

    if (GetDiskFreeSpaceExW(L"C:\\", &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
        const auto usedBytes = static_cast<double>(totalBytes.QuadPart - totalFreeBytes.QuadPart);
        const auto totalBytesD = static_cast<double>(totalBytes.QuadPart);
        return (usedBytes / totalBytesD) * 100.0;
    }

    return 0.0;
}

std::wstring SystemInfoCollector::GetRegistryValue(HKEY hKey, std::wstring_view subKey,
                                                   std::wstring_view valueName) {
    const std::wstring subKeyStr(subKey);
    const std::wstring valueNameStr(valueName);

    HKEY hSubKey;
    if (RegOpenKeyExW(hKey, subKeyStr.c_str(), 0, KEY_READ, &hSubKey) != ERROR_SUCCESS) return L"";

    wchar_t data[1024]{};
    DWORD dataSize = sizeof(data);
    DWORD dataType{};

    if (RegQueryValueExW(hSubKey, valueNameStr.c_str(), nullptr, &dataType, reinterpret_cast<LPBYTE>(data), &dataSize)
        == ERROR_SUCCESS) {
        if (dataType == REG_SZ) {
            RegCloseKey(hSubKey);
            return {data};
        }
    }

    RegCloseKey(hSubKey);
    return L"";
}

std::wstring SystemInfoCollector::FormatBytes(const ULONGLONG bytes) {
    constexpr std::array units = {L"B", L"KB", L"MB", L"GB", L"TB"};
    size_t unitIndex = 0;
    auto size = static_cast<double>(bytes);

    while (size >= 1024.0 && unitIndex < 4) {
        size /= 1024.0;
        ++unitIndex;
    }

    return std::format(L"{:.2f} {}", size, units[unitIndex]);
}

std::wstring SystemInfoCollector::FormatUptime(const ULONGLONG milliseconds) {
    const auto seconds = milliseconds / 1000;
    const auto minutes = seconds / 60;
    const auto hours = minutes / 60;
    const auto days = hours / 24;

    return std::format(L"{} days, {} hours, {} minutes", days, hours % 24, minutes % 60);
}
