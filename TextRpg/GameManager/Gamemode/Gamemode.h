#pragma once
#include <cstdlib>
#include <memory>

#include "../../Entity/Character/PlayerCharacter.h"

class MapBase;

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


private:

    std::unique_ptr<PlayerCharacter> player;
    PlayerState currentState = PlayerState::Create;
};

