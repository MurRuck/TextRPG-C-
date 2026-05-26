#include "Shop.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include "../../Logging/LoggingManager.h"
#include "../../Entity/Item/HealthPotion.h"
#include "../../Entity/Item/AttackBoost.h"



static void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int CalcSellPrice(int basePrice)
{
    return static_cast<int>(basePrice * 0.6f);
}

// ────────────────────────────────────────────
//  생성자: 재고 등록
// ────────────────────────────────────────────
Shop::Shop()
{
    logManager.Initialize();

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
        std::cout << "  Gold: " << player->gold << "G\n";
    
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
    std::cout << "  9. Sell Item\n";
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
    if (player->gold < price)
    {
        std::cout << "  Not enough gold!  \n";
        return;
    }

    // 구매 처리: 골드 차감 + 인벤토리 즉시 추가 + 재고 감소
    std::string boughtName = temp->getName();

    player->gold -= price;
    player->inventory.push_back(std::move(temp));
    entry.stock--;

    logManager.Add(LogHeader::Info, "BuyItem:", boughtName + " (-" + std::to_string(price) + "G)");
   
    std::cout << "  [" << boughtName << "] purchased! (-" << price << "G)\n";
    std::cout << "  Gold remaining: " << player->gold << "G\n";
}

// ────────────────────────────────────────────
//  판매 목록 출력
//-───────────────────────────────────────────
void Shop::PrintInventoryForSell(const PlayerCharacter* player) const
{
    std::cout << "\n*  S E L L  *\n";
    if (!player)
    {
        std::cout << "  No player.\n";
        return;
    }

    std::cout << "  Gold: " << player->gold << "G\n";

    if (player->inventory.empty())
    {
        std::cout << "  Inventory is empty.\n";
        std::cout << "  0. Back\n";
        std::cout << "  Choice: ";
        return;
    }

    for (int i = 0; i < (int)player->inventory.size(); ++i)
    {
        const auto& item = player->inventory[i];
        int sellPrice = CalcSellPrice(item->getPrice());
        std::cout << "  " << (i + 1) << ". "
            << std::left << std::setw(16) << item->getName()
            << " Sell: " << sellPrice << "G (Base: " << item->getPrice() << "G)\n";
    }

    std::cout << "  0. Back\n";
    std::cout << "  Choice: ";
}

// ────────────────────────────────────────────
//  판매 처리
//-───────────────────────────────────────────
void Shop::SellItem(int invIndex, Gamemode& gamemode)
{
    PlayerCharacter* player = gamemode.GetMutablePlayer();
    if (!player) return;

    if (invIndex < 0 || invIndex >= (int)player->inventory.size())
    {
        std::cout << "  Invalid number.\n";
        return;
    }

    auto& item = player->inventory[invIndex];
    int basePrice = item->getPrice();
    int sellPrice = CalcSellPrice(basePrice);

    std::string name = item->getName();

    // 골드 지급
    player->gold += sellPrice;

    // 인벤토리에서 제거 (unique_ptr 파괴)
    player->inventory.erase(player->inventory.begin() + invIndex);

    logManager.Add(LogHeader::Info, "SellItem:", name + " (+" + std::to_string(sellPrice) + "G)");
    
    std::cout << "  [" << name << "] sold! (+" << sellPrice << "G)\n";
    std::cout << "  Gold remaining: " << player->gold << "G\n";
}

// ────────────────────────────────────────────
//  판매 루프
//  0 입력 시 상점 메뉴로 복귀
//-───────────────────────────────────────────
void Shop::OpenSellMenu(Gamemode& gamemode)
{
    while (true)
    {
        PlayerCharacter* player = gamemode.GetMutablePlayer();
        PrintInventoryForSell(player);

        int input;
        if (!(std::cin >> input))
        {
            ClearInput();
            std::cout << "  Please enter a number.\n";
            continue;
        }
        ClearInput();

        if (input == 0)
            return;

        int invIndex = input - 1;
        SellItem(invIndex, gamemode);
    }
}

// ────────────────────────────────────────────
//  상점 구매 루프
//  0 입력 시 Battle 상태로 복귀
// ────────────────────────────────────────────
void Shop::OpenShop(Gamemode& gamemode)
{
    while (true)
    {
        PrintStock(gamemode.GetPlayerCharacter());

        int input;
        if (!(std::cin >> input))
        {
            ClearInput();
            std::cout << "  Please enter a number.\n";
            continue;
        }
        ClearInput();

        if (input == 0)
        {
            gamemode.SetPlayerState(PlayerState::Battle);
            std::cout << "  Leaving the shop. Safe travels!\n";
            return;
        }

        if (input == 9)
        {
            OpenSellMenu(gamemode);
            continue;
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