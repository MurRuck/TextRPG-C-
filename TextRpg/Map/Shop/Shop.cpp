#include "Shop.h"

#include <iostream>
#include <iomanip>
#include <limits>
#include "../../Entity/Character/PlayerCharacter.h"
#include "../../GameManager/LoggingManager/LoggingManager.h"
#include "../../Item/HealthPotion.h"
#include "../../Item/AttackBoost.h"


static void ClearInput()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int CalcSellPrice(int basePrice)
{
	return static_cast<int>(basePrice * 0.6f); //원가60%로 판매
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
			ShopMain(&gamemode.GetPlayerCharacter());
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
//  상점 창 출력
// ────────────────────────────────────────────
void Shop::ShopMain(const PlayerCharacter* player) const
{
	std::cout << "=====================================\n";
    std::cout << "            *  S H O P  *            \n";
    std::cout << "=====================================\n";
   
    std::cout << "  1. Buy Item\n";
    std::cout << "  2. Sell Item\n";
    std::cout << "  3. Leave Shop\n";
    std::cout << "\n  Choice: ";
}

// ────────────────────────────────────────────
//  구매 처리
// ────────────────────────────────────────────
void Shop::BuyItemInShop(int index, Gamemode& gamemode)
{
    PlayerCharacter* player = gamemode.GetMutablePlayer();
    if (!player) return;

    ShopEntry& entry = stock_[index];
    auto itemPtr = entry.factory();         // 구매할 아이템 생성
    int price = itemPtr->getPrice();
    std::string itemName = itemPtr->getName();

    // 현재 골드/재고 리스트 출력
    std::cout << "  Gold: " << player->GetGold() << "G\n";
    for (int i = 0; i < (int)stock_.size(); ++i)
    {
        auto tmp = stock_[i].factory();
        std::cout << "  " << (i + 1) << ". "
            << std::left << std::setw(16) << tmp->getName()
            << std::setw(6) << (std::to_string(tmp->getPrice()) + "G");

        if (stock_[i].stock > 0) std::cout << stock_[i].stock << "/5\n";
        else                     std::cout << "  SOLD OUT\n";
    }

    // 품절 체크
    if (entry.stock <= 0)
    {
        std::cout << "  " << itemName << " is sold out.\n";
        return;
    }

    // 골드 부족 체크 (player->GetGold()는 public getter)
    if (player->GetGold() < price)
    {
        std::cout << "  Not enough gold!\n";
        return;
    }

    player->BuyItem(price, std::move(itemPtr));   
    entry.stock--;

    logManager.Add(LogHeader::Info, "BuyItem:", itemName + " (-" + std::to_string(price) + "G)");

    std::cout << "  [" << itemName << "] purchased! (-" << price << "G)\n";
    std::cout << "  Gold remaining: " << player->GetGold() << "G\n";
}

// ────────────────────────────────────────────
//  판매 목록 출력
//-───────────────────────────────────────────
void Shop::PrintInventoryForSell(const PlayerCharacter* player) const
{
    std::cout << "  Gold: " << player->GetGold() << "G\n";
    player->ShowInventory();
            
    std::cout << "  0. Back\n";
    std::cout << "  Choice: ";
}

// ────────────────────────────────────────────
//  판매 처리
//-───────────────────────────────────────────
void Shop::SellItemInShop(int invIndex, Gamemode& gamemode)
{
    PlayerCharacter* player = gamemode.GetMutablePlayer();
    if (!player) return;
      
    player->SellItem(invIndex);

    std::cout << "  Gold remaining: " << player->GetGold() << "G\n";

}

// ────────────────────────────────────────────
//  판매 루프
//  0 입력 시 상점 메뉴로 복귀
//-───────────────────────────────────────────
void Shop::OpenSellMenu(Gamemode& gamemode)
{
    while (true)
    {
        std::cout << "=====================================\n";
        std::cout << "            *  S E L L  *            \n";
        std::cout << "=====================================\n";
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
        SellItemInShop(invIndex, gamemode);
    }
}

// ────────────────────────────────────────────
//  구매 루프
// -───────────────────────────────────────────
void Shop::OpenBuyMenu(Gamemode& gamemode)
{
    while (true)
    {
        std::cout << "=====================================\n";
        std::cout << "            *  S H O P  *            \n";
        std::cout << "=====================================\n";
        std::cout << "1. HealthPotion: Restores 50 HP. (50G)\n";
        std::cout << "2. AttackBoost: Increases attack by 10 for the next battle. (10G)\n";
        std::cout << "0. Back\n";
        PlayerCharacter* player = gamemode.GetMutablePlayer();
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
        int index = input - 1;
        if (index < 0 || index >= (int)stock_.size())
        {
            std::cout << "  Invalid number.\n";
            continue;
        }
        BuyItemInShop(index, gamemode);
	}

}


// ────────────────────────────────────────────
//  상점  루프
//  3 입력 시 Battle 상태로 복귀
// ────────────────────────────────────────────
void Shop::OpenShop(Gamemode& gamemode)
{
    while (true)
    {
        ShopMain(&gamemode.GetPlayerCharacter());

        int input;
        if (!(std::cin >> input))
        {
            ClearInput();
            std::cout << "  Please enter a number.\n";
            continue;
        }
        ClearInput();
        
        if (input == 1)
        {
            OpenBuyMenu(gamemode); //구매 화면
            continue;
        }

        if (input == 2)
        {
            OpenSellMenu(gamemode); //판매 화면
            continue;
        }

        if (input == 3) // 상점 나가기
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

       
    }
}
