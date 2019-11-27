#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Components/UILabel.h"

#include <Core/GameLogic/Authentication/Authenticator.h>

class UIStateAuthAuthenticating : public BaseStateDepricated {

public:
	UIStateAuthAuthenticating(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_AUTH_AUTHENTICATING)

	virtual void OnEnterState();
	virtual void OnExitState();
    
	virtual STRING_ID GetStateID() { return UI_STATE_AUTH_AUTHENTICATING; }

private:
	UILabel* mHeader;

	Authenticator m_authenticator;

	void DisplayUI();
	void OnAuthenticationResult(Authenticator::AuthenticationResult result);
	void SubmitAuthenticationRequest();
};
