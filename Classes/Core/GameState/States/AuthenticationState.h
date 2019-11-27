#pragma once

#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/UIState/UIStateMachine.h"

class AuthenticationState : public BaseStateDepricated, public IUIStateMachineEventHandler {
public:

	AuthenticationState(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(LEVEL_NAME)
	CONST_STRING_DEC(AUTHENTICATION_STATE)

	virtual void OnEnterState();
	virtual void OnExitState();

	void OnStateChanged();

	virtual STRING_ID GetStateID() { return AUTHENTICATION_STATE; }
};