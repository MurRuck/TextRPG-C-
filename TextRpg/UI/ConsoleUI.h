#pragma once
#include <string>
#include <vector>

class ConsoleUI
{
public:
    static void Clear();
    static void Pause();

    static void PrintLine(int width = 40);
    static void PrintTitle(const std::string& title, int width = 40);
    static void PrintBox(const std::vector<std::string>& lines, int width = 40);
    static void PrintMenu(const std::vector<std::string>& menuItems, int width = 40);

private:
    static std::string CenterText(const std::string& text, int width);
};