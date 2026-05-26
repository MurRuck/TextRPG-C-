#pragma once 

#include <string>

#include "Item.h"

class PlayerCharacter;

//10~70 사이 랜덤 체력회복
class ChaosHpPotion : public Item {
private:
	std::string name = "ChaosHpPotion";
	int healthRestore = 0;
	int price = 35;
public:
	ChaosHpPotion();

	std::string getName() const override;
	int getPrice() const override;

	void use(PlayerCharacter* playerCharacter);
};