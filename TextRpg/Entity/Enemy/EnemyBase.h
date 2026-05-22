// monster.h

#pragma once
#include <string>

class EnemyBase
{
private:
	std::string name;
	int hp;
	int attack;
	int exp;
	int gold;

public:
	EnemyBase(std::string name, int level, float statMulfiplier = 1.0f);

	std::string GetName() const;
	int GetHp() const;
	int GetAttack() const;
	int GetExp() const;
	int GetGold() const;

	void TakeDamage(int damage);
	bool IsAlive() const;

	void PrintInfo() const;
};
