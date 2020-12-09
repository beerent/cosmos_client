#include <Core/GUI/UIState/States/UIStateMainMenu.h>

#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include <Core/General/DeviceMemoryInterface.h>

#include "IEngine.h"

CONST_STRING_DEF(UIStateMainMenu, UI_STATE_MAINMENU)

UIStateMainMenu::UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener): BaseStateDepricated(stateChangeListener), m_mainMenuWidget(nullptr), m_usernameEditWidget(nullptr) {}

UIStateMainMenu::~UIStateMainMenu() {}

void UIStateMainMenu::OnEnterState() {
    m_usernameEditWidget = new UsernameEditWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_usernameEditWidget->RegisterEditUsernameCloser(this);
    
    m_mainMenuWidget = new MainMenuWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_mainMenuWidget->init();
    
    MainMenuWidget::onMenuItemSelectedCallBack mainMenuSelectionCallback;
    mainMenuSelectionCallback.bind(this, &UIStateMainMenu::onMainMenuItemSelected);
    m_mainMenuWidget->registerForMenuItemSelectedEvent(mainMenuSelectionCallback);
    
    UITouchButton::onButtonStateChangedCallBack usernamePressedCallback;
    usernamePressedCallback.bind(this, &UIStateMainMenu::OnUsernamePressed);
    m_mainMenuWidget->registerUsernamePressedCallback(usernamePressedCallback);
}

void UIStateMainMenu::OnExitState() {
    m_mainMenuWidget->release();
    delete m_mainMenuWidget;
    m_mainMenuWidget = nullptr;
    
    BaseStateDepricated::OnExitState();
}

void UIStateMainMenu::onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem) {
    if (selectedItem == MainMenuWidget::LOAD_LOGIN_LEVEL) {
		ChangeState(UIStateAuthInput::UI_STATE_AUTH_INPUT);
	}

	else if (selectedItem == MainMenuWidget::LOAD_CHALLENGE_MENU_LEVEL) {
		ChangeState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
	}
}

void UIStateMainMenu::OnUsernamePressed(UITouchButton::ButtonState state) {
    m_mainMenuWidget->SetVisible(false);
    m_usernameEditWidget->Init();
}

void UIStateMainMenu::CloseEditUsername() {
    if (DeviceMemoryInterface().ReadRememberUsername()) {
        DeviceMemoryInterface().StoreUsername(IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername());
    } else {
        DeviceMemoryInterface().StoreUsername("");
    }
    
    IEngine::getEngine()->GetUserProvider()->LogOut();
    m_usernameEditWidget->Release();
    m_mainMenuWidget->SetVisible(true);
}
