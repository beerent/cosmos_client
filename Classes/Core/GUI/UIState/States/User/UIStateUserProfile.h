#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"

class UIStateUserProfile : public BaseStateDepricated {
public:
	UIStateUserProfile(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_USER_PROFILE)

	virtual void OnEnterState();
	virtual void OnExitState();

	virtual STRING_ID GetStateID() { return UI_STATE_USER_PROFILE; }

};