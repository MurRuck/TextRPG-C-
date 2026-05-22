#include "Gamemode.h"

#include <iostream>
#include <ostream>

#include "../../Map/MapBase.h"
#include "../../Map/Battle/Battle.h"
#include "../../Map/Shop/Shop.h"

Gamemode::Gamemode()
{
    createState = std::make_unique<Create>();
    battleState = std::make_unique<Battle>();
    shopState = std::make_unique<Shop>();
}

Gamemode::~Gamemode() = default;

MapBase* Gamemode::GetCurrentMapState()
{
    switch (currentState)
    {
    case PlayerState::Create:
        return createState.get();
    case PlayerState::Battle:
        return battleState.get();
    case PlayerState::Shop:
        return shopState.get();
    }
}

PlayerState Gamemode::GetPlayerState() const
{
}


void Gamemode::InitUI()
{
}

void Gamemode::UIRenderer()
{
    if (MapBase* map = GetCurrentMapState())
    {
        map->Render(*this);
    }
}


void Gamemode::HandleInput()
{
}
