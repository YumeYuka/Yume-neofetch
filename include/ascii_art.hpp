#ifndef ASCII_ART_MERGED_HPP
#define ASCII_ART_MERGED_HPP

#include <head.hpp>

#include "color_theme.hpp"

class AsciiArt {
public:
    AsciiArt() {}

    ~AsciiArt() {}

    std::vector<std::wstring> GetArtByName(std::wstring const& logoName) {
        if (logoName == L"windows" || logoName == L"Windows") {
            return GetWindowsArt();
        } else if (logoName == L"arch" || logoName == L"Arch") {
            return GetArchArt();
        } else if (logoName == L"debian" || logoName == L"Debian") {
            return GetDebianArt();
        } else if (logoName == L"fedora" || logoName == L"Fedora") {
            return GetFedoraArt();
        } else if (logoName == L"macos" || logoName == L"macOS" || logoName == L"MacOS") {
            return GetMacOsArt();
        } else if (logoName == L"ubuntu" || logoName == L"Ubuntu") {
            return GetUbuntuArt();
        } else if (logoName == L"mint" || logoName == L"Mint" || logoName == L"LinuxMint") {
            return GetMintArt();
        } else if (logoName == L"opensuse" || logoName == L"OpenSUSE" || logoName == L"openSUSE") {
            return GetOpenSUSEArt();
        } else if (logoName == L"gentoo" || logoName == L"Gentoo") {
            return GetGentooArt();
        } else {
            return GetWindowsArt();
        }
    }

    std::vector<std::wstring> GetColoredArtByName(std::wstring const& logoName) {
        std::vector<std::wstring> art = GetArtByName(logoName);
        std::vector<std::wstring> coloredArt;
        for (auto const& line : art) coloredArt.push_back(ColorizeAsciiLine(line));
        return coloredArt;
    }

    std::vector<std::wstring> GetOSArt() {
        return GetArtByName(L"windows");
    }

    std::vector<std::wstring> GetColoredOSArt() {
        return GetColoredArtByName(L"windows");
    }

    std::vector<std::wstring> GetAvailableLogos() {
        return {L"windows", L"arch", L"debian",   L"fedora", L"macos",
                L"ubuntu",  L"mint", L"opensuse", L"gentoo"};
    }

    std::vector<std::wstring> GetWindowsArt() {
        std::vector<std::wstring> art = {
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"                                  ",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll", L"llllllllllllllll   llllllllllllllll",
            L"llllllllllllllll   llllllllllllllll"};

        return art;
    }

    std::vector<std::wstring> GetArchArt() {
        std::vector<std::wstring> art = {
            L"                   -`               ",  L"                  .o+`              ",
            L"                 `ooo/              ",  L"                `+oooo:             ",
            L"               `+oooooo:            ",  L"               -+oooooo+:           ",
            L"             `/:-:++oooo+:          ",  L"            `/++++/+++++++:         ",
            L"           `/++++++++++++++:        ",  L"          `/+++ooooooooooooo/       ",
            L"         ./ooosssso++osssssso+      ",  L"        .oossssso-````/ossssss+     ",
            L"       -osssssso.      :ssssssso.   ",  L"      :osssssss/        osssso++.   ",
            L"     /ossssssss/        +ssssooo/   ",  L"   `/ossssso+/:-        -:/+osssso+ ",
            L"  `+sso+:-`                 `.-/+oso",  L" `++:.                           `-/+",
            L" .`                                `/", L"                                    "};
        return art;
    }

    std::vector<std::wstring> GetDebianArt() {
        std::vector<std::wstring> art = {
            L"       _,met$$$$$gg.              ",     L"    ,g$$$$$$$$$$$$$$$P.           ",
            L"  ,g$$P\"     \"\"\"Y$$.\"            ", L" ,$$P'              `$$$.         ",
            L"',$$P       ,ggs.     `$$b:      ",      L"`d$$'     ,$P'   .    $$$        ",
            L" $$P      d$'     ,    $$P       ",      L" $$:      $$.   -    ,d$$'       ",
            L" $$;      Y$P.   .   $$P         ",      L" $$b      `Y$$.   ,$$P'          ",
            L" $Y$$.    `.`Y$$$$P'             ",      L" `.Y$$b.   `-.__                 ",
            L"   `._Y$$b.                      ",      L"      `._Y$$b.                   ",
            L"         `._$$b.                 ",      L"            `$$b.                ",
            L"              `Y$$b.             ",      L"                 `Y$$b.          ",
            L"                    `Y$$b.       ",      L"                       `Y$$b.    "};
        return art;
    }

    std::vector<std::wstring> GetFedoraArt() {
        std::vector<std::wstring> art = {
            L"             ````````             ", L"         `/osssssssso/`           ",
            L"       `+ssssssssssssss+`         ", L"     `/ssssssssssssssssss/`       ",
            L"    `/sssssssssoosssssssss/`      ", L"   -osssssssso`  `/osssssssso-    ",
            L"  -sssssssss/      /sssssssss-    ", L" `ossssssss/        /ssssssssso`  ",
            L" `ossssssss/        /ssssssssso`  ", L" `ossssssss/        /ssssssssso`  ",
            L" `ossssssss/        /ssssssssso`  ", L"  -sssssssss/      /sssssssss-    ",
            L"   -osssssssso`  `/osssssssso-    ", L"    `/sssssssssoosssssssss/`      ",
            L"      -ohdmmmdhso+/:--..`         ", L"           ``````                 ",
            L"                                  ", L"                                  ",
            L"                                  ", L"                                  "};
        return art;
    }

