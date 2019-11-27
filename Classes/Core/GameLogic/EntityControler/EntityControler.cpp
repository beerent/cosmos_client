//
//  EntityControler.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#include "Core/GameLogic/EntityControler/EntityControler.h"
#include "Core/Debug/Logger/Logger.h" 
#include "ScriptedShipControler.h"
#include "Core/GameLogic/EntityControler/CharacterControler.h"
#include "Core/GameLogic/EntityControler/AICharacterControler.h"

CONST_STRING_DEF(EntityControler, SCRIPT_SHIP_CONTROLLER);
CONST_STRING_DEF(EntityControler, CHARACTER_CONTROLLER);
CONST_STRING_DEF(EntityControler, AI_CHARACTER_CONTROLLER);

EntityControler* Factory(STRING_ID name);

EntityControler* EntityControler::Factory(STRING_ID name)
{
    if(name == SCRIPT_SHIP_CONTROLLER)
    {
        return new ScriptedShipControler();
	}
	else if (name == CHARACTER_CONTROLLER)
	{
		return new CharacterControler();
	}
	else if (name == AI_CHARACTER_CONTROLLER)
	{
		return new AICharacterControler();
	}
    
    sprintf(Logger::GetTempCharBuffer(), "__FUNCTION__ Unknown entity controler name %s",
            StringManager::getStringFromID(name)->c_str());
    Logger::Error(Logger::GetTempCharBuffer());
    
    return NULL;
}
