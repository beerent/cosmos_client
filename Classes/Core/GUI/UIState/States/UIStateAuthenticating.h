#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Components/UILabel.h"

class UIStateAuthenticating : public BaseStateDepricated {

public:
	UIStateAuthenticating(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_AUTHENTICATING)

	virtual void OnEnterState();
	virtual void OnExitState();

	virtual STRING_ID GetStateID() { return UI_STATE_AUTHENTICATING; }

private:
	UILabel* mHeader;
};