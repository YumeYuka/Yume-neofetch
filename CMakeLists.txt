cmake_minimum_required(VERSION 3.15)

project(Yumefetch VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(YumefetchCore OBJECT
        src/system_info.cpp
        src/Config.cpp
)

target_sources(YumefetchCore PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
        include/ascii_art.hpp
        include/color_theme.hpp
        include/Config.hpp
        include/head.hpp
        include/system_info.hpp
        include/Utils.hpp
)

add_executable(Yumefetch src/main.cpp)

target_include_directories(YumefetchCore INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Yumefetch PRIVATE YumefetchCore)

if(WIN32)
    target_link_libraries(Yumefetch PRIVATE
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

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose build type" FORCE)
endif()

if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/O1 /GL /DNDEBUG /MT")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /OPT:REF /OPT:ICF")
    target_compile_definitions(Yumefetch PRIVATE UNICODE _UNICODE)
else()
    set(CMAKE_CXX_FLAGS_RELEASE "-Os -ffunction-sections -fdata-sections -DNDEBUG")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-static -static-libgcc -static-libstdc++ -Wl,--gc-sections -s")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set_target_properties(Yumefetch PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}"
        OUTPUT_NAME "Yumefetch"
)

install(TARGETS Yumefetch
        RUNTIME DESTINATION bin
        COMPONENT Runtime
)

if(EXISTS ${CMAKE_SOURCE_DIR}/config.yaml)
    install(FILES ${CMAKE_SOURCE_DIR}/config.yaml
            DESTINATION bin
            COMPONENT Runtime
    )
endif()