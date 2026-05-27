#include "GameStart.h"
#include <iostream>
#include <string>
#include <windows.h>    
#include <ctime>

#include "GameManager/Gamemode/Gamemode.h"
#include "GameManager/LoggingManager/LoggingManager.h"
#include "Map/Battle/Battle.h"
#include "Entity/Character/PlayerCharacter.h"





void GameStart()
{
    srand((unsigned int)time(nullptr));

    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    Gamemode Gamemode;
    LoggingManager log;

    log.Add(LogHeader::Warning, "테스트 : ", std::string("테스트1"));
    log.Add(LogHeader::Warning, "테스트 : ", std::string("테스트2"));
    std::cout << "start" << std::endl;

    BattleLoop battle;
    PlayerCharacter tempPlayer("TestPlayer");
    battle.StartBattle(&tempPlayer);


}
