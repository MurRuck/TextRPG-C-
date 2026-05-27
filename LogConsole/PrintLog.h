#pragma once
#include <iostream>
#include <string>
#include <Windows.h>

class PrintLog
{
public:
    void SetColor(WORD color);
    void PrintLogLine(const std::string& line);
};