    std::vector<std::wstring> GetMacOsArt() {
        std::vector<std::wstring> art = {
            L"             .:'                  ", L"         __ :'__                 ",
            L"      .'`__`-'__``.              ",  L"     :__________.-'              ",
            L"     :_________:                 ",  L"      :_________:                ",
            L"      .'________`'.              ",  L"     '-----------'               ",
            L"    `-------------`              ",  L"   `--------------`              ",
            L"  `---------------`              ",  L" `----------------`              ",
            L" `----------------`              ",  L" `----------------`              ",
            L" `----------------`              ",  L" `----------------`              ",
            L"  `---------------`              ",  L"   `--------------`              ",
            L"    `-------------`              ",  L"     `-----------`               "};
        return art;
    }

    std::vector<std::wstring> GetUbuntuArt() {
        std::vector<std::wstring> art = {
            L"            .-/+oossssoo+/-.           ",  L"        `:+ssssssssssssssssss+:`       ",
            L"      -+ssssssssssssssssssyyssss+-     ",  L"    .ossssssssssssssssssdMMMNysssso.   ",
            L"   /ssssssssssshdmmNNmmyNMMMMhssssss/  ",  L"  +ssssssssshmydMMMMMMMNddddyssssssss+ ",
            L" /sssssssshNMMMyhhyyyyhmNMMMNhssssssss/",  L".ssssssssdMMMNhsssssssssshNMMMdssssssss.",
            L"+sssshhhyNMMNyssssssssssssyNMMMysssssss+", L"ossyNMMMNyMMhsssssssssssssshmmmhssssssso",
            L"ossyNMMMNyMMhsssssssssssssshmmmhssssssso", L"+sssshhhyNMMNyssssssssssssyNMMMysssssss+",
            L".ssssssssdMMMNhsssssssssshNMMMdssssssss.", L" /sssssssshNMMMyhhyyyyhmNMMMNhssssssss/",
            L"  +ssssssssshmydMMMMMMMNddddyssssssss+ ",  L"   /ssssssssssshdmmNNmmyNMMMMhssssss/  ",
            L"    .ossssssssssssssssssdMMMNysssso.   ",  L"      -+ssssssssssssssssssyyssss+-     ",
            L"        `:+ssssssssssssssssss+:`       ",  L"            .-/+oossssoo+/-.           "};
        return art;
    }

    std::vector<std::wstring> GetMintArt() {
        std::vector<std::wstring> art = {
            L"             _______                ",  L"            /\\      \\              ",
            L"           /  \\      \\             ", L"          / /\\ \\______\\           ",
            L"         / / / /\\_____\\           ",  L"        / / /_/ /\\     \\          ",
            L"       / /_/__\\/ /      \\         ",  L"       \\________/       /          ",
            L"        \\_______\\______/          ",  L"            \\__________/           ",
            L"                                   ",   L"                                   ",
            L"                                   ",   L"                                   ",
            L"                                   ",   L"                                   ",
            L"                                   ",   L"                                   ",
            L"                                   ",   L"                                   "};
        return art;
    }

    std::vector<std::wstring> GetOpenSUSEArt() {
        std::vector<std::wstring> art = {
            L"         .----.                 ",  L"       _/      \\_               ",
            L"      /  o      o\\              ", L"     /     --     \\             ",
            L"    /\\_        _/\\             ", L"   /   \\______/   \\            ",
            L"  /                \\           ",  L" /                  \\          ",
            L"/__________________\\           ",  L"                                ",
            L"                                ",  L"                                ",
            L"                                ",  L"                                ",
            L"                                ",  L"                                ",
            L"                                ",  L"                                ",
            L"                                ",  L"                                "};
        return art;
    }

    std::vector<std::wstring> GetGentooArt() {
        std::vector<std::wstring> art = {
            L"         -/\\                     ", L"        /  |\\                   ",
            L"       /   | \\                  ",  L"      /    |  \\                 ",
            L"     /     |   \\                ",  L"    /      |    \\               ",
            L"   /       |     \\              ",  L"  /        |      \\             ",
            L" /         |       \\            ",  L"/__________|________\\           ",
            L"                                ",   L"                                ",
            L"                                ",   L"                                ",
            L"                                ",   L"                                ",
            L"                                ",   L"                                ",
            L"                                ",   L"                                "};
        return art;
    }

    std::vector<std::wstring> GetColoredArtByOS(std::wstring const& osName) {
        return GetColoredArtByName(osName);
    }

    std::vector<std::wstring> GetArtByOS(std::wstring const& osName) {
        return GetArtByName(osName);
    }

private:
    std::wstring ColorizeAsciiLine(std::wstring const& line) {
        std::string sessionColor = ColorTheme::GetSessionColor();
        std::string resetColor   = ColorTheme::MonetPalette::RESET;

        std::wstring colorPrefix;
        std::wstring colorSuffix;

        int colorLen = MultiByteToWideChar(CP_UTF8, 0, sessionColor.c_str(), -1, nullptr, 0);
        if (colorLen > 0) {
            colorPrefix.resize(colorLen - 1);
            MultiByteToWideChar(CP_UTF8, 0, sessionColor.c_str(), -1, &colorPrefix[0], colorLen);
        }

        int resetLen = MultiByteToWideChar(CP_UTF8, 0, resetColor.c_str(), -1, nullptr, 0);
        if (resetLen > 0) {
            colorSuffix.resize(resetLen - 1);
            MultiByteToWideChar(CP_UTF8, 0, resetColor.c_str(), -1, &colorSuffix[0], resetLen);
        }

        if (line.find_first_not_of(L' ') != std::wstring::npos) return colorPrefix + line + colorSuffix;
        else return line;
    }
};

#endif // ASCII_ART_MERGED_HPP
