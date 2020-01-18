#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"
#include "Core/GUI/UIState/States/UIStateMainMenu.h"


#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GUI/UIState/UIStateMachine.h"
#include "Core/GameState/GameStateMachine.h"

#include <Core/User/UserProvider.h>
#include <Core/User/User.h>

CONST_STRING_DEF(UIStateAuthInput, UI_STATE_AUTH_INPUT)

void UIStateAuthInput::OnEnterState() {
	m_authenticationInputWidget = new AuthenticationInputWidget();
	m_authenticationInputWidget->init(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
	m_authenticationInputWidget->ShowLoginFields();
    IEngine::getEngine()->GetKeyboardManager()->RegisterKeyboardListener(this);

	AuthenticationInputWidget::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UIStateAuthInput::OnInputButtonPressed);
	m_authenticationInputWidget->RegisterForMenuItemSelectedEvent(callBack);


	m_authenticator.SetRestConnector(IEngine::getEngine()->GetRestConnector());
	m_authenticator.SetUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	m_authenticator.RegisterAuthenticationResultListener(this);
    
    RegisterUsernameListener();
    RegisterPasswordListener();

	BaseStateDepricated::OnEnterState();
}

void UIStateAuthInput::OnExitState() {
    IEngine::getEngine()->GetKeyboardManager()->UnregisterKeyboardListener();
    m_authenticator.UnregisterAuthenticationResultListener();
	m_authenticationInputWidget->Release();
	delete m_authenticationInputWidget;
	BaseStateDepricated::OnExitState();
}

void UIStateAuthInput::OnCharacterPressed(char c) {
	switch (m_currentEditField) {
	case (KeyboardSelectedOption::USERNAME):
		AddCharacterToUsername(c);
		break;
	case (KeyboardSelectedOption::PASSWORD):
		AddCharacterToPassword(c);
		break;
    default:
        break;
	}
}

void UIStateAuthInput::OnDeletePressed() {
	switch (m_currentEditField) {
	case (KeyboardSelectedOption::USERNAME):
		RemoveCharacterFromUsername();
		break;
	case (KeyboardSelectedOption::PASSWORD):
		RemoveCharacterFromPassword();
		break;
    default:
        break;
	}
}

void UIStateAuthInput::OnEnterPressed() {
    m_currentEditField = KeyboardSelectedOption::INVALID;
    OnUsernameLostFocus();
    OnPasswordLostFocus();
    RegisterUsernameListener();
    RegisterPasswordListener();
    m_keyboardManager->DeactivateKeyboard();
}

void UIStateAuthInput::AddCharacterToUsername(char c) {
	m_username.push_back(c);
    DisplayUsernameCursor(); //updates the username with added char 'c'
}

void UIStateAuthInput::AddCharacterToPassword(char c) {
	m_password.push_back(c);
    DisplayPasswordCursor(); //updates the password with added char 'c'
}

void UIStateAuthInput::RemoveCharacterFromUsername() {
	if (m_username.size() == 0) {
		return;
	}

	m_username.pop_back();
    DisplayUsernameCursor(); //updates the username with the removed char
}

void UIStateAuthInput::RemoveCharacterFromPassword() {
	if (m_password.size() == 0) {
		return;
	}

	m_password.pop_back();
    DisplayPasswordCursor(); //updates the password with the removed char
}

void UIStateAuthInput::OnInputButtonPressed(AuthenticationInputWidget::AuthenticationInputButtons button) {
    m_timer.DeregisterTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
    m_keyboardManager->DeactivateKeyboard();
	User user(m_username, m_password, UserAccessLevel::MEMBER);

	m_authenticator.SetUser(user);
	SubmitAuthenticationRequest();
}

void UIStateAuthInput::SubmitAuthenticationRequest() {
	m_authenticationInputWidget->ClearFailureFields();
	m_authenticationInputWidget->ClearLoginFields();
	m_authenticationInputWidget->ShowAuthenticatingFields();

	m_authenticator.SendAuthenticationRequest();
}

