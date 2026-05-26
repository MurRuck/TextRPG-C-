#pragma once
#include <vector>
#include <memory>
#include <functional>
#include "../../GameManager/Gamemode/Gamemode.h"
#include "../../Item/Item.h"

// ────────────────────────────────────────────
//  상점 재고 항목
//  아이템 생성 함수와 수량을 묶어서 관리
//  가격은 아이템의 getPrice()로 직접 가져옴
// ────────────────────────────────────────────
struct ShopEntry
{
    std::function<std::unique_ptr<Item>()> factory; // 아이템 생성 함수
    int                                    stock;   // 남은 수량 (최대 3)
};

class Shop
{
public:
    Shop();

    // 전투 후 상점 입장 여부 질문
    // y → SetPlayerState(PlayerState::Shop)
    // n → 상태 변경 없음
    static void AskEnterShop(Gamemode& gamemode);

    // 상점 구매 루프 - 입장 후 호출
    void OpenShop(Gamemode& gamemode);

private:
    std::vector<ShopEntry> stock_; // 재고 목록 (아이템별 최대 3개)

    void PrintStock(const PlayerCharacter* player) const;
    void BuyItem(int index, Gamemode& gamemode);
};
