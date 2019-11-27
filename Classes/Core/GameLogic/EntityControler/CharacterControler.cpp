#include "CharacterControler.h"
#include <IEngine.h>
#include <Core/Events/GameEvents.h>
#include "Core/Events/EventDispatcher.h"
#include <Core/Entity/Entities/Entity.h>

CharacterControler::CharacterControler() {
}

CharacterControler::~CharacterControler() {

}

void CharacterControler::onInputEvent(InputManager::InputEvent keyEvent, InputManager::InputEventData data)
{
	if (keyEvent == InputManager::ON_KEY_DOWN && data.getCharValue() == 'W')
	{ 
		Event gameEvent(GameEvents::MOVE_FORWARD_START);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_UP	&& data.getCharValue() == 'W')
	{
		Event gameEvent(GameEvents::MOVE_FORWARD_STOP);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_DOWN && data.getCharValue() == 'S')
	{
		Event gameEvent(GameEvents::MOVE_BACKWARDS_START);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_UP	&& data.getCharValue() == 'S')
	{
		Event gameEvent(GameEvents::MOVE_BACKWARDS_STOP);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_DOWN && data.getCharValue() == 'A')
	{
		Event gameEvent(GameEvents::TURN_LEFT_START);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_UP	&& data.getCharValue() == 'A')
	{
		Event gameEvent(GameEvents::TURN_LEFT_STOP);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_DOWN && data.getCharValue() == 'D')
	{
		Event gameEvent(GameEvents::TURN_RIGHT_START);
		mEntity->dispatchEvent(gameEvent);
	}
	else if (keyEvent == InputManager::ON_KEY_UP	&& data.getCharValue() == 'D')
	{
		Event gameEvent(GameEvents::TURN_RIGHT_STOP);
		mEntity->dispatchEvent(gameEvent);
	}
}