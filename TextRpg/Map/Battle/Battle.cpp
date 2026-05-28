#include "Battle.h"
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <windows.h>
#include "../../GameManager/Gamemode/Gamemode.h"
#include "../../UI/AsciiArt.h"


using namespace std;

namespace
{
    ArtType GetMonsterArtType(const std::unique_ptr<EnemyBase>& monster)
    {
        if (!monster)
            return ArtType::Slime;

        const std::string name = monster->GetName();
        if (name == "Goblin")
            return ArtType::Goblin;
        if (name == "Orc")
            return ArtType::Orc;
        if (name == "Boss")
            return ArtType::Boss;

        return ArtType::Slime;
    }
}

// =========================
// UI 출력
// =========================
void BattleLoop::DisplayUI() const
{
    const vector<vector<string>>& testFrames = AsciiArt::GetTestAnimationFrames();

    const auto renderFrame = [this](const vector<string>& playerFrame)
    {
        system("cls");

        cout << "==============================================\n";
        cout << "           TEXT RPG BATTLE\n";
        cout << "==============================================\n";
        cout << "PLAYER\n";
        for (const string& line : playerFrame)
        {
            cout << line << "\n";
        }
        cout << "\nENEMY\n";
        AsciiArt::Print(GetMonsterArtType(monster));
        cout << "==============================================\n";

        cout << " [ " << player->GetName() << " ]"
            << "                     "
            << "[ " << (monster ? monster->GetName() : "None") << " ]\n";
        cout << " [Lv. " << player->GetLevel() << "]" << " [EXP:" << player->GetEXP() << " / " << player->GetRequiredEXP() << "]\n";

        cout << "----------------------------------------------\n";
        cout << "[보유골드: " << player->GetGold() << "G] \n";
        cout << "==============================================\n";

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
        cout << " BUFF : ";

        if (buffTurn > 0)
            cout << buffTurn << " turn(s)\n";
        else
            cout << "none\n";

        cout << "==============================================\n";
        cout << " LOG\n";
        cout << "----------------------------------------------\n";
        cout << " > " << lastLog1 << "\n";
        cout << " > " << lastLog2 << "\n";
        cout << " > " << lastLog3 << "\n";
        cout << "==============================================\n";
    };

    for (int repeat = 0; repeat < 3; ++repeat)
    {
        for (const vector<string>& frame : testFrames)
        {
            renderFrame(frame);
            Sleep(180);
        }
    }
}

// =========================
// 로그 추가
// =========================
void BattleLoop::PushLog(const std::string& newLog)
{
    lastLog3 = lastLog2;
    lastLog2 = lastLog1;
    lastLog1 = newLog;
}
// 전투 종료시 전투로그 초기화

void BattleLoop::ClearLog()
{
    lastLog1.clear();
    lastLog2.clear();
    lastLog3.clear();
}

// =========================
// 전투 시작
// =========================
void BattleLoop::StartBattle(PlayerCharacter* player)
{
    this->player = player;

    if (!player)
        return;

    gamemode.GetLogger()->Add(LogHeader::System, "Player ", "entered battle");

    CreateMonster();

    while (true)
    {
        PlayerTurn();
        if (CheckBattleEnd()) break;

        MonsterTurn();
        if (CheckBattleEnd()) break;
    }

}
// =========================
// 몬스터 생성
// =========================
void BattleLoop::CreateMonster()
{
    int type = rand() % 3;

    string name;
    int level = player->GetLevel();   // ⭐ 기본값 반드시 초기화

    if(player->IsMaxLevel())
    {
        type = 4; 
       
	}



    switch (type)
    {
    case 0:
        name = "Slime";
        level += (rand() % 2 - 1);   // -1 ~ 0
        break;

    case 1:
        name = "Goblin";
        level += (rand() % 3 - 1);   // -1 ~ +1
        break;

    case 2:
        name = "Orc";
        level += (rand() % 2 + 1);   // +1 ~ +2
        break;

    case 4:
        name = "Boss";
        level += 10;   // +2 ~ +4
        break;

    }

   
    level = std::max(1, level);

    monster = std::make_unique<EnemyBase>(name, level, 1.0f);

    PushLog(name + " 등장!");
    DisplayUI();

    gamemode.GetLogger()->Add(LogHeader::System,
        "Monster Spawned : ",
        monster->GetName());
}

