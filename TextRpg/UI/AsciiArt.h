#pragma once
#include <string>
#include <vector>

enum class ArtType
{
    Title,
    Player,
    BattlePlayer,
    Slime,
    Goblin,
    Orc,
    Boss,
     LevelUp,
    Shop,
    NotEnoughGold,
    PurchaseSuccess
};

class AsciiArt
{
public:
    static void Print(ArtType type);
    static void PlayAsciiAnimation(const std::vector<std::vector<std::string>>& frames, int repeatCount = 6, int delayMs = 250);
    static void PlayTestAnimation();
    static const std::vector<std::vector<std::string>>& GetTestAnimationFrames();

private:
    static const std::vector<std::string>& GetArt(ArtType type);
};
