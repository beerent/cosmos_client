#include "Core/GUI/UIState/States/User/UIStateUserProfile.h"

#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GameState/GameStateMachine.h"

CONST_STRING_DEF(UIStateUserProfile, UI_STATE_USER_PROFILE)

void UIStateUserProfile::OnEnterState() {
}

void UIStateUserProfile::OnExitState() {
	BaseStateDepricated::OnExitState();
}