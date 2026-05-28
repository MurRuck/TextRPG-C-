#include "ConsoleUI.h"

#include <iostream>
#include <limits>

void ConsoleUI::Clear()
{
    system("cls");
}

void ConsoleUI::Pause()
{
    std::cout << "\n계속하려면 Enter를 누르세요...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void ConsoleUI::PrintLine(int width)
{
    std::cout << std::string(width, '=') << std::endl;
}

void ConsoleUI::PrintTitle(const std::string& title, int width)
{
    PrintLine(width);
    std::cout << CenterText(title, width) << std::endl;
    PrintLine(width);
}

void ConsoleUI::PrintBox(const std::vector<std::string>& lines, int width)
{
    const int innerWidth = width - 2;

    std::cout << "+" << std::string(innerWidth, '-') << "+" << std::endl;
    for (const std::string& line : lines)
    {
        std::string text = line;
        if (static_cast<int>(text.size()) > innerWidth - 2)
        {
            text = text.substr(0, innerWidth - 2);
        }

        std::cout << "| " << text
            << std::string(innerWidth - 1 - text.size(), ' ')
            << "|" << std::endl;
    }
    std::cout << "+" << std::string(innerWidth, '-') << "+" << std::endl;
}

void ConsoleUI::PrintMenu(const std::vector<std::string>& menuItems, int width)
{
    std::vector<std::string> lines;
    for (size_t i = 0; i < menuItems.size(); i++)
    {
        lines.push_back(std::to_string(i + 1) + ". " + menuItems[i]);
    }

    PrintBox(lines, width);
}

std::string ConsoleUI::CenterText(const std::string& text, int width)
{
    if (static_cast<int>(text.size()) >= width)
        return text;

    const int leftPadding = (width - static_cast<int>(text.size())) / 2;
    return std::string(leftPadding, ' ') + text;
}
