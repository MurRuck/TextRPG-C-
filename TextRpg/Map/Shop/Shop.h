#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../../GameManager/Gamemode/Gamemode.h"
#include "../../GameManager/LoggingManager/LoggingManager.h"
#include "../../Item/Item.h"

// ────────────────────────────────────────────
//  상점 재고 항목
//  아이템 생성 함수와 수량을 묶어서 관리
//  가격은 아이템의 getPrice()로 직접 가져옴
// ────────────────────────────────────────────
struct ShopEntry
{
    std::function<std::unique_ptr<Item>()> factory; // 아이템 생성 함수
    int stock;   // 남은 수량 
};
    
class Shop
{
public:
    Shop();
        
    void AskEnterShop(Gamemode& gamemode);
    void OpenShop(Gamemode& gamemode);

private:
    LoggingManager logManager;
    std::vector<ShopEntry> stock_; // 재고 목록 
    void ShopMain(const PlayerCharacter* player) const;
    void BuyItemInShop(int index, Gamemode& gamemode);
    void PrintInventoryForSell(const PlayerCharacter* player) const;
    void SellItemInShop(int invIndex, Gamemode& gamemode);
    void OpenSellMenu(Gamemode& gamemode);
	void OpenBuyMenu(Gamemode& gamemode);
};

