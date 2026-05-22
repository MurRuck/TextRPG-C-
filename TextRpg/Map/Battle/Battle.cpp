#include "Battle.h"
#include <iostream>

// =========================
// 임시 테스트용 구조 (나중에 삭제)
// =========================
class Player {
public:
    int hp;
    int atk;
};

class Monster {
public:
    int hp;
    int atk;
};

// =========================
// 전투 시작
// =========================
void BattleLoop::StartBattle(Player* player)
{
    this->player = player;

    CreateMonster();

    std::cout << "전투 시작!\n";

    while (true)
    {
        PlayerTurn();

        if (CheckBattleEnd())
            break;

        MonsterTurn();

        if (CheckBattleEnd())
            break;
    }

    GiveReward();
}

// =========================
// 몬스터 생성
// =========================
void BattleLoop::CreateMonster()
{
    monster = new Monster();

    monster->hp = 50;
    monster->atk = 10;

    std::cout << "몬스터 등장! HP: " << monster->hp << "\n";
}

// =========================
// 플레이어 턴
// =========================
void BattleLoop::PlayerTurn()
{
    std::cout << "플레이어 공격!\n";

    monster->hp -= player->atk;

    std::cout << "몬스터 HP: " << monster->hp << "\n";
}

// =========================
// 몬스터 턴
// =========================
void BattleLoop::MonsterTurn()
{
    std::cout << "몬스터 공격!\n";

    player->hp -= monster->atk;

    std::cout << "플레이어 HP: " << player->hp << "\n";
}

// =========================
// 종료 체크
// =========================
bool BattleLoop::CheckBattleEnd()
{
    if (monster == nullptr || player == nullptr)
        return true;

    if (monster->hp <= 0)
    {
        std::cout << "승리!\n";
        return true;
    }

    if (player->hp <= 0)
    {
        std::cout << "패배...\n";
        return true;
    }

    return false;
}

// =========================
// 보상 + 메모리 정리
// =========================
void BattleLoop::GiveReward()
{
    if (player && player->hp > 0)
    {
        std::cout << "보상 획득!\n";
    }

    delete monster;
    monster = nullptr;
}