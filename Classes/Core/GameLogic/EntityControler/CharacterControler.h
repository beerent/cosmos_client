#ifndef __Projectios2__CharacterControler__
#define __Projectios2__CharacterControler__

#include <stdio.h>
#include "Core/GameLogic/EntityControler/EntityControler.h"
#include <Core/Input/InputManager.h>

class CharacterControler : public EntityControler
{
public:
	CharacterControler();
	~CharacterControler();

	void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data);

private:

};

#endif /* defined(__Projectios2__CharacterControler__) */