// =========================
// 인벤토리 체크
// =========================
bool BattleLoop::HasItem() const
{
    return false;
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

            PushLog("버프 종료!");
            DisplayUI();
            gamemode.GetLogger()->Add(LogHeader::Warning, "Player : ", "Attack buff removed");
            gamemode.GetLogger()->Add(LogHeader::System, "Player ", "Buff end");
        }
    }

    if (hpRatio <= 0.3f)
    {
        if (HasItem())
        {
            player->UseItem(0);

            PushLog("HP 포션 사용!");
            Attack();
            DisplayUI();

            gamemode.GetLogger()->Add(LogHeader::System, "Player : ", "Use potion. reason = HP critical - used item");
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

        PushLog("포션 사용!");
        DisplayUI();

        gamemode.GetLogger()->Add(LogHeader::System, "Player : ", "Use potion. reason = Random action - used potion");
    }
}

// =========================
// 몬스터 턴
// =========================
void BattleLoop::MonsterTurn()
{
    int damage = monster->GetAttack();

    player->TakeDamage(damage);

    PushLog(monster->GetName() + " 공격! "
        + to_string(damage) + " 데미지");

    DisplayUI();

    gamemode.GetLogger()->Add(LogHeader::System, "Monster " + monster->GetName() + " : Attack player deal damage ", damage);
}

// =========================
// 전투 종료 체크
// =========================
bool BattleLoop::CheckBattleEnd()
{
    if (!player || !monster)
        return true;

    if (!monster->IsAlive())
        return true;

    if (player->IsDead())
        return true;

    return false;
}

 

// =========================
// 보상 지급
// =========================
void BattleLoop::GiveReward()
{
    if (!player || !monster)
        return;

    if (!player->IsDead())
    {
        monster->OnDeath(player);

        PushLog("보상 획득!");
        DisplayUI();


        gamemode.GetLogger()->Add(LogHeader::System, "Player ", "EXP & Gold granted");
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

    PushLog("플레이어 공격! "
        + to_string(damage) + " 데미지");

    DisplayUI();

    gamemode.GetLogger()->Add(LogHeader::System, "Player : ", "deal damage " + damage);
}

// =========================
// 버프
// =========================
void BattleLoop::UseBuff()
{
    player->SetAttack(player->GetAttack() + 10);

    buffTurn = 3;

    PushLog("공격력 버프 사용! ATK +10 (3턴)");
    DisplayUI();

    gamemode.GetLogger()->Add(LogHeader::System, "Player : ", "Buff Used");
    gamemode.GetLogger()->Add(LogHeader::Warning, "Player : ", "Attack +10 Buff for 3 turns");
}

void BattleLoop::ShowEndMenu()
{
	COORD menupos = { 0,12 };   

    for (int i = 0; i < 6; i++)
    {
        cout << "                                            \n";
    }

    
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), menupos);

    cout << "==============================================\n";
    cout << "다음 행동 선택                                \n";
    cout << "==============================================\n";
    cout << " [1] 전투                                     \n";
    cout << " [2] 상점                                     \n";
    cout << " [3] 인벤토리                                 \n";
    cout << " [4] 게임 종료                                \n" ;
    cout << "==============================================\n";
}
PlayerState BattleLoop::HandleBattleEnd()
{
    if (!player)
        return PlayerState::Create;

    if (player->IsDead())
    {
        PushLog("패배...");
        DisplayUI();
        ClearLog();

        return PlayerState::Create; 
    }

    PushLog("승리!");
    DisplayUI();

    GiveReward();
    ClearLog();

    return PlayerState::Create;
}
