#include <Core/GUI/UIState/States/UIStateMainMenu.h>

#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GUI/UIState/States/Authentication/UIStateAuthInput.h"
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include <Core/Util/UsernameGenerator.h>

CONST_STRING_DEF(UIStateMainMenu, UI_STATE_MAINMENU)

UIStateMainMenu::UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener): BaseStateDepricated(stateChangeListener), m_timer(this), m_cursorOn(false), m_editingGuestUsername(false) {}

UIStateMainMenu::~UIStateMainMenu() {}

void UIStateMainMenu::OnEnterState() {
    m_keyboardManager = IEngine::getEngine()->GetKeyboardManager();
    m_keyboardManager->RegisterKeyboardListener(this);
    
    m_mainMenuWidget = new MainMenuWidget(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
    m_mainMenuWidget->RegisterUsernameRefreshListener(this);
    
    LogOutGuestUser();
    
    m_mainMenuWidget->init();
    
    m_guestUsername = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    if (m_guestUsername.empty()) {
        m_guestUsername = GetRandomUsername();
    }
    HideCursor();
    
    if (IEngine::getEngine()->GetUserProvider()->IsLoggedIn() == false) {
        UILabel::onButtonStateChangedCallBack guestUsernameCallback;
        guestUsernameCallback.bind(this, &::UIStateMainMenu::OnPressed);
        m_mainMenuWidget->RegisterUsernameFocusCallback(guestUsernameCallback);
    }
    
    MainMenuWidget::onMenuItemSelectedCallBack callback;
    callback.bind(this, &UIStateMainMenu::onMainMenuItemSelected);
    m_mainMenuWidget->registerForMenuItemSelectedEvent(callback);
}

void UIStateMainMenu::OnExitState() {
    m_keyboardManager->UnregisterKeyboardListener();
    m_mainMenuWidget->release();
    delete(m_mainMenuWidget);
    BaseStateDepricated::OnExitState();
}

void UIStateMainMenu::onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem) {
    if (selectedItem == MainMenuWidget::LOAD_LOGIN_LEVEL) {
		ChangeState(UIStateAuthInput::UI_STATE_AUTH_INPUT);
	}

	else if (selectedItem == MainMenuWidget::LOAD_CHALLENGE_MENU_LEVEL) {
        UserProvider* userProvider = IEngine::getEngine()->GetUserProvider();
        if (userProvider->IsLoggedIn() == false) {
            User guestUser(m_guestUsername, "guest", UserAccessLevel::GUEST);
            userProvider->SetUser(guestUser);
        }
        
		ChangeState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
	}
}

void UIStateMainMenu::OnDeletePressed() {
    
    std::string currentUsername = m_guestUsername;
    if (currentUsername.empty() == false) {
        currentUsername.pop_back();
    }
    
    m_guestUsername = currentUsername;
    DisplayCursor();
    m_timer.ResetTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
}

void UIStateMainMenu::OnCharacterPressed(char c) {
    
    std::string currentUsername = m_guestUsername;
    
    if (currentUsername.size() < 20) {
        currentUsername += c;
        m_guestUsername = currentUsername;
    }
    
    DisplayCursor();
    m_timer.ResetTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
}

void UIStateMainMenu::OnEnterPressed() {
    m_editingGuestUsername = false;
    HideCursor();
    m_keyboardManager->DeactivateKeyboard();
    
    UILabel::onButtonStateChangedCallBack guestUsernameCallback;
    guestUsernameCallback.bind(this, &::UIStateMainMenu::OnPressed);
    m_mainMenuWidget->RegisterUsernameFocusCallback(guestUsernameCallback);
}

void UIStateMainMenu::OnTimerEvent(Timer::TimerType type) {
    switch(type) {
        case Timer::TimerType::CURSOR_BLINK_500_MS:
            if (IsEditingGuestUsername() == false) {
                break;
            }
            
            if (m_cursorOn) {
                HideCursor();
                m_cursorOn = false;
            } else {
                m_cursorOn = true;
                DisplayCursor();
            }
            
            break;
        default:
            break;
    }
}

bool UIStateMainMenu::IsEditingGuestUsername() {
    return m_editingGuestUsername;
}

void UIStateMainMenu::DisplayCursor() {
    std::string displayGuestUsername = m_guestUsername + "|";
    m_mainMenuWidget->SetGuestUsernameDisplay(displayGuestUsername);
}

void UIStateMainMenu::HideCursor() {
    m_mainMenuWidget->SetGuestUsernameDisplay(m_guestUsername);
}

void UIStateMainMenu::OnPressed(UITouchButton::ButtonState state) {
    m_keyboardManager->ActivateKeyboard();
    DisplayCursor();
    m_timer.RegisterTimer(Timer::TimerType::CURSOR_BLINK_500_MS);
    m_editingGuestUsername = true;
}

void UIStateMainMenu::LogOutGuestUser() {
    UserProvider* userProvider = IEngine::getEngine()->GetUserProvider();
    if (userProvider->IsLoggedIn() && userProvider->GetUser().GetAccessLevel() == UserAccessLevel::GUEST) {
        userProvider->LogOut();
    }
}

void UIStateMainMenu::OnUsernameRefresh() {
    m_guestUsername = GetRandomUsername();
    HideCursor();
    
    UILabel::onButtonStateChangedCallBack guestUsernameCallback;
    guestUsernameCallback.bind(this, &::UIStateMainMenu::OnPressed);
    m_mainMenuWidget->RegisterUsernameFocusCallback(guestUsernameCallback);
}
