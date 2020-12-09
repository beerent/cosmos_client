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
    
    void registerUsernamePressedCallback(UITouchButton::onButtonStateChangedCallBack usernamePressedCallback);
    
	void registerForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
	void unregisterForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
    
    void DisplayUsername();
    void RegisterUsernameRefreshListener(IUsernameRefreshListener* listener) {
        m_usernameRefreshListener = listener;
    }
    
    void SetVisible(bool visible);

private:
    UITouchButton::onButtonStateChangedCallBack m_usernamePressedCallback;
    std::list<onMenuItemSelectedCallBack> m_onMenuItemSelectedListeners;

    void OnUsernamePressed(UITouchButton::ButtonState state);
	void OnLoadChallengeMenu(UITouchButton::ButtonState state);
    
    void DisplayAppVersion();
    void TakeDownAppVersion();
    
	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UIComponent* m_menu;
    UILabel* m_username;
    UILabel* m_appVersion;
    
    IUsernameRefreshListener* m_usernameRefreshListener;
};
