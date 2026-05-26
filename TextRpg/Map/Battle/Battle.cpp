#include <iostream>
#include <string>

using namespace std;

int main()
{
    // =========================
    // 더미 데이터
    // =========================
    string playerName = "철수";
    string monsterName = "슬라임";

    int playerHP = 78;
    int playerMaxHP = 100;
    int playerATK = 25;

    int monsterHP = 45;
    int monsterATK = 12;

    int buffTurn = 2;

    // 로그 3줄 더미
    string log1 = "Player attacks 15 damage";
    string log2 = "Monster takes damage";
    string log3 = "Monster attacks 10 damage";

    // =========================
    // UI 출력
    // =========================
    system("cls");

    cout << "==============================================\n";
    cout << "           TEXT RPG BATTLE\n";
    cout << "==============================================\n";

    // =========================
    // HEADER
    // =========================
    cout << " [ " << playerName << " ]"
        << "                     "
        << "[ " << monsterName << " ]\n";

    cout << "----------------------------------------------\n";

    // =========================
    // HP / ATK
    // =========================
    cout << " HP  : " << playerHP << " / " << playerMaxHP
        << "              "
        << "HP  : " << monsterHP << "\n";

    cout << " ATK : " << playerATK
        << "                      "
        << "ATK : " << monsterATK << "\n";

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

    cout << " > " << log1 << "\n";
    cout << " > " << log2 << "\n";
    cout << " > " << log3 << "\n";

    cout << "==============================================\n";

    return 0;
}