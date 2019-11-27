#pragma once

#include <Core/OpenSource/FastDelegate.h>
#include <list>

#include <Core/GUI/Components/UITouchButton.h>
#include <Core/GUI/Components/UILabel.h>

class UIComponentFactory;
class UIComponent;

class MainMenuWidget {

public:
    enum MainMenuItems{LOAD_GAME_TEST_LEVEL, LOAD_LOGIN_LEVEL, LOAD_CHALLENGE_MENU_LEVEL};
    typedef fastdelegate::FastDelegate1<MainMenuItems> onMenuItemSelectedCallBack;

	MainMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
	void init();
	void release();
	void registerForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);
	void unregisterForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack);

private:
    std::list<onMenuItemSelectedCallBack> m_onMenuItemSelectedListeners;

	void OnLoadChallengeMenu(UITouchButton::ButtonState state);
	void onLogin(UITouchButton::ButtonState state);

	void AddLoginButton(UIComponent *parentComponent);
	void AddRegisterButton(UIComponent *parentComponent);

	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UIComponent *m_menu;
	UILabel* m_loginButton;
	UILabel* m_registerButton;

};