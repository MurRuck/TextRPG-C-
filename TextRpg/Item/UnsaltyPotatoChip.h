#pragma once

#include <string>

#include "Item.h"

class PlayerCharacter;

//쓸모없는 아이템 = 짜지않은감자칩
class UnsaltyPotatoChip : public Item{
private:
	std::string name = "UnsaltyPotatoChip";
	int price = 100;
	int healthRestore = 1;
	int attackIncrease = 1;
public:
	UnsaltyPotatoChip() {}

	std::string getName() const override;
	int getPrice() const override;

	void use(PlayerCharacter* playerCharacter) override;
};