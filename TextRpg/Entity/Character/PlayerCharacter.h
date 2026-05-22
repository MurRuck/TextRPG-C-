#pragma once
#include "../../Item/Item.h"
#include<string>
#include <memory>
#include <vector>

class PlayerCharacter

{
public:
    void ShowStatus() const;
    void TakeDamage(int damage);
    void GainEXP(int amount);
    void GainGold(int amount);
    
    void AddItem(std::unique_ptr<Item> item);
    void ShowInventory() const;
    void UseItem(int index);
    
    bool IsDead() const;
    
    int GetAttack() const;
private:
    std::string name;
    int maxHP = 200;
    int hp = 200;
    int exp = 0;
    int attack = 30;
    int max_EXP = 100;
    int level = 1;
    int gold = 0;

    std::vector<std::unique_ptr<Item>> inventory;
    
    bool CanLevelUp() const;
    void LevelUp();
    bool IsMaxLevel() const;
};
