#include "Battle.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include "../../GameManager/Gamemode/Gamemode.h"


using namespace std;

// =========================
// UI 출력
// =========================
void BattleLoop::DisplayUI() const
{
	
    system("cls");

    

    cout << "==============================================\n";
    cout << "           TEXT RPG BATTLE\n";
    cout << "==============================================\n";

    // =========================
    // HEADER
    // =========================
    cout << " [ " << player->GetName() << " ]"
        << "                     "
        << "[ " << (monster ? monster->GetName() : "None") << " ]\n";
    cout << " [Lv. " << player->GetLevel() << "]" << " [EXP:" << player->GetEXP() << " / " << player->GetRequiredEXP() << "]\n";
       
       

    cout << "----------------------------------------------\n";
	cout << "[보유골드: " << player->GetGold() << "G] \n";
	cout << "==============================================\n";
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
    // LOG
    // =========================
    cout << " LOG\n";
    cout << "----------------------------------------------\n";

    cout << " > " << lastLog1 << "\n";
    cout << " > " << lastLog2 << "\n";
    cout << " > " << lastLog3 << "\n";

    cout << "==============================================\n";


    Sleep(700);
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

    logger.Add(LogHeader::Info, "Battle Start", "Player entered battle");

    CreateMonster();

    while (true)
    {
        PlayerTurn();
        if (CheckBattleEnd()) break;

        MonsterTurn();
        if (CheckBattleEnd()) break;
    }

    HandleBattleEnd();
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

    logger.Add(LogHeader::Info,
        "Monster Spawn",
        monster->GetName());
}

// =========================
// 인벤토리 체크
// =========================
bool BattleLoop::HasItem() const
{
    return true;
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

            logger.Add(LogHeader::Warning, "Buff End", "Attack buff removed");
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

        PushLog("포션 사용!");
        DisplayUI();

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

    PushLog(monster->GetName() + " 공격! "
        + to_string(damage) + " 데미지");

    DisplayUI();

    logger.Add(LogHeader::Warning, "Monster Attack", damage);
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


        logger.Add(LogHeader::Info, "Reward", "EXP & Gold granted");
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

    logger.Add(LogHeader::Info, "Player Attack", damage);
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

    logger.Add(LogHeader::Warning, "Buff Used", "Attack +10 for 3 turns");
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
    if (player->IsDead())
    {
        PushLog("패배...");
        DisplayUI();

        return PlayerState::Create; 
    }

    PushLog("승리!");
    DisplayUI();

    GiveReward();
    ShowEndMenu();

    int input;
    cin >> input;

    switch (input)
    {
    case 1:
        ClearLog();
        StartBattle(player);
        return PlayerState::Battle;

    case 2:
        return PlayerState::Shop;

    case 3:
        return PlayerState::Create; // 

    case 4:
        return PlayerState::Shop; // 임시 fallback

    default:
        return PlayerState::Battle;
    }
}