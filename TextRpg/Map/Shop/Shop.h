#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../../Item/Item.h"

class LoggingManager;
class Gamemode;

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
    Shop(Gamemode* gamemode_ptr = nullptr);
        
    
    void OpenShop();

private:
    Gamemode* gamemode = nullptr;
    
    std::vector<ShopEntry> stock_; // 재고 목록 
    void ShopMain(const PlayerCharacter* player) const;
    void BuyItemInShop(int index);
    void PrintInventoryForSell(const PlayerCharacter* player) const;
    void SellItemInShop(int invIndex);
    void OpenSellMenu();
    void OpenBuyMenu();
};

