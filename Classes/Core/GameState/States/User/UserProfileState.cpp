#include <Core/GameState/States/User/UserProfileState.h>

#include "Core/LevelFactory/LevelFactory.h"
#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GUI/UIState/States/User/UIStateUserProfile.h"

CONST_STRING_DEF(UserProfileState, LEVEL_NAME)
CONST_STRING_DEF(UserProfileState, USER_PROFILE_STATE)

void UserProfileState::OnEnterState() {
	UIStateMachine* uiStateMachine = UIStateMachine::GetInstance();
	uiStateMachine->SetEventHandler(*this);
	uiStateMachine->SetState(UIStateUserProfile::UI_STATE_USER_PROFILE);
}

void UserProfileState::OnExitState() {
	DynamicObjectTrackerManager::getInstance()->clean();
	DynamicObjectTrackerManager::getInstance()->printReport();
	BaseStateDepricated::OnExitState();
}

void UserProfileState::OnStateChanged() {
}