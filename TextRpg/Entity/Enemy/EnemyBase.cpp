#include "EnemyBase.h"

#include <cstdlib>
#include <iostream>
#include <memory>

using namespace std;

EnemyBase::EnemyBase(string name, int level, float statMultiplier)
{
	this->name = name;
	hp = static_cast<int>(((rand() % (level * 10 + 1)) + level * 20) * statMultiplier);
	attack = static_cast<int>(((rand() % (level * 5 + 1)) + level * 5) * statMultiplier);
	exp = level * 25;
	gold = level * 10;

	if (name == "Slime")
		dropItemName = "슬라임의 방울";
	else if (name == "Goblin")
		dropItemName = "고블린 이빨";
	else if (name == "Orc")
		dropItemName = "오크 가죽";
	else
		dropItemName = "Unknown Drop";
}

string EnemyBase::GetName() const
{
	return name;
}

int EnemyBase::GetHp() const
{
	return hp;
}

int EnemyBase::GetAttack() const
{
	return attack;
}

int EnemyBase::GetExp() const
{
	return exp;
}

int EnemyBase::GetGold() const
{
	return gold;
}

void EnemyBase::TakeDamage(int damage)
{
	hp -= damage;
	if (hp < 0)
		hp = 0;
}

bool EnemyBase::IsAlive() const
{
	return hp > 0;
}

void EnemyBase::PrintInfo() const
{
	cout << "[ " << name << " ]" << endl;
	cout << "HP: " << hp << endl;
	cout << "ATK: " << attack << endl;
}

void EnemyBase::OnDeath(PlayerCharacter* player)
{
	if (!player)
		return;

	player->GainEXP(exp);
	player->GainGold(gold);
	player->AddItem(std::make_unique<DropItem>(dropItemName));
}
