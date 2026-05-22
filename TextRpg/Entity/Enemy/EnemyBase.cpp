// EnemyBase.cpp

#include "EnemyBase.h"
#include <iostream>
#include <cstdlib>
using namespace std;

EnemyBase::EnemyBase(string name, int level, float statMultiplier)
{
	this->name = name;
	hp = ((rand() % (level * 10 + 1)) + level * 20) * statMultiplier;
	attack = ((rand() % (level * 5 + 1)) + level * 5) * statMultiplier;
	exp = level * 25;
	gold = level * 10;
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
	if (hp < 0) hp = 0;
}

bool EnemyBase::IsAlive() const
{
	return hp < 0;
}

void EnemyBase::PrintInfo() const
{
	cout << "[ " << name << " ]" << endl;
	cout << "HP: " << hp << endl;
	cout << "ATK: " << attack << endl;
}

void EnemyBase::OnDeath(PlayerCharacter* player)
{
	player->GainExp(exp);
	player->GainGold(gold);
}