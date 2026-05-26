#include "Item.h"
#include "ChaosHpPotion.h"
#include "../Entity/Character/PlayerCharacter.h"

#include <string>
#include <algorithm>
#include <random>

ChaosHpPotion::ChaosHpPotion() {
	std::mt19937 gen (std::random_device{}());
	std::uniform_int_distribution<int> dist(10, 70);
	healthRestore = dist(gen);
}

std::string ChaosHpPotion::getName() const {
	return this->name;
}

int ChaosHpPotion::getPrice() const {
	return this->price;
}

void ChaosHpPotion::use(PlayerCharacter* playerCharacter) {
	playerCharacter->Heal(healthRestore);
}