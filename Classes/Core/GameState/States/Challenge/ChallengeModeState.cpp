#include "Core/GameState/States/Challenge/ChallengeModeState.h"
#include "IEngine.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h"

CONST_STRING_DEF(ChallengeModeState, LEVEL_NAME)
CONST_STRING_DEF(ChallengeModeState, CHALLENGE_MODE_STATE)

void ChallengeModeState::OnEnterState() {
	//LevelFactory* levelFactory = LevelFactory::getInstance();
	//levelFactory->clearCurrentLevel();
	//IEngine::getEngine()->ResetPhysics();
	//levelFactory->LoadLevel(StringManager::getIDForString("KYCPlanetSizeLevel"));

	UIStateMachine* uiStateMachine = UIStateMachine::GetInstance();
	uiStateMachine->SetEventHandler(*this);
	uiStateMachine->SetState(UIStateChallengeMode::UI_STATE_CHALLENGE_MODE);
}

void ChallengeModeState::OnExitState() {
	DynamicObjectTrackerManager::getInstance()->clean();
	DynamicObjectTrackerManager::getInstance()->printReport();
	BaseStateDepricated::OnExitState();
}

void ChallengeModeState::OnStateChanged() {
}
