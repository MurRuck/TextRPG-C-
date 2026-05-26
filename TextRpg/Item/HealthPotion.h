#pragma once

#include <string>

#include "Item.h"

class PlayerCharacter;

class HealthPotion : public Item{
private:
	std::string name = "HealthPotion";
	int healthRestore = 50;
	int price = 50;
public:
	HealthPotion() {}
	
	std::string getName() const override {}
	int getPrice() const override {}

	void use(PlayerCharacter* playerCharacter) override {}
};