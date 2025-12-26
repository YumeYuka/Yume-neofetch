cmake_minimum_required(VERSION 4.1)
project(YumeFetch LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(YumeFetch
        src/main.cpp
        src/system_info.cpp
        src/Config.cpp
)

target_include_directories(YumeFetch PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_compile_options(YumeFetch PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
)


target_link_libraries(YumeFetch
        PRIVATE
        ${CMAKE_SOURCE_DIR}/lib/libsixel.a
        ${CMAKE_SOURCE_DIR}/lib/libpng.a
        ${CMAKE_SOURCE_DIR}/lib/libjpeg.a
        ${CMAKE_SOURCE_DIR}/lib/libz.a
        dxgi
        dxguid
)


if (CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(YumeFetch PRIVATE -O3 -march=native)
endif()


if (MINGW)
    target_link_libraries(YumeFetch PRIVATE
            m
    )
    target_link_options(YumeFetch PRIVATE
            -static-libstdc++
            -static-libgcc
    )
endif()
