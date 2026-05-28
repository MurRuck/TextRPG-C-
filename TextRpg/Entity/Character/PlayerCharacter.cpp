#include "PlayerCharacter.h"
#include <iostream>

#include "../../GameManager/Gamemode/Gamemode.h"
#include "../../UI/ConsoleUI.h"
using namespace std;

//public
PlayerCharacter::PlayerCharacter(const std::string& characterName, Gamemode* gamemode_ptr)
    : name(characterName), gamemode(gamemode_ptr)
{
    CreateCharacter();
}

void PlayerCharacter::ShowStatus() const
{
    ConsoleUI::PrintTitle("캐릭터 상태");
    
    std::vector<std::string> lines;
    lines.push_back("이름: " + name);
    lines.push_back("레벨: " + std::to_string(level));
    lines.push_back("HP: " + std::to_string(hp) + " / " + std::to_string(maxHP));
    lines.push_back("MP: " + std::to_string(mp));
    lines.push_back("공격력: " + std::to_string(attack));
    lines.push_back("EXP: " + std::to_string(exp) + " / " + std::to_string(max_EXP));
    lines.push_back("Gold: " + std::to_string(gold));
    
    
    ConsoleUI::PrintBox(lines);
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
    
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : ", "Player Died " + to_string(IsDead()));
    
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
    {
       gamemode->GetLogger()->Add(LogHeader::Error, "Player : WrongGold", amount);
        return;
    }

    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Gain Gold ", amount);
    gold += amount;
    
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Remain Gold ", to_string(GetGold()));
}

void PlayerCharacter::BuyItem(int amount, std::unique_ptr<Item> item)
{
    if (amount <= 0 || !IsCanBuy(amount) || item == nullptr)
    {
        gamemode->GetLogger()->Add(LogHeader::Error, "Insufficient Balance ", "Item : " + to_string(amount));
        gamemode->GetLogger()->Add(LogHeader::Info, "Player : RemainGold ", to_string(GetGold()));
        return;
    }

    const std::string itemName = item->getName();
    
    AddItem(std::move(item));
   gamemode->GetLogger()->Add(LogHeader::Info, "Player : Buy Item ", itemName);
    gold -= amount;
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Remain Gold ", to_string(GetGold()));
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

    const std::string itemName = item->getName();
    inventory.push_back(std::move(item));
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Add Item ", itemName);
}

void PlayerCharacter::ShowInventory() const
{
    for (size_t i = 0; i < inventory.size(); i++)
        cout << i << ". " << inventory[i]->getName() << endl;
}

void PlayerCharacter::ShowItemInspector(int index) const
{
    if (index < 0 || index >= static_cast<int>(inventory.size()))
        return;

    cout << inventory[index]->getName() << endl;
    cout << inventory[index]->getPrice() << endl;
}

void PlayerCharacter::UseItem(int index)
{
    if (index < 0 || index >= static_cast<int>(inventory.size()))
        return;
    
    inventory[index]->use(this);
    
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Use item ", inventory[index]->getName());
    
    inventory.erase(inventory.begin() + index);
}

void PlayerCharacter::SellItem(int index)
{
    if (index < 0 || index >= static_cast<int>(inventory.size()))
        return;
    
    int sellPrice = static_cast<int>(inventory[index]->getPrice() * 0.6f);
    
    GainGold(sellPrice);
    
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Sell item ", sellPrice);
    gamemode->GetLogger()->Add(LogHeader::Info, "Player : Remain Gold ",to_string(GetGold()));
    
    
    
    inventory.erase(inventory.begin() + index);
    
    
    cout << "  [" << name << "] sold! (+" << sellPrice << "G)\n";
    cout << "  Gold remaining: " << GetGold() << "G\n";
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
    
    gamemode->GetLogger()->Add(LogHeader::System, "Player : Player name set ", name);
    //예시 skills.emplace_back("존나 개쌔게 내려찎기", 10, 20, SkillType::Attack);        
}

void PlayerCharacter::ShowSkills() const
{   
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
