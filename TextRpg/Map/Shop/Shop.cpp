#include "Shop.h"

#include <iostream>
#include <iomanip>
#include <limits>

#include "../../Item/HealthPotion.h"
#include "../../Item/AttackBoost.h"

static void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ────────────────────────────────────────────
//  생성자: 재고 등록
// ────────────────────────────────────────────
Shop::Shop()
{
    stock_.push_back({ []() { return std::make_unique<HealthPotion>(); }, 5 });
    stock_.push_back({ []() { return std::make_unique<AttackBoost>();  }, 5 });
}

// ────────────────────────────────────────────
//  전투 후 상점 입장 여부 질문
// ────────────────────────────────────────────
void Shop::AskEnterShop(Gamemode& gamemode)
{
    char choice;
    while (true)
    {
        std::cout << "Would you like to enter the shop? (y/n): ";
        std::cin >> choice;
        ClearInput();

        if (choice == 'y' || choice == 'Y')
        {
            gamemode.SetPlayerState(PlayerState::Shop);
            return;
        }
        if (choice == 'n' || choice == 'N')
        {
            return;
        }
        std::cout << "Please enter y or n.\n";
    }
}

// ────────────────────────────────────────────
//  재고 목록 출력
// ────────────────────────────────────────────
void Shop::PrintStock(const PlayerCharacter* player) const
{
   
    std::cout << "*  S H O P  *\n";
   
    if (player)
        std::cout << "  Gold: " << player->GetGold() << "G\n";
    
    for (int i = 0; i < (int)stock_.size(); ++i)
    {
        auto temp = stock_[i].factory();
        std::cout << "  " << (i + 1) << ". "
            << std::left << std::setw(16) << temp->getName()
            << std::setw(6) << (std::to_string(temp->getPrice()) + "G");

        if (stock_[i].stock > 0)
            std::cout <<  stock_[i].stock << "/5\n";
        else
            std::cout << "  SOLD OUT\n";
    }
    
    std::cout << "  0. Leave Shop\n";
    std::cout << "\n  Choice: ";
}

// ────────────────────────────────────────────
//  구매 처리
// ────────────────────────────────────────────
void Shop::BuyItem(int index, Gamemode& gamemode)
{
    PlayerCharacter* player = gamemode.GetMutablePlayer();
    if (!player) return;

    ShopEntry& entry = stock_[index];
    auto temp = entry.factory();
    int price = temp->getPrice();
    // 품절 확인
    if (entry.stock <= 0)
    {
        std::cout << "  " << temp->getName() << " is sold out.\n";
        return;
    }
            
    // 골드 부족 확인
    if (player->GetGold() < price)
    {
        std::cout << "  Not enough gold!  \n";
        return;
    }

    // 구매 처리: 골드 차감 + 인벤토리 즉시 추가 + 재고 감소
    const std::string itemName = temp->getName();
    player->BuyItem(price);
    player->AddItem(std::move(temp));
    entry.stock--;

    std::cout << "  [" << itemName << "] purchased! (-" << price << "G)\n";
    std::cout << "  Gold remaining: " << player->GetGold() << "G\n";
}

// ────────────────────────────────────────────
//  상점 구매 루프
//  0 입력 시 Battle 상태로 복귀
// ────────────────────────────────────────────
void Shop::OpenShop(Gamemode& gamemode)
{
    while (true)
    {
        PrintStock(&gamemode.GetPlayerCharacter());

        int input;
        std::cin >> input;
        ClearInput();

        if (input == 0)
        {
            gamemode.SetPlayerState(PlayerState::Battle);
            std::cout << "  Leaving the shop. Safe travels!\n";
            return;
        }

        int index = input - 1; // 1-based → 0-based
        if (index < 0 || index >= (int)stock_.size())
        {
            std::cout << "  Invalid number.\n";
            continue;
        }

        BuyItem(index, gamemode);
    }
}
