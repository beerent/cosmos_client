#include "Core/GameState/States/AuthenticationState.h"
#include "IEngine.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"

CONST_STRING_DEF(AuthenticationState, LEVEL_NAME)
CONST_STRING_DEF(AuthenticationState, AUTHENTICATION_STATE)

void AuthenticationState::OnEnterState() {
	//LevelFactory* levelFactory = LevelFactory::getInstance();
	//levelFactory->clearCurrentLevel();
	//IEngine::getEngine()->ResetPhysics();
	//levelFactory->LoadLevel(StringManager::getIDForString("KYCPlanetSizeLevel"));

	UIStateMachine* uiStateMachine = UIStateMachine::GetInstance();
	uiStateMachine->SetEventHandler(*this);
	uiStateMachine->SetState(UIStateAuthInput::UI_STATE_AUTH_INPUT);
}

void AuthenticationState::OnExitState() {
	DynamicObjectTrackerManager::getInstance()->clean();
	DynamicObjectTrackerManager::getInstance()->printReport();
	BaseStateDepricated::OnExitState();
}

void AuthenticationState::OnStateChanged() {
}