void UIStateAuthInput::OnAuthenticationResultReceived(AuthenticationResult result) {
	switch (result) {
	case AuthenticationResult::SUCCESS:
		IEngine::getEngine()->OnAuthenticationResultReceived(result);
		UIStateMachine::GetInstance()->SetState(UIStateMainMenu::UI_STATE_MAINMENU);
		break;
            
	case AuthenticationResult::FAILURE:
		m_currentEditField = KeyboardSelectedOption::USERNAME;

		m_username.clear();
		m_password.clear();

		m_authenticationInputWidget->IncrementFailureCount();

		m_authenticationInputWidget->ClearAuthenticatingFields();
		m_authenticationInputWidget->ShowFailureFields();
		m_authenticationInputWidget->ShowLoginFields();
		break;
            
	default:
		break;
	}
}

void UIStateAuthInput::OnTimerEvent(Timer::TimerType type) {
    switch(type) {
        case Timer::TimerType::CURSOR_BLINK_500_MS:            
            if (m_currentEditField == KeyboardSelectedOption::USERNAME) {
                if (m_cursorOn) {
                    HideUsernameCursor();
                    m_cursorOn = false;
                } else {
                    DisplayUsernameCursor();
                    m_cursorOn = true;
                }
                
            } else if (m_currentEditField == KeyboardSelectedOption::PASSWORD) {
                if (m_cursorOn) {
                    HidePasswordCursor();
                    m_cursorOn = false;
                } else {
                    DisplayPasswordCursor();
                    m_cursorOn = true;
                }
            }
            
            break;
        default:
            break;
    }
}

void UIStateAuthInput::DisplayUsernameCursor() {
    m_authenticationInputWidget->UpdateUsername(m_username + "|");
    RegisterUsernameListener();
}

void UIStateAuthInput::DisplayPasswordCursor() {
    m_authenticationInputWidget->UpdatePassword(m_password + "|");
    RegisterPasswordListener();
}

void UIStateAuthInput::HideUsernameCursor() {
    m_authenticationInputWidget->UpdateUsername(m_username);
    RegisterUsernameListener();
}

void UIStateAuthInput::HidePasswordCursor() {
    m_authenticationInputWidget->UpdatePassword(m_password);
    RegisterPasswordListener();
}

void UIStateAuthInput::OnUsernameLostFocus() {
    if (m_username.empty()) {
        m_authenticationInputWidget->UpdateUsername("enter username...");
    } else {
        HideUsernameCursor();
    }
    
    RegisterUsernameListener();
}

void UIStateAuthInput::OnPasswordLostFocus() {
    if (m_password.empty()) {
        m_authenticationInputWidget->UpdatePassword("enter password...");
    } else {
        HidePasswordCursor();
    }
    
    RegisterPasswordListener();
}

void UIStateAuthInput::OnUsernamePressed(UITouchButton::ButtonState state) {
    OnPasswordLostFocus();
    DisplayUsernameCursor();
    
    m_keyboardManager->ActivateKeyboard();
    m_currentEditField = KeyboardSelectedOption::USERNAME;
    m_timer.RegisterTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
}

void UIStateAuthInput::OnPasswordPressed(UITouchButton::ButtonState state) {
    OnUsernameLostFocus();
    DisplayPasswordCursor();
    
    m_keyboardManager->ActivateKeyboard();
    m_currentEditField = KeyboardSelectedOption::PASSWORD;
    m_timer.RegisterTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
}

void UIStateAuthInput::RegisterUsernameListener() {
    UILabel::onButtonStateChangedCallBack usernameCallback;
    usernameCallback.bind(this, &::UIStateAuthInput::OnUsernamePressed);
    m_authenticationInputWidget->RegisterUsernameFocusCallback(usernameCallback);
}

void UIStateAuthInput::RegisterPasswordListener() {
    UILabel::onButtonStateChangedCallBack passwordCallback;
    passwordCallback.bind(this, &::UIStateAuthInput::OnPasswordPressed);
    m_authenticationInputWidget->RegisterPasswordFocusCallback(passwordCallback);
}
