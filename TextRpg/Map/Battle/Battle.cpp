#include "Battle.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// =========================
// UI 출력
// =========================
void BattleLoop::DisplayUI(const std::string& log) const
{
    system("cls");

    cout << "==============================================\n";
    cout << "           TEXT RPG BATTLE\n";
    cout << "==============================================\n";

    // =========================
    // HEADER
    // =========================
    //cout << " [ " << player->GetName() << " ]"
    //    << "                     "
    //    << "[ " << (monster ? monster->GetName() : "None") << " ]\n";

    //cout << "----------------------------------------------\n";

    // =========================
    // HP / ATK
    // =========================
    cout << " HP  : " << player->GetHP()
        << " / " << player->GetMaxHP()
        << "              ";

    if (monster)
        cout << "HP  : " << monster->GetHp() << "\n";
    else
        cout << "HP  : -\n";

    cout << " ATK : " << player->GetAttack()
        << "                      ";

    if (monster)
        cout << "ATK : " << monster->GetAttack() << "\n";
    else
        cout << "ATK : -\n";

    cout << "----------------------------------------------\n";

    // =========================
    // BUFF
    // =========================
    cout << " BUFF : ";
    if (buffTurn > 0)
        cout << buffTurn << " turn(s)\n";
    else
        cout << "none\n";

    cout << "==============================================\n";

    // =========================
    // LOG (3줄)
    // =========================
    cout << " LOG\n";
    cout << "----------------------------------------------\n";

    cout << " > " << log << "\n";
    cout << " > " << lastLog2 << "\n";
    cout << " > " << lastLog3 << "\n";

    cout << "==============================================\n";
}
// =========================
// 전투 시작
// =========================
void BattleLoop::StartBattle(PlayerCharacter* player)
{
    this->player = player;

    if (!player)
        return;

    logger.Add(LogHeader::Info, "Battle Start", "Player entered battle");

    CreateMonster();

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

    logger.Add(LogHeader::Info, "Battle End", "Battle finished");
}

// =========================
// 몬스터 생성
// =========================
void BattleLoop::CreateMonster()
{
    int type = rand() % 3;// 임시

    if (type == 0)
    {
        monster = std::make_unique<EnemyBase>("Slime", 1);
    }
    else if (type == 1)
    {
        monster = std::make_unique<EnemyBase>("Goblin", 2);
    }
    else
    {
        monster = std::make_unique<EnemyBase>("Orc", 3);
    }

    DisplayUI("몬스터 등장!");
    logger.Add(LogHeader::Info, "Monster Spawn", monster->GetName());
}

// =========================
// 인벤토리 체크
// =========================
bool BattleLoop::HasItem() const
{
    return true;  //추가예정
}

// =========================
// 플레이어 턴
// =========================
void BattleLoop::PlayerTurn()
{
    float hpRatio = (float)player->GetHP() / player->GetMaxHP();

    if (buffTurn > 0)
    {
        buffTurn--;
        if (buffTurn == 0)
        {
            player->SetAttack(player->GetAttack() - 10);
            DisplayUI("버프 종료!");
            logger.Add(LogHeader::Warning, "Buff End", "Attack buff removed");
        }
    }

    if (hpRatio <= 0.3f)
    {
        if (HasItem())
        {
            player->UseItem(0);
            DisplayUI("HP 포션 사용!");
            logger.Add(LogHeader::Info, "Potion Use", "HP critical - used item");
        }
        else
        {
            Attack();
        }
        return;
    }

    if (hpRatio >= 0.7f)
    {
        if (buffTurn == 0)
            UseBuff();
        else
            Attack();
        return;
    }

    if (rand() % 2 == 0 || !HasItem())
    {
        Attack();
    }
    else
    {
        player->UseItem(0);
        DisplayUI("포션 사용!");
        logger.Add(LogHeader::Info, "Potion Use", "Random action - used potion");
    }
}

// =========================
// 몬스터 턴
// =========================
void BattleLoop::MonsterTurn()
{
    int damage = monster->GetAttack();
    player->TakeDamage(damage);

    DisplayUI(monster->GetName() + " 공격! " + to_string(damage) + " 데미지");
    logger.Add(LogHeader::Warning, "Monster Attack", damage);
}

// =========================
// 전투 종료 체크
// =========================
bool BattleLoop::CheckBattleEnd() const
{
    if (!player || !monster)
        return true;

    if (!monster->IsAlive())
    {
        DisplayUI("승리!");
        return true;
    }

    if (player->IsDead())
    {
        DisplayUI("패배...");
        return true;
    }

    return false;
}

// =========================
// 보상 지급
// =========================
void BattleLoop::GiveReward()
{
    if (!player)
        return;

    if (!player->IsDead())
    {
        player->GainEXP(monster->GetExp());
        player->GainGold(monster->GetGold());
        logger.Add(LogHeader::Info, "Reward", "EXP & Gold granted");
        DisplayUI("보상 획득!");
    }

    monster.reset();
}

// =========================
// 공격
// =========================
void BattleLoop::Attack()
{
    if (!monster)
        return;

    int damage = player->GetAttack();
    monster->TakeDamage(damage);

    DisplayUI("플레이어 공격! " + to_string(damage) + " 데미지");
    logger.Add(LogHeader::Info, "Player Attack", damage);
}

// =========================
// 버프
// =========================
void BattleLoop::UseBuff()
{
    player->SetAttack(player->GetAttack() + 10);
    buffTurn = 3;

    DisplayUI("공격력 버프 사용! ATK +" + to_string(10) + " (3턴)");
    logger.Add(LogHeader::Warning, "Buff Used", "Attack +10 for 3 turns");
}