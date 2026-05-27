#include "Gamemode.h"
#include "../../Entity/Character/PlayerCharacter.h"
#include "../../Map/Battle/Battle.h"

Gamemode::Gamemode()
{
}

Gamemode::~Gamemode() = default;


void Gamemode::LoopRun()
{
    while (true)
    {
        switch (currentState)
        {
        case PlayerState::Create:
        {
            // 캐릭터 생성 로직
            // player 생성 후
            // 상태 변경
            SetPlayerState(PlayerState::Battle);
            break;
        }

        case PlayerState::Battle:
        {
            BattleLoop battle(*this);

            battle.StartBattle(player.get());


            currentState = battle.HandleBattleEnd();

            break;
        }
        case PlayerState::Shop:
        {
            // 상점 로직
            // 아이템 구매 / 회복 등

            // 다시 전투로
            SetPlayerState(PlayerState::Battle);
            break;
        }

        }
       
        if (!player)
            break;
    }
}