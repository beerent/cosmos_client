#pragma once

#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/UIState/UIStateMachine.h"

class UserProfileState : public BaseStateDepricated, public IUIStateMachineEventHandler {
public:

	UserProfileState(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(LEVEL_NAME)
	CONST_STRING_DEC(USER_PROFILE_STATE)

	virtual void OnEnterState();
	virtual void OnExitState();

	void OnStateChanged();

	virtual STRING_ID GetStateID() { return USER_PROFILE_STATE; }
};