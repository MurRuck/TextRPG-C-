#pragma once
#include "../../Item/Item.h"
#include<string>
#include <memory>
#include <vector>
#include "Skill.h"
class PlayerCharacter

{
public:
    explicit PlayerCharacter(const std::string& characterName = "Player");

    void ShowStatus() const;
    void TakeDamage(int damage);
    void GainEXP(int amount);
    void GainExp(int amount) { GainEXP(amount); }
    void GainGold(int amount);
    void BuyItem(int amount);
    void Heal(int amount);
    void IncreaseAttack(int amount);
    
    void AddItem(std::unique_ptr<Item> item);
    void ShowInventory() const;
    void UseItem(int index);
    void CreateCharacter();
    void ShowSkills() const;
    void UseSkill(int index);
    
    bool IsDead() const;
    bool IsMaxLevel() const;
    
    const std::string& GetName() const;
    int GetLevel() const;
    int GetHP() const;
    int GetMaxHP() const;
    int GetAttack() const;
    int GetMP() const;
    int GetEXP() const;
    int GetRequiredEXP() const;
    int GetGold() const;
    const std::vector<Skill>& GetSkills() const;
    
    void SetName(std::string name);
    void SetHP(int amount);
    void SetMP(int amount);
    void SetAttack(int amount);
private:
    static constexpr int MAX_LEVEL = 10;

    std::string name;
    int maxHP = 200;
    int hp = 200;
    int mp = 100;
    int exp = 0;
    int attack = 30;
    int max_EXP = 100;
    int level = 1;
    int gold = 0;

    std::vector<std::unique_ptr<Item>> inventory;
    std::vector<Skill> skills;
    bool CanLevelUp() const;
    void LevelUp();
    bool IsCanBuy(int amount) const;
};
