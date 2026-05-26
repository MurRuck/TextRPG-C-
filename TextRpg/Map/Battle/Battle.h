#pragma once
#include "../../Entity/Character/PlayerCharacter.h"
#include "../../Entity/Enemy/EnemyBase.h"
#include "../../GameManager/LoggingManager/LoggingManager.h"
#include <memory>
#include <string>

class BattleLoop
{
public:
    void StartBattle(PlayerCharacter* player);

private:
    void CreateMonster();
    void PlayerTurn();
    void MonsterTurn();
    void Attack();
    void UseBuff();
    bool CheckBattleEnd() const;
    void GiveReward();
    bool HasItem() const; // 포션 체크용 미완성
    void DisplayUI(const std::string& log) const;  // UI 

private:
    PlayerCharacter* player = nullptr;
    std::unique_ptr<EnemyBase> monster;
    int buffTurn = 0;
    LoggingManager logger;
    std::string lastLog;  
};