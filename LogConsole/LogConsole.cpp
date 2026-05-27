 // LogConsole.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>
#include <string>

 int main()
{
    std::cout << "========== TextRPG Logger ==========" << std::endl;
    
    std::string line;
    while (std::getline(std::cin, line))
    {
        std::cout << line << std::endl;
    }
     
     return 0;
}
