#include "Here.hpp"
#include "Config.hpp"
#include <print>
#include <string>
#include <vector>

void printHelp() {
    const auto helpText = {
        "Yume - System Info Display",
        "",
        "Usage:",
        "  Yume                    Auto mode (read image_path from config.yaml)",
        "  Yume --image <path>      Use custom image path",
        "",
        "Options:",
        "  --set-image <path>      Set image path in config.yaml",
        "  --version               Show version info",
        "  --help, -h              Show this help"
    };
    for (const auto& line : helpText) {
        std::println("{}", line);
    }
}

void printVersion() {
    std::print("Yume Version: 1.0.2\nAuthor: YumeYuka\nCompiler: ");
#if defined(_MSC_VER)
    std::println("MSVC");
#elif defined(__MINGW32__)
    std::println("MinGW");
#elif defined(__clang__)
    std::println("Clang");
#else
    std::println("unknown");
#endif
    std::println("Build time: {} {}", __DATE__, __TIME__);
}

int main(const int argc, char* argv[]) {
    try {if (argc == 1) {
            const auto imagePathOpt = Config::getImagePath();
            if (imagePathOpt.has_value()) {
                Here::PrintNeofetch(imagePathOpt->c_str(), 300);
            } else {
                Here::PrintNeofetchASCII();
            }
            return 0;
        }

        const std::string arg1 = argv[1];

        if (arg1 == "--set-image" && argc == 3) {
            Config::setImagePath(argv[2]);
            return 0;
        }

        if (arg1 == "--version") {
            printVersion();
            return 0;
        }

        if (arg1 == "--help" || arg1 == "-h") {
            printHelp();
            return 0;
        }

        if (arg1 == "--image" && argc == 3) {
            Here::PrintNeofetch(argv[2], 300);
            return 0;
        }

        std::println(stderr, "Unknown option: {}", arg1);
        std::println(stderr, "Use --help for usage information.");
        return 1;

    } catch (const std::exception& e) {
        std::println(stderr, "Error: {}", e.what());
        return 1;
    }
}
