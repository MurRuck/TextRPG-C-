#pragma once

#include "../GameManager/Gamemode/Gamemode.h"

class MapBase
{
public:
    virtual ~MapBase() = default;
    virtual void Render(Gamemode& gamemode) = 0;
    virtual void HandleInput(Gamemode& gamemode) = 0;
};
