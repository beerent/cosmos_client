//
//  DebugLogic.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/4/14.
//
//
#include "Core/StringManager/StringManager.h"
#include <Core/Input/InputManager.h>

#ifndef Projectios1_DebugLogic_h
#define Projectios1_DebugLogic_h
class DebugLogic
{
public:
	DebugLogic();
    static DebugLogic* getInstance();
    
    void fireProjectileFromCamera(STRING_ID type);
    
private:
    static DebugLogic* mInstance;

	void reloadShaders();
	void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data);
};

#endif
