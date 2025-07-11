cmake_minimum_required(VERSION 3.10)

# 项目名称和版本
project(Yumefetch VERSION 1.0.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 静态链接设置 - 这是关键！
if(WIN32)
    # 强制静态链接所有运行时库
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -static-libgcc -static-libstdc++ -static")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -static-libgcc -static-libstdc++ -static")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# 设置编译器标志
if(MSVC)
    # Visual Studio编译器设置
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
    add_definitions(-DUNICODE -D_UNICODE)
else()
    # GCC/Clang编译器设置
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
endif()

# 源文件
set(SOURCES
    src/main.cpp
    src/system_info.cpp
)

# 创建可执行文件
add_executable(Yumefetch ${SOURCES})

# 包含头文件目录
target_include_directories(Yumefetch
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

# 链接Windows系统库
if(WIN32)
    target_link_libraries(Yumefetch
        -static
        user32
        kernel32
        advapi32
        version
        psapi
        pdh
        ntdll
        dxgi
    )
endif()

# 设置可执行文件输出目录
set_target_properties(Yumefetch PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin
)

# 安装配置
install(TARGETS Yumefetch
    RUNTIME DESTINATION bin
)
