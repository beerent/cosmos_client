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
	m_currentSelectedOption = KeyboardSelectedOption::USERNAME;

	m_authenticationInputWidget = new AuthenticationInputWidget();
	m_authenticationInputWidget->init(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
	m_authenticationInputWidget->ShowLoginFields();
    IEngine::getEngine()->GetKeyboardManager()->RegisterKeyboardListener(this);
    IEngine::getEngine()->GetKeyboardManager()->ActivateKeyboard();

	AuthenticationInputWidget::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UIStateAuthInput::OnInputButtonPressed);
	m_authenticationInputWidget->RegisterForMenuItemSelectedEvent(callBack);

	Authenticator::AuthenticationResultListener callback;
	callback.bind(this, &UIStateAuthInput::OnAuthenticationResult);

	m_authenticator.SetRestConnector(IEngine::getEngine()->GetRestConnector());
	m_authenticator.SetUser(IEngine::getEngine()->GetUserProvider()->GetUser());
	m_authenticator.RegisterAuthenticationResultListener(callback);

	BaseStateDepricated::OnEnterState();
}

void UIStateAuthInput::OnExitState() {
    IEngine::getEngine()->GetKeyboardManager()->UnregisterKeyboardListener();
    IEngine::getEngine()->GetKeyboardManager()->DeactivateKeyboard();
	m_authenticationInputWidget->Release();
	delete m_authenticationInputWidget;
	BaseStateDepricated::OnExitState();
}

void UIStateAuthInput::OnCharacterPressed(char c) {
	switch (m_currentSelectedOption) {
	case (KeyboardSelectedOption::USERNAME):
		AddCharacterToUsername(c);
		break;
	case (KeyboardSelectedOption::PASSWORD):
		AddCharacterToPassword(c);
		break;
	}
}

void UIStateAuthInput::OnDeletePressed() {
	switch (m_currentSelectedOption) {
	case (KeyboardSelectedOption::USERNAME):
		RemoveCharacterFromUsername();
		break;
	case (KeyboardSelectedOption::PASSWORD):
		RemoveCharacterFromPassword();
		break;
	}
}

void UIStateAuthInput::OnEnterPressed() {
	switch (m_currentSelectedOption) {
	case (KeyboardSelectedOption::USERNAME):
		m_currentSelectedOption = KeyboardSelectedOption::PASSWORD;
		break;
	case (KeyboardSelectedOption::PASSWORD):
		m_currentSelectedOption = KeyboardSelectedOption::USERNAME;
		OnInputButtonPressed(AuthenticationInputWidget::AuthenticationInputButtons::SUBMIT_AUTH_REQUEST);
		break;
	}
}

void UIStateAuthInput::AddCharacterToUsername(char c) {
	m_username.push_back(c);
	m_authenticationInputWidget->UpdateUsername(m_username);
}

void UIStateAuthInput::AddCharacterToPassword(char c) {
	m_password.push_back(c);
	m_authenticationInputWidget->UpdatePassword(m_password);
}

void UIStateAuthInput::RemoveCharacterFromUsername() {
	if (m_username.size() == 0) {
		return;
	}

	m_username.pop_back();
	m_authenticationInputWidget->UpdateUsername(m_username);
}

void UIStateAuthInput::RemoveCharacterFromPassword() {
	if (m_password.size() == 0) {
		return;
	}

	m_password.pop_back();
	m_authenticationInputWidget->UpdatePassword(m_password);
}

void UIStateAuthInput::OnInputButtonPressed(AuthenticationInputWidget::AuthenticationInputButtons button) {
	User user(m_username, m_password);

	m_authenticator.SetUser(user);
	SubmitAuthenticationRequest();
}

void UIStateAuthInput::SubmitAuthenticationRequest() {
	m_authenticationInputWidget->ClearFailureFields();
	m_authenticationInputWidget->ClearLoginFields();
	m_authenticationInputWidget->ShowAuthenticatingFields();

	m_authenticator.SendAuthenticationRequest();
}

void UIStateAuthInput::OnAuthenticationResult(Authenticator::AuthenticationResult result) {
	switch (result) {
	case Authenticator::AuthenticationResult::SUCCESS:
		IEngine::getEngine()->AuthenticationResultReceived(result);
		UIStateMachine::GetInstance()->SetState(UIStateMainMenu::UI_STATE_MAINMENU);
		break;
            
	case Authenticator::AuthenticationResult::FAILURE:
		m_currentSelectedOption = KeyboardSelectedOption::USERNAME;

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
