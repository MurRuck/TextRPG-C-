#include "Shop.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <windows.h>
#include <mmsystem.h>

#include "../../Entity/Character/PlayerCharacter.h"
#include "../../GameManager/Gamemode/Gamemode.h"
#include "../../GameManager/LoggingManager/LoggingManager.h"
#include "../../Item/AttackBoost.h"
#include "../../Item/HealthPotion.h"
#include "../../UI/AsciiArt.h"
#include "../../UI/ConsoleUI.h"

#pragma comment(lib, "winmm.lib")

namespace
{
    void ClearInput()
    {
        std::cin.clear();
    }

    void WaitEnter()
    {
        std::cout << "\n계속하려면 Enter를 누르세요...";
        std::cin.get();
    }

    bool Confirm(const std::string& message)
    {
        while (true)
        {
            std::cout << message << " (y/n): ";

            char choice;
            if (!(std::cin >> choice))
            {
                ClearInput();
                continue;
            }
            ClearInput();

            if (choice == 'y' || choice == 'Y')
            {
                return true;
            }

            if (choice == 'n' || choice == 'N')
            {
                return false;
            }

            std::cout << "y 또는 n을 입력해주세요.\n";
        }
    }

    void PlayPurchaseSound()
    {
        const char* soundPaths[] = {
            "SpekyJoayo.WAV",
            "spekyjoayo.wav",
            "..\\..\\SpekyJoayo.WAV",
            "..\\..\\spekyjoayo.wav",
            "TextRpg\\SpekyJoayo.WAV",
            "TextRpg\\spekyjoayo.wav"
        };

        for (const char* path : soundPaths)
        {
            if (PlaySoundA(path, nullptr, SND_FILENAME | SND_ASYNC | SND_NODEFAULT))
            {
                return;
            }
        }
    }
}

Shop::Shop(Gamemode* gamemode_ptr) : gamemode(gamemode_ptr)
{
    stock_.push_back({ []() { return std::make_unique<HealthPotion>(); }, 5 });
    stock_.push_back({ []() { return std::make_unique<AttackBoost>(); }, 5 });
}



void Shop::ShopMain(const PlayerCharacter* player) const
{
    ConsoleUI::Clear();
    AsciiArt::Print(ArtType::Shop);
    ConsoleUI::PrintTitle("상점");

    if (player != nullptr)
    {
        std::cout << "보유 골드: " << player->GetGold() << "G\n\n";
    }

    std::cout << "1. 아이템 사기\n";
    std::cout << "2. 아이템 팔기\n";
    std::cout << "0. 나가기\n";
    std::cout << "\n선택 > ";
}

void Shop::BuyItemInShop(int index)
{
    PlayerCharacter* player = gamemode->GetMutablePlayer();
    ConsoleUI::Clear();
    if (player == nullptr)
    {
        return;
    }

    if (index < 0 || index >= static_cast<int>(stock_.size()))
    {
        std::cout << "없는 아이템 번호입니다.\n";
        return;
    }

    ShopEntry& entry = stock_[index];
    auto item = entry.factory();
    const int price = item->getPrice();
    const std::string itemName = item->getName();

    if (entry.stock <= 0)
    {
        std::cout << itemName << "은(는) 품절입니다.\n";
        return;
    }

    if (player->GetGold() < price)
    {
        AsciiArt::Print(ArtType::NotEnoughGold);
        std::cout << "골드가 부족합니다. 필요 골드: " << price << "G\n";
        return;
    }

    player->BuyItem(price, std::move(item));
    entry.stock--;

    AsciiArt::Print(ArtType::PurchaseSuccess);
    PlayPurchaseSound();
    
}

void Shop::PrintInventoryForSell(const PlayerCharacter* player) const
{
    if (player == nullptr)
    {
        return;
    }

    std::cout << "보유 골드: " << player->GetGold() << "G\n\n";
    std::cout << "인벤토리 목록\n";
    player->ShowInventory();
    std::cout << "\n판매할 인벤토리 번호를 입력하세요. (-1: 뒤로가기)\n";
    std::cout << "선택 > ";
}

void Shop::SellItemInShop(int invIndex)
{
    
    PlayerCharacter* player = gamemode->GetMutablePlayer();
    if (player == nullptr)
    {
        return;
    }

    player->SellItem(invIndex);
    
    auto item = stock_[invIndex].factory();
    const int price = item->getPrice();
    const std::string itemName = item->getName();
    
    
    
}

void Shop::OpenSellMenu()
{
    PlayerCharacter* player = gamemode->GetMutablePlayer();
    if (player == nullptr)
    {
        return;
    }

    while (true)
    {
        ConsoleUI::Clear();
        AsciiArt::Print(ArtType::Shop);
        ConsoleUI::PrintTitle("아이템 팔기");
        PrintInventoryForSell(player);

        int input;
        if (!(std::cin >> input))
        {
            ClearInput();
            std::cout << "숫자를 입력해주세요.\n";
            WaitEnter();
            continue;
        }
        ClearInput();

        if (input == -1)
        {
            return;
        }

        if (!Confirm("선택한 아이템을 판매하시겠습니까?"))
        {
            return;
        }

        SellItemInShop(input);
        WaitEnter();
    }
}

void Shop::OpenBuyMenu()
{
    PlayerCharacter* player = gamemode->GetMutablePlayer();
    if (player == nullptr)
    {
        return;
    }

    while (true)
    {
        ConsoleUI::Clear();
        AsciiArt::Print(ArtType::Shop);
        ConsoleUI::PrintTitle("아이템 사기");

        std::cout << "보유 골드: " << player->GetGold() << "G\n\n";
        for (int i = 0; i < static_cast<int>(stock_.size()); ++i)
        {
            auto item = stock_[i].factory();
            std::cout << i + 1 << ". "
                << std::left << std::setw(16) << item->getName()
                << std::right << std::setw(5) << item->getPrice() << "G"
                << " / 재고: " << stock_[i].stock << "\n";
        }

        std::cout << "0. 뒤로가기\n\n";
        std::cout << "구매할 아이템 번호 입력 > ";

        int input;
        if (!(std::cin >> input))
        {
            ClearInput();
            std::cout << "숫자를 입력해주세요.\n";
            WaitEnter();
            continue;
        }
        ClearInput();

        if (input == 0)
        {
            return;
        }

        const int index = input - 1;
        if (index < 0 || index >= static_cast<int>(stock_.size()))
        {
            std::cout << "없는 아이템 번호입니다.\n";
            WaitEnter();
            continue;
        }

        auto item = stock_[index].factory();
        const std::string itemName = item->getName();
        const int price = item->getPrice();

        if (!Confirm(itemName + "을(를) " + std::to_string(price) + "G에 구매하시겠습니까?"))
        {
            return;
        }

        BuyItemInShop(index);
        WaitEnter();
    }
}

void Shop::OpenShop()
{
    while (true)
    {
        ShopMain(gamemode->GetMutablePlayer());

        int input;
        if (!(std::cin >> input))
        {
            ClearInput();
            std::cout << "숫자를 입력해주세요.\n";
            WaitEnter();
            continue;
        }
        ClearInput();

        if (input == 1)
        {
            OpenBuyMenu();
        }
        else if (input == 2)
        {
            OpenSellMenu();
        }
        else if (input == 0)
        {
            std::cout << "상점에서 나갑니다.\n";
            WaitEnter();
            return;
        }
        else
        {
            std::cout << "없는 메뉴입니다.\n";
            WaitEnter();
        }
    }
}
