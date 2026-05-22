#include "PlayerCharacter.h"
#include <iostream>
using namespace std;

//public
void PlayerCharacter::ShowStatus() const
{
    cout << level << endl;
    cout << name << endl;
    cout << hp << endl;
    cout << attack << endl;
    cout << exp << endl;
    cout << gold << endl;
}

void PlayerCharacter::TakeDamage(int damage)
{
    hp -= damage;

    if (IsDead())
        cout << "으앙 쥬금... (×﹏×)" << endl;
}

void PlayerCharacter::GainEXP(int amount)
{
    if (IsMaxLevel())
        return;
    exp += amount;
    while (CanLevelUp() && !IsMaxLevel())
    {
        LevelUp();
    }
}

void PlayerCharacter::GainGold(int amount)
{
    gold += amount;
}

void PlayerCharacter::AddItem(std::unique_ptr<Item> item)
{
    inventory.push_back(std::move(item));
}

void PlayerCharacter::ShowInventory() const
{
    for (size_t i = 0; i < inventory.size(); i++)
        cout << i << ". " << inventory[i]->getName() << endl;
}

void PlayerCharacter::UseItem(int index)
{
    if (index < 0 || index >= static_cast<int>(inventory.size()))
        return;
    
    inventory[index]->use(this);
    
    inventory.erase(inventory.begin() + index);
}

bool PlayerCharacter::IsDead() const
{
    return hp <= 0;
}

int PlayerCharacter::GetAttack() const
{
    return attack;
}


//private
bool PlayerCharacter::CanLevelUp() const
{
    return exp >= max_EXP;
}

void PlayerCharacter::LevelUp()
{
    if (IsMaxLevel())
        return;
    exp -= max_EXP;
    level++;
    maxHP += level * 20;
    hp = maxHP;
    attack += level * 5;
}

bool PlayerCharacter::IsMaxLevel() const
{
    return level >= 10; //최대레벨은 고정이라 이렇게 해도 될듯?
}
