//
//  IScriptedEventEngine.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/17/15.
//
//

#ifndef __Projectios2__IGameMaster__
#define __Projectios2__IGameMaster__

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include <Core/Input/InputManager.h>

class IGameMaster
{
public:
    virtual ~IGameMaster(){};

	virtual void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data) {};
	virtual void start() {};
	virtual void stop() {};
};

#endif /* defined(__Projectios2__IGameMaster__) */
