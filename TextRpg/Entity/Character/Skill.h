#pragma once
#include <string>

class PlayerCharacter;

enum class SkillType
{
    Attack,
    Buff,
    Heal
};

class Skill
{
public:
    Skill(const std::string& name, int mpCost, int effectValue, SkillType type);

    const std::string& GetName() const;
    int GetMpCost() const;
    int GetEffectValue() const;
    SkillType GetType() const;

    int GetTotalDamage(int playerAttack) const;
    bool CanUse(int currentMp) const;
    void Use(PlayerCharacter& playerCharacter) const;

private:
    void UseAttackSkill(PlayerCharacter& playerCharacter) const;
    void UseBuffSkill(PlayerCharacter& playerCharacter) const;
    void UseHealSkill(PlayerCharacter& playerCharacter) const;

    std::string name;
    int mpCost;
    int effectValue;
    SkillType type;
};
