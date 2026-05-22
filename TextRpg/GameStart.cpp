#include "GameStart.h"
#include <iostream>
#include <string>

#include "GameManager/Gamemode/Gamemode.h"
#include "GameManager/LoggingManager/LoggingManager.h"

void GameStart()
{
    Gamemode Gamemode;
    LoggingManager log;
    
    log.Add(LogHeader::Warning, "테스트 : ", std::string("테스트1"));
    log.Add(LogHeader::Warning, "테스트 : ", std::string("테스트2"));
    std::cout << "start" << std::endl;
}
