#include "Item.h"
#include "UnsaltyPotatoChip.h"
#include "../Entity/Character/PlayerCharacter.h"

#include <string>

std::string UnsaltyPotatoChip::getName() const {
	return this->name;
}

int UnsaltyPotatoChip::getPrice() const {
	return this->price;
}

void UnsaltyPotatoChip::use(PlayerCharacter* playerCharacter) {
	playerCharacter->IncreaseAttack(attackIncrease);
	playerCharacter->Heal(healthRestore);
}
