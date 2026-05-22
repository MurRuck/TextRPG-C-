#pragma once
#include <cstdlib>
#include <memory>

#include "../../Entity/Character/PlayerCharacter.h"

class MapBase;
class Create;
class Battle;
class Shop;

enum class PlayerState { Create, Battle, Shop };

class Gamemode
{
public:
    Gamemode();
    ~Gamemode();


    // player getter
    const PlayerCharacter* GetPlayerCharacter() const { return player.get(); }

    // player setter
    void SetPlayerCharceter(std::unique_ptr<PlayerCharacter> newCharacter)
    {
        player = std::move(newCharacter);
    }

    // state getter
    PlayerState GetPlayerState() const;

    // state setter
    void SetPlayerState(PlayerState newState) { currentState = newState; }

    // UI
    void InitUI();
    void UIRenderer();
    void HandleInput();
    void ClearCLI() { std::system("cls"); }

private:
    MapBase* GetCurrentMapState();

    std::unique_ptr<PlayerCharacter> player;
    PlayerState currentState = PlayerState::Create;

    std::unique_ptr<Create> createState;
    std::unique_ptr<Battle> battleState;
    std::unique_ptr<Shop> shopState;
};
