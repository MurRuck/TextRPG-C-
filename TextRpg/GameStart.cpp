#include "GameStart.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <windows.h>

#include "GameManager/Gamemode/Gamemode.h"
#include "GameManager/LoggingManager/LoggingManager.h"
#include "Map/Battle/Battle.h"
#include "Map/Shop/Shop.h"
#include "UI/AsciiArt.h"
#include "UI/ConsoleUI.h"

void SetupConsole()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
}

namespace
{
    std::string ReadInput()
    {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    void WaitEnter()
    {
        std::cout << "\n계속하려면 Enter를 누르세요...";
        ReadInput();
    }

    bool HasPlayer(Gamemode& gameMode)
    {
        return gameMode.GetMutablePlayer() != nullptr;
    }

    void ShowMainMenu()
    {
        ConsoleUI::PrintMenu({
            "새 게임",
            "던전 입장",
            "캐릭터 상세",
            "스킬 목록",
            "상점가기",
            "게임 종료"
        });
        std::cout << "선택 > ";
    }

    void CreatePlayer(Gamemode& gameMode)
    {
        ConsoleUI::Clear();
        ConsoleUI::PrintTitle("캐릭터 생성");

        std::string name;
        do
        {
            std::cout << "캐릭터 닉네임 입력 > ";
            name = ReadInput();
        } while (name.empty());

        auto player = std::make_unique<PlayerCharacter>(name, &gameMode);
        gameMode.SetPlayerCharceter(std::move(player));
        gameMode.SetPlayerState(PlayerState::Create);
        gameMode.GetLogger()->Add(LogHeader::Info, "Player Created: ", name);

        ConsoleUI::Clear();
        AsciiArt::Print(ArtType::Player);
        gameMode.GetPlayerCharacter().ShowStatus();
        std::cout << "\n캐릭터 생성 완료! 메인 메뉴로 돌아갑니다." << std::endl;
        WaitEnter();
    }

    void EnterBattle(Gamemode& gameMode)
    {
        PlayerCharacter* player = gameMode.GetMutablePlayer();
        if (player == nullptr)
        {
            ConsoleUI::PrintTitle("던전 입장");
            std::cout << "생성된 캐릭터가 없습니다. 새 게임을 먼저 시작해주세요." << std::endl;
            WaitEnter();
            return;
        }

        gameMode.SetPlayerState(PlayerState::Battle);

        BattleLoop battle(gameMode);
        battle.StartBattle(player);
        battle.HandleBattleEnd();

        if (player->IsDead())
        {
            gameMode.SetPlayerCharceter(std::unique_ptr<PlayerCharacter>());
            gameMode.SetPlayerState(PlayerState::Create);

            ConsoleUI::PrintTitle("게임 오버");
            std::cout << "캐릭터가 사망했습니다." << std::endl;
            std::cout << "이름, 레벨, 골드, 아이템이 모두 초기화됩니다." << std::endl;
            std::cout << "새 게임으로 다시 시작해주세요." << std::endl;
            WaitEnter();
            return;
        }

        gameMode.SetPlayerState(PlayerState::Create);

        ConsoleUI::PrintTitle("전투 종료");
        std::cout << "전투에서 승리했습니다." << std::endl;
        WaitEnter();
    }

    void EnterShop(Gamemode& gameMode)
    {
        if (!HasPlayer(gameMode))
        {
            ConsoleUI::Clear();
            ConsoleUI::PrintTitle("상점가기");
            std::cout << "생성된 캐릭터가 없습니다. 새 게임을 먼저 시작해주세요." << std::endl;
            WaitEnter();
            return;
        }

        gameMode.SetPlayerState(PlayerState::Shop);

        Shop shop(&gameMode);
        shop.OpenShop();

        gameMode.SetPlayerState(PlayerState::Create);
    }

    void ShowPlayerStatus(Gamemode& gameMode)
    {
        ConsoleUI::Clear();
        if (HasPlayer(gameMode))
        {
            AsciiArt::Print(ArtType::Player);
            gameMode.GetPlayerCharacter().ShowStatus();
        }
        else
        {
            ConsoleUI::PrintTitle("캐릭터 상세");
            std::cout << "생성된 캐릭터가 없습니다. 새 게임을 먼저 시작해주세요." << std::endl;
        }

        WaitEnter();
    }

    void ShowPlayerSkills(Gamemode& gameMode)
    {
        ConsoleUI::Clear();
        if (HasPlayer(gameMode))
        {
            gameMode.GetPlayerCharacter().ShowSkills();
        }
        else
        {
            ConsoleUI::PrintTitle("스킬 목록");
            std::cout << "생성된 캐릭터가 없습니다. 새 게임을 먼저 시작해주세요." << std::endl;
        }

        WaitEnter();
    }
}

void GameStart()
{
    SetupConsole();

    Gamemode gameMode;
    LoggingManager log;

    bool isRunning = true;
    while (isRunning)
    {
        ConsoleUI::Clear();
        AsciiArt::Print(ArtType::Title);
        if (HasPlayer(gameMode))
        {
            AsciiArt::Print(ArtType::Player);
        }

        ShowMainMenu();
        const std::string input = ReadInput();

        if (input == "1" || input == "새 게임")
        {
            CreatePlayer(gameMode);
        }
        else if (input == "2" || input == "던전 입장")
        {
            EnterBattle(gameMode);
        }
        else if (input == "3" || input == "캐릭터 상세")
        {
            ShowPlayerStatus(gameMode);
        }
        else if (input == "4" || input == "스킬 목록")
        {
            ShowPlayerSkills(gameMode);
        }
        else if (input == "5" || input == "상점가기")
        {
            EnterShop(gameMode);
        }
        else if (input == "6" || input == "게임 종료")
        {
            ConsoleUI::PrintTitle("게임 종료");
            isRunning = false;
        }
    }
}
