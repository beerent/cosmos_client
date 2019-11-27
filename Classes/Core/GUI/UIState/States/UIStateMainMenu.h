#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"

class UIStateMainMenu : public BaseStateDepricated {
public:
    UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener): BaseStateDepricated(stateChangeListener){};
    CONST_STRING_DEC(UI_STATE_MAINMENU)
    
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID(){return UI_STATE_MAINMENU;}
    
private:
    
    MainMenuWidget* m_mainMenuWidget;
    
    void onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem);

};