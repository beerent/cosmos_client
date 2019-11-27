#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"

#include <Core/GUI/Widgets/Authentication/AuthenticationInputWidget.h>
#include <Core/GameLogic/Authentication/Authenticator.h>

#include <Core/Keyboard/IKeyboardListener.h>

class UIStateAuthInput : public BaseStateDepricated, IKeyboardListener {
	enum class KeyboardSelectedOption {
		USERNAME,
		PASSWORD
	};

public:
	UIStateAuthInput(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_AUTH_INPUT)

	virtual void OnEnterState();
	virtual void OnExitState();
    
    virtual void OnCharacterPressed(char c);
    virtual void OnDeletePressed();
	virtual void OnEnterPressed();

	virtual STRING_ID GetStateID() { return UI_STATE_AUTH_INPUT; }

private:
	AuthenticationInputWidget* m_authenticationInputWidget;
	KeyboardSelectedOption m_currentSelectedOption;

	std::string m_username;
	std::string m_password;

	Authenticator m_authenticator;

	void AddCharacterToUsername(char c);
	void AddCharacterToPassword(char c);

	void RemoveCharacterFromUsername();
	void RemoveCharacterFromPassword();

	void SubmitAuthenticationRequest();
	void OnAuthenticationResult(Authenticator::AuthenticationResult result);

	void OnInputButtonPressed(AuthenticationInputWidget::AuthenticationInputButtons button);

};
