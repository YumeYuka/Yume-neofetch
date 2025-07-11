//
// Created by YumeYuka on 2025/7/11.
//

#ifndef HEAD_HPP
#define HEAD_HPP

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <dxgi.h>
#include <ntstatus.h>
#include <pdh.h>
#include <pdhmsg.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <windows.h>
#include <winternl.h>

// 移除pragma注释，改为在CMakeLists.txt中处理库链接
// MinGW编译器不支持这些pragma注释，会产生警告

#include "ascii_art.hpp"
#include "system_info.hpp"

#endif // HEAD_HPP
