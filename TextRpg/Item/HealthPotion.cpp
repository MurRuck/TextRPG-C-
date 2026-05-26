#include "Item.h"
#include "HealthPotion.h"
#include "../Entity/Character/PlayerCharacter.h"

#include <string.h>
#include <algorithm>

std::string HealthPotion::getName() const {
	return this->name;
}

int HealthPotion::getPrice() const {
	return this->price;
}

void HealthPotion::use(PlayerCharacter* playerCharacter) {
	playerCharacter->SetHP(std::min(playerCharacter->GetMaxHP(), playerCharacter->GetHP() + healthRestore));
}