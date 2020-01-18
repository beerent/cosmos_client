#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"

#include <Core/GUI/Widgets/Authentication/AuthenticationInputWidget.h>
#include <Core/GameLogic/Authentication/Authenticator.h>

#include <Core/Util/SimpleTimer.h>
#include <Core/Keyboard/IKeyboardListener.h>

class UIStateAuthInput : public BaseStateDepricated, IKeyboardListener, Timer::SimpleTimerListener, IAuthenticationResultListener {
	enum class KeyboardSelectedOption {
        INVALID = -1,
		USERNAME,
		PASSWORD
	};

public:
	UIStateAuthInput(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener), m_cursorOn(false), m_timer(this), m_currentEditField(KeyboardSelectedOption::INVALID) {
        m_keyboardManager = IEngine::getEngine()->GetKeyboardManager();
    }
    
	CONST_STRING_DEC(UI_STATE_AUTH_INPUT)

	virtual void OnEnterState();
	virtual void OnExitState();
    
    virtual void OnDeletePressed();
    virtual void OnCharacterPressed(char c);
    virtual void OnEnterPressed();
    
    virtual void OnTimerEvent(Timer::TimerType type);

	virtual STRING_ID GetStateID() { return UI_STATE_AUTH_INPUT; }

private:
	AuthenticationInputWidget* m_authenticationInputWidget;
	KeyboardSelectedOption m_currentEditField;

	std::string m_username;
	std::string m_password;

	Authenticator m_authenticator;

	void AddCharacterToUsername(char c);
	void AddCharacterToPassword(char c);

	void RemoveCharacterFromUsername();
	void RemoveCharacterFromPassword();

	void SubmitAuthenticationRequest();
	void OnAuthenticationResultReceived(AuthenticationResult result);

	void OnInputButtonPressed(AuthenticationInputWidget::AuthenticationInputButtons button);
    
    Timer::SimpleTimer m_timer;
    KeyboardManager* m_keyboardManager;
    
    bool m_cursorOn;
    
    void DisplayUsernameCursor();
    void DisplayPasswordCursor();
    void HideUsernameCursor();
    void HidePasswordCursor();
    
    void RegisterUsernameListener();
    void RegisterPasswordListener();
    
    void OnUsernamePressed(UITouchButton::ButtonState state);
    void OnPasswordPressed(UITouchButton::ButtonState state);
 
    void OnUsernameLostFocus();
    void OnPasswordLostFocus();
};
