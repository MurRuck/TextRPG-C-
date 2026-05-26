#include "Item.h"
#include "ChaosAttackBoost.h"
#include "../Entity/Character/PlayerCharacter.h"

#include <string>
#include <random>

ChaosAttackBoost::ChaosAttackBoost() {
	std::mt19937 gen(std::random_device{}());
	std::uniform_int_distribution<int> dist(5, 15);
	attackIncrease = dist(gen);
}

std::string ChaosAttackBoost::getName() const {
	return this->name;
}

int ChaosAttackBoost::getPrice() const {
	return this->price;
}

void ChaosAttackBoost::use(PlayerCharacter* playerCharacter) {
	playerCharacter->IncreaseAttack(attackIncrease);
}