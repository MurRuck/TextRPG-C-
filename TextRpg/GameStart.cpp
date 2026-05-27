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
    std::cout << "start" << std::endl;

    BattleLoop battle(Gamemode);
    PlayerCharacter tempPlayer("TestPlayer");
    battle.StartBattle(&tempPlayer);


}
