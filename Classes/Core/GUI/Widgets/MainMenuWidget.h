#pragma once

#include <Core/OpenSource/FastDelegate.h>
#include <list>

#include <Core/GUI/Components/UITouchButton.h>
#include <Core/GUI/Components/UILabel.h>
#include <Core/Util/SimpleTimer.h>

class UIComponentFactory;
class UIComponent;

class IUsernameRefreshListener {
public:
    virtual void OnUsernameRefresh() = 0;
};

class MainMenuWidget {

public:
    enum MainMenuItems{LOAD_GAME_TEST_LEVEL, LOAD_LOGIN_LEVEL, LOAD_CHALLENGE_MENU_LEVEL};
    typedef fastdelegate::FastDelegate1<MainMenuItems> onMenuItemSelectedCallBack;

	MainMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
    ~MainMenuWidget();
    
	void init();
	void release();
	void registerForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
	void unregisterForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
    
    void RegisterUsernameFocusCallback(UILabel::onButtonStateChangedCallBack callback);
    
    void SetUsernameText();
    void SetGuestUsernameDisplay(const std::string& displayUsername);
    void RegisterUsernameRefreshListener(IUsernameRefreshListener* listener) {
        m_usernameRefreshListener = listener;
    }

private:
    std::list<onMenuItemSelectedCallBack> m_onMenuItemSelectedListeners;

	void OnLoadChallengeMenu(UITouchButton::ButtonState state);
	void onLogin(UITouchButton::ButtonState state);

	void AddLoginButton(UIComponent *parentComponent);
    void AddUsernameRefreshButton();
    void OnRefreshUsername(UITouchButton::ButtonState state);
    
    void DisplayAppVersion();
    void TakeDownAppVersion();
    
    //we're using input event callback to close the keyboard if the screen is touched.
    void onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data);
    InputManager::onInputEventCallBack m_closeKeyboardCallback;
	//void AddRegisterButton(UIComponent *parentComponent);

	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UIComponent* m_menu;
    UILabel* m_username;
    UILabel* m_usernameInputBox;
    UILabel* m_usernameRefreshButton;
	UILabel* m_loginButton;
    UILabel* m_appVersion;
    
    IUsernameRefreshListener* m_usernameRefreshListener;
    
	//UILabel* m_registerButton;
};
