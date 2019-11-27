#include <Core/GUI/UIState/States/UIStateMainMenu.h>

#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"

CONST_STRING_DEF(UIStateMainMenu, UI_STATE_MAINMENU)

void UIStateMainMenu::OnEnterState() {
    m_mainMenuWidget = new MainMenuWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_mainMenuWidget->init();
    
    MainMenuWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateMainMenu::onMainMenuItemSelected);
    m_mainMenuWidget->registerForMenuItemSelectedEvent(callback);
}

void UIStateMainMenu::OnExitState() {
    m_mainMenuWidget->release();
    delete(m_mainMenuWidget);
    BaseStateDepricated::OnExitState();
}

void UIStateMainMenu::onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem) {
    STRING_ID levelID = StringManager::UNDEFINED;

	if (selectedItem == MainMenuWidget::LOAD_LOGIN_LEVEL) {
		ChangeState(UIStateAuthInput::UI_STATE_AUTH_INPUT);
	}

	else if (selectedItem == MainMenuWidget::LOAD_CHALLENGE_MENU_LEVEL) {
		ChangeState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
	}
}