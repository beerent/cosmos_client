//
//  IScriptedEventEngine.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__GameMaster__
#define __Projectios2__GameMaster__

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include <Core/Input/InputManager.h>
#include "IGameMaster.h"
#include "Core/LevelFactory/Level.h"

class GameMaster : public IGameMaster
{
public:
	CONST_STRING_DEC(PGP_GAME_MASTER)

    static IGameMaster* Factory(STRING_ID name, Level& level);
    
	GameMaster(Level& level) : mLevel(level) {
	}

    virtual ~GameMaster(){};

	Level& getLevel() { return mLevel; }

private:
	Level& mLevel;
};

#endif /* defined(__Projectios2__IGameMaster__) */
