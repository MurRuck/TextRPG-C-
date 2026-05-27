#pragma once
#include "../../Entity/Character/PlayerCharacter.h"
#include "../../Entity/Enemy/EnemyBase.h"
#include "../../GameManager/Gamemode/Gamemode.h"

#include <memory>
#include <string>


class BattleLoop
{
public:
    BattleLoop(Gamemode& gamemode) : gamemode(gamemode) {}
    ~BattleLoop() {}

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
    Gamemode& gamemode;
    PlayerCharacter* player = nullptr;
    std::unique_ptr<EnemyBase> monster;
    int buffTurn = 0;
    

   
    void PushLog(const std::string& newLog);

    std::string lastLog1;
    std::string lastLog2;
    std::string lastLog3;
};