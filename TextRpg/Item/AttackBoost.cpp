#include "Item.h"
#include "AttackBoost.h"
#include "../Entity/Character/PlayerCharacter.h"

#include <string>

std::string AttackBoost::getName() const {
	return this->name;
}

int AttackBoost::getPrice() const {
	return this->price;
}

void AttackBoost::use(PlayerCharacter* playerCharacter) {
	playerCharacter->SetAttack(playerCharacter->GetAttack() + attackIncrease);
}