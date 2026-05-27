#include "PrintLog.h"


void  SetColor(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void PrintLogLine(const std::string& line)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(console, &info);
    WORD color = info.wAttributes;
    
    std::string header;
    WORD headerColor = color;
    
    if (line.rfind("[Error]",0) == 0)
    {
        header = "[Error]";
        headerColor = FOREGROUND_RED | FOREGROUND_INTENSITY;
    }
    else if (line.rfind("[Warning]",0) == 0)
    {
        header = "[Warning]";
        headerColor = FOREGROUND_RED | FOREGROUND_GREEN |FOREGROUND_INTENSITY;
    }
    else if (line.rfind("[Info]",0) == 0)
    {
        header = "[Info]";
        headerColor = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
    }
    
    if (!header.empty())
    {
        SetColor(headerColor);
        std::cout << header;
        
        SetColor(color);
        std::cout << line.substr(header.size()) << std::endl;
    }
    else
    {
        std::cout << line << std::endl;
    }
}