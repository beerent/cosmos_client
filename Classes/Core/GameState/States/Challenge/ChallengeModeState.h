#pragma once

#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/UIState/UIStateMachine.h"

class ChallengeModeState : public BaseStateDepricated, public IUIStateMachineEventHandler {
public:

	ChallengeModeState(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(LEVEL_NAME)
	CONST_STRING_DEC(CHALLENGE_MODE_STATE)

	virtual void OnEnterState();
	virtual void OnExitState();

	void OnStateChanged();

	virtual STRING_ID GetStateID() { return CHALLENGE_MODE_STATE; }
};