//
//  IScriptedEventEngine.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#include "GameMaster.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/GameLogic/GameMaster/PGPGameMaster.h"
#include <Core/LevelFactory/Level.h>

CONST_STRING_DEF(GameMaster, PGP_GAME_MASTER)

IGameMaster* GameMaster::Factory(STRING_ID name, Level& level)
{
    if(name == PGP_GAME_MASTER)
    {
        return new PGPGameMaster(level);
    }
    
    sprintf(Logger::GetTempCharBuffer(), "__FUNCTION__ Unknown game master name %s",
            StringManager::getStringFromID(name)->c_str());
    Logger::Error(Logger::GetTempCharBuffer());
    
    return NULL;
}
