#pragma once

#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/UIState/UIStateMachine.h"

class ChallengeMenuState : public BaseStateDepricated, public IUIStateMachineEventHandler {
public:

	ChallengeMenuState(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(LEVEL_NAME)
	CONST_STRING_DEC(CHALLENGE_MENU_STATE)

	virtual void OnEnterState();
	virtual void OnExitState();

	void OnStateChanged();

	virtual STRING_ID GetStateID() { return CHALLENGE_MENU_STATE; }
};