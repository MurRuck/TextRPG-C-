#include "PlayerCharacter.h"
#include <iostream>
#include "../../GameManager/LoggingManager/LoggingManager.h"
using namespace std;

//public
PlayerCharacter::PlayerCharacter(const std::string& characterName)
    : name(characterName)
{
    CreateCharacter();
}

void PlayerCharacter::ShowStatus() const
{
    cout << "===== 캐릭터 상태 =====" << endl;
    cout << "이름: " << name << endl;
    cout << "레벨: " << level << endl;
    cout << "체력: " << hp << " / " << maxHP << endl;
    cout << "마나: " << mp << endl;
    cout << "공격력: " << attack << endl;
    cout << "경험치: " << exp << " / " << max_EXP << endl;
    cout << "골드: " << gold << endl;
    cout << "======================" << endl;
}

void PlayerCharacter::TakeDamage(int damage)
{
    if (damage <= 0)
        return;

    hp -= damage;
    if (hp < 0)
        hp = 0;

    if (IsDead())
        cout << "으앙 쥬금... (x_x)" << endl;
}

void PlayerCharacter::GainEXP(int amount)
{
    if (amount <= 0 || IsMaxLevel())
        return;

    exp += amount;
    while (CanLevelUp() && !IsMaxLevel())
    {
        LevelUp();
    }
}

void PlayerCharacter::GainGold(int amount)
{
    if (amount <= 0)
        return;

    gold += amount;
}

void PlayerCharacter::BuyItem(int amount, std::unique_ptr<Item> item)
{
    if (amount <= 0 || !IsCanBuy(amount))
        return;
    
    AddItem(std::move(item));
    gold -= amount;
}

void PlayerCharacter::Heal(int amount)
{
    if (amount <= 0)
        return;

    hp += amount;
    if (hp > maxHP)
        hp = maxHP;
}

void PlayerCharacter::IncreaseAttack(int amount)
{
    if (amount <= 0)
        return;

    attack += amount;
}

void PlayerCharacter::AddItem(std::unique_ptr<Item> item)
{
    if (item == nullptr)
        return;

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

void PlayerCharacter::SellItem(int index)
{
    if (index < 0 || index >= static_cast<int>(inventory.size()))
        return;
    
    GainGold(inventory[index]->getPrice());
    
    inventory.erase(inventory.begin() + index);
}

void PlayerCharacter::CreateCharacter()
{
    maxHP = 200;
    hp = 200;
    mp = 100;
    exp = 0;
    attack = 30;
    level = 1;
    gold = 0;
    max_EXP = 100;

    skills.clear();
    
    //예시 skills.emplace_back("존나 개쌔게 내려찎기", 10, 20, SkillType::Attack);        
}

void PlayerCharacter::ShowSkills() const
{   //아직 logmng가 덜 구현되어 있어서 일단 적용 안함
    cout << "===== 스킬 목록 =====" << endl;
    for (size_t i = 0; i < skills.size(); i++)
    {
        cout << i << ". " << skills[i].GetName()
            << " / MP: " << skills[i].GetMpCost()
            << " / 효과량?: " << skills[i].GetEffectValue()
            << endl;
    }
    cout << "====================" << endl;
}

void PlayerCharacter::UseSkill(int index)
{
    if (index < 0 || index >= static_cast<int>(skills.size()))
        return;

    const Skill& skill = skills[index];
    if (!skill.CanUse(mp))
    {
        cout << "MP가 부족지에엥" << endl;
        return;
    }

    SetMP(mp - skill.GetMpCost());
    skill.Use(*this);
}

bool PlayerCharacter::IsDead() const
{
    return hp <= 0;
}

bool PlayerCharacter::IsMaxLevel() const
{
    return level >= MAX_LEVEL;
}

const std::string& PlayerCharacter::GetName() const
{
    return name;
}

int PlayerCharacter::GetLevel() const
{
    return level;
}

int PlayerCharacter::GetHP() const
{
    return hp;
}

int PlayerCharacter::GetMaxHP() const
{
    return maxHP;
}

int PlayerCharacter::GetAttack() const
{
    return attack;
}

int PlayerCharacter::GetMP() const
{
    return mp;
}

int PlayerCharacter::GetEXP() const
{
    return exp;
}

int PlayerCharacter::GetRequiredEXP() const
{
    return max_EXP;
}

int PlayerCharacter::GetGold() const
{
    return gold;
}

const std::vector<Skill>& PlayerCharacter::GetSkills() const
{
    return skills;
}

void PlayerCharacter::SetName(std::string name)
{
    this->name = name;
}

void PlayerCharacter::SetHP(int amount)
{
    hp = amount;
    if (hp < 0)
        hp = 0;
    if (hp > maxHP)
        hp = maxHP;
}

void PlayerCharacter::SetMP(int amount)
{
    mp = amount;
    if (mp < 0)
        mp = 0;
}

void PlayerCharacter::SetAttack(int amount)
{
    attack = amount;
    if (attack < 0)
        attack = 0;
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

bool PlayerCharacter::IsCanBuy(int amount) const
{
    return amount <= gold;
}
