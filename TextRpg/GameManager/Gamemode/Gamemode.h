#pragma once
#include <cstdlib>
#include <memory>
#include "../../Entity/Character/PlayerCharacter.h"
#include "../LoggingManager/LoggingManager.h"

class MapBase;

enum class PlayerState { Create, Battle, Shop };

class Gamemode
{
public:
    Gamemode();
    ~Gamemode();
    
    
    // logger getter;
    LoggingManager* GetLogger()
    {
        return logger.get();
    }
    
    // logger getter const
    const LoggingManager* GetLogger() const
    {
        return logger.get();
    } 

    // player getter
    const PlayerCharacter& GetPlayerCharacter() const { return *player; }
    PlayerCharacter* GetMutablePlayer() { return player.get(); }

    // player setter
    void SetPlayerCharceter(std::unique_ptr<PlayerCharacter> newCharacter)
    {
        player = std::move(newCharacter);
    }

    // state getter
    PlayerState GetPlayerState() const { return currentState; }

    // state setter
    void SetPlayerState(PlayerState newState) { currentState = newState; }
    void LoopRun();
    
    


private:
    std::unique_ptr<PlayerCharacter> player;
    std::unique_ptr<LoggingManager> logger = std::make_unique<LoggingManager>();
    PlayerState currentState = PlayerState::Create;
};

