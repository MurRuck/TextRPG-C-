#pragma once

#include <string>

#include "Item.h"

class PlayerCharacter;

class HealthPotion : public Item{
private:
	std::string name = "HealthPotion";
	int healthRestore = 50;

public:
	HealthPotion() {}
	
	std::string getName() override {}
	
	void use(PlayerCharacter* playerCharacter) override {}
};