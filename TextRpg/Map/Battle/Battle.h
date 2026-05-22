#pragma once

class Player;
class Monster;


// 전투 루프 클래스

class BattleLoop
{
public:
    void StartBattle(Player* player);
private:
    void CreateMonster();

    void PlayerTurn();
    void MonsterTurn();

    bool CheckBattleEnd();

    void GiveReward();

private:
    Player* player = nullptr;
    Monster* monster = nullptr;
};