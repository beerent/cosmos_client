#include "Core/GameState/States/Challenge/ChallengeMenuState.h"
#include "IEngine.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"

CONST_STRING_DEF(ChallengeMenuState, LEVEL_NAME)
CONST_STRING_DEF(ChallengeMenuState, CHALLENGE_MENU_STATE)

void ChallengeMenuState::OnEnterState() {
	//LevelFactory* levelFactory = LevelFactory::getInstance();
	//levelFactory->clearCurrentLevel();
	//IEngine::getEngine()->ResetPhysics();
	//levelFactory->LoadLevel(StringManager::getIDForString("KYCPlanetSizeLevel"));

	UIStateMachine* uiStateMachine = UIStateMachine::GetInstance();
	uiStateMachine->SetEventHandler(*this);
	uiStateMachine->SetState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
}

void ChallengeMenuState::OnExitState() {
	DynamicObjectTrackerManager::getInstance()->clean();
	DynamicObjectTrackerManager::getInstance()->printReport();
	BaseStateDepricated::OnExitState();
}

void ChallengeMenuState::OnStateChanged() {
}
