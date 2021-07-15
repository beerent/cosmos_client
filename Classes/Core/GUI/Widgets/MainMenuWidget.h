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
    enum MainMenuItems{LOAD_GAME_TEST_LEVEL, LOAD_LOGIN_LEVEL, LOAD_CHALLENGE_MENU_LEVEL, LOAD_COSMOS_LIVE_LEVEL};
    typedef fastdelegate::FastDelegate1<MainMenuItems> onMenuItemSelectedCallBack;

	MainMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
    ~MainMenuWidget();
    
	void init();
	void release();
    
    void registerUsernamePressedCallback(UITouchButton::onButtonStateChangedCallBack usernamePressedCallback);
    
	void registerForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
	void unregisterForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
    
    void DisplayUsername();
    void UpdateMessage(const std::string& message, int offset);
    void ActivateRainbowMessageColor();
    void RegisterUsernameRefreshListener(IUsernameRefreshListener* listener) {
        m_usernameRefreshListener = listener;
    }
    
    void SetVisible(bool visible);
    int GetAppVersionPressCount() const;

private:
    UITouchButton::onButtonStateChangedCallBack m_usernamePressedCallback;
    std::list<onMenuItemSelectedCallBack> m_onMenuItemSelectedListeners;

    void OnUsernamePressed(UITouchButton::ButtonState state);
	void OnLoadChallengeMenu(UITouchButton::ButtonState state);
    void OnLoadCosmosLiveLobby(UITouchButton::ButtonState state);
    
    void DisplayChallengeModeButton();
    void DisplayCosmosLiveButton();
    
    void DisplayAppVersion();
    void TakeDownAppVersion();
    void DisplayBuyMeCoffee();
    void TakeDownBuyMeCoffee();
    void OnAppVersionPressed(UITouchButton::ButtonState state);
    void OnBuyMeCoffeePressed(UITouchButton::ButtonState state);
    void UpdateAppTitleTextColor();
    void UpdateLabelColor(int colorOffset, UILabel* labelComponent);
    void TakeDownMessage();
    
	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UIComponent* m_menu;
    UILabel* m_title;
    UILabel* m_usernamePrefix;
    UILabel* m_username;
    UILabel* m_challengeMode;
    UILabel* m_cosmosLive;
    UILabel* m_message;
    UILabel* m_appVersion;
    UILabel* m_buyMeCoffee;
    bool m_usingRainbowMessageColor;
    int m_appVersionPressCount;
    
    IUsernameRefreshListener* m_usernameRefreshListener;
};
