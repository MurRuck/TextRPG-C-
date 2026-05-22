#pragma once

#include <string>

class PlayerCharacter;

class Item {
public:
	virtual std::string getName() const = 0;
	virtual void use(PlayerCharacter* PlayerCharacter) = 0;
	
	virtual ~Item() {}
};