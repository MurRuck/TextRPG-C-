#pragma once

#include <string>

#include "Item.h"

class PlayerCharacter;

//5~15 사이 랜덤 공격력증가
class ChaosAttackBoost : public Item {
private:
	std::string name = "ChaosAttackBoost";
	int attackIncrease = 0;
	int price = 7;
public:
	ChaosAttackBoost();

	std::string getName() const override;
	int getPrice() const override;

	void use(PlayerCharacter* playerCharacter);
};