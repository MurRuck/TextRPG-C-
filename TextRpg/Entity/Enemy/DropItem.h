// DropItem.h

#pragma once
#include "../../Item/Item.h"
#include <string>

class DropItem : public Item
{
private:
	std::string name;
	
public:
	DropItem(std::string itemName);
	std::string getName() const override;
	int getPrice() const override;
	void use(PlayerCharacter* playerCharacter) override;
};