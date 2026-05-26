#pragma once

#include <string>

#include "Item.h"

class PlayerCharacter;

class AttackBoost : public Item {
private:
	std::string name = "AttackBoost";
	int attackIncrease = 10;
	int price = 10;
public:
	AttackBoost() {}

	std::string getName() const override;
	int getPrice() const override;

	void use(PlayerCharacter* playerCharacter) override;
};