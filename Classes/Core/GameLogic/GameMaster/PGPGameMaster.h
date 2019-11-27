//
//  IScriptedEventEngine.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__PGPGameMaster__
#define __Projectios2__PGPGameMaster__

#include <stdio.h>
#include "IGameMaster.h"
#include "Core/GameLogic/EntityControler/EntityControler.h"
#include "Core/Debug/Logger/Logger.h"

class PGPGameMaster : public GameMaster
{
public:    

	PGPGameMaster(Level& level) : GameMaster(level) {}
	void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data) {
		if (mActiveCharacterControler != nullptr) {
			mActiveCharacterControler->onInputEvent(event, data);
		}
	}

	void start(){
		Entity* playerEntity = getLevel().getEntityByName(StringManager::getIDForString("PlayerEntity"));
		if (playerEntity == nullptr) {
			Logger::getInstance().Error("Can't find playerEntity in level");
			return;
		}

		mActiveCharacterControler = playerEntity->getControler();
		
		if (mActiveCharacterControler == nullptr) {
			Logger::getInstance().Error("Can't find playerEntity in level");
			return;
		}
	}

	void stop(){
	}

private:
	EntityControler* mActiveCharacterControler;

};

#endif /* defined(__Projectios2__PGPGameMaster__) */
