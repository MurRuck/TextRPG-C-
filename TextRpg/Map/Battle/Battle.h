#pragma once
#include "../../Entity/Character/PlayerCharacter.h"
#include "../../Entity/Enemy/EnemyBase.h"
#include "../../GameManager/LoggingManager/LoggingManager.h"
#include <memory>
#include <string>
#include "../../GameManager/Gamemode/Gamemode.h"

enum class PlayerState;

class BattleLoop
{
public:
    void StartBattle(PlayerCharacter* player);
    PlayerState HandleBattleEnd();

private:
    void CreateMonster();
    void PlayerTurn();
    void MonsterTurn();
    void Attack();
    void UseBuff();
    bool CheckBattleEnd();
    void GiveReward();
    bool HasItem() const; 
    void DisplayUI() const;  
    void ClearLog();
    void ShowEndMenu();
    


private:
    PlayerCharacter* player = nullptr;
    std::unique_ptr<EnemyBase> monster;
    int buffTurn = 0;
    LoggingManager logger;

   
    void PushLog(const std::string& newLog);

    std::string lastLog1;
    std::string lastLog2;
    std::string lastLog3;
};