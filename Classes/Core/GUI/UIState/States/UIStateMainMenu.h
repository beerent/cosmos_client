#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"
#include <Core/GUI/Widgets/User/Username/UsernameEditWidget.h>

class UIStateMainMenu : public BaseStateDepricated, IEditUsernameCloser {
public:
    UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener);
    ~UIStateMainMenu();
    
    CONST_STRING_DEC(UI_STATE_MAINMENU)
    
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID(){ return UI_STATE_MAINMENU; }

    virtual void CloseEditUsername(User newUser);
private:
    MainMenuWidget* m_mainMenuWidget;
    UsernameEditWidget* m_usernameEditWidget;
    
    void LogOutGuestUser();
    
    void OnUsernamePressed(UITouchButton::ButtonState state);
    void onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem);
    
    void InitUsernameEditWidget();
    void ReleaseUsernameEditWidget();
    
    void HandleNewUser(User newUser);
};
