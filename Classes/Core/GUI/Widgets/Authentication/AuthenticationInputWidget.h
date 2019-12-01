#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/GUI/Components/UIComponent.h>
#include <Core/OpenSource/FastDelegate.h>

#include <list>

class AuthenticationInputWidget {

public:
	enum AuthenticationInputButtons { SUBMIT_AUTH_REQUEST};
    
	typedef fastdelegate::FastDelegate1<AuthenticationInputButtons> onButtonStateChangedCallBack;

	void init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
	void Release();

	void RegisterForMenuItemSelectedEvent(onButtonStateChangedCallBack callBack);
	void UnregisterForMenuItemSelectedEvent(onButtonStateChangedCallBack callBack);
    
    void RegisterUsernameFocusCallback(UILabel::onButtonStateChangedCallBack callback);
    void RegisterPasswordFocusCallback(UILabel::onButtonStateChangedCallBack callback);

	void UpdateUsername(const std::string& username);
	void UpdatePassword(const std::string& password);

	void ShowLoginFields();
	void ClearLoginFields();

	void ShowAuthenticatingFields();
	void ClearAuthenticatingFields();

	void ShowFailureFields();
	void ClearFailureFields();

	void IncrementFailureCount();
	bool HasFailureOccured() const;
private:
	std::list<onButtonStateChangedCallBack> m_buttonPressListeners;

	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UILabel* m_username;
	UILabel* m_password;
	UILabel* m_submitButton;
	UILabel* m_authenticating;
	UILabel* m_failure;

	void AddUsername();
	void AddPassword();
	void AddSubmitButton();
	void AddAuthenticating();
	void AddFailure();

	int m_failureCount;

	void OnSubmitCredentials(UITouchButton::ButtonState state);
};
