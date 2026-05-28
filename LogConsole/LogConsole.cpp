 // LogConsole.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <Windows.h>
#include <conio.h>
#include <iostream>
#include <string>

void  SetColor(WORD color)
{
    // 터미널 글자색 변경 이게 진짜 터꾸지
    // GetStdHandle = 현재 출력되는 터미널
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void PrintLogLine(const std::string& line)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // 현재 터미널의 화면 버퍼 정보 struct
    // info = 버퍼크기, 커서 위치, 현재 글자/배경 색상, 창 영역/크기 가지고있음
    CONSOLE_SCREEN_BUFFER_INFO info{};
    GetConsoleScreenBufferInfo(console, &info);
    
    // wAttributes = 색상 
    WORD color = info.wAttributes;
    
    std::string header;
    WORD headerColor = color;
    
    // find = index 앞부터 찾기
    // rfind = reverse -> 뒤에서부터 찾기
    // offset이 0이라 사실상 문자열 시작이 [Error]인지 검사 근데 앞에 다른 문자가 붙을 수 있음
    // 그래서 뒤에서부터 탐색
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
    else if (line.rfind("[System]",0) == 0)
    {
        header = "[System]";
        headerColor = FOREGROUND_RED | FOREGROUND_GREEN |FOREGROUND_INTENSITY;
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

int main()
{
    std::cout << "========== TextRPG Logger ==========" << std::endl;
    
    std::string line;
    while (std::getline(std::cin, line))
    {
        PrintLogLine(line);
    }

    std::cout << "========== Logger disconnected ==========" << std::endl;
    std::cout << "Press any key to close..." << std::endl;
    
    // 키보드에서 아무거나 입력받기 전까지 대기
    _getch();
     
     return 0;
}
