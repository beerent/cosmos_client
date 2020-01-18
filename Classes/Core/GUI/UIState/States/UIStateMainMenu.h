#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"
#include <Core/Util/SimpleTimer.h>
#include <Core/Keyboard/IKeyboardListener.h>

class UIStateMainMenu : public BaseStateDepricated, IKeyboardListener, Timer::SimpleTimerListener, IUsernameRefreshListener  {
public:
    UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener);
    ~UIStateMainMenu();
    CONST_STRING_DEC(UI_STATE_MAINMENU)
    
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual void OnDeletePressed();
    virtual void OnCharacterPressed(char c);
    virtual void OnEnterPressed();
    
    virtual void OnTimerEvent(Timer::TimerType type);
    
    virtual STRING_ID GetStateID(){return UI_STATE_MAINMENU;}
    
    virtual void OnUsernameRefresh();
    
private:
    
    MainMenuWidget* m_mainMenuWidget;
    
    Timer::SimpleTimer m_timer;
    KeyboardManager* m_keyboardManager;
    
    std::string m_guestUsername;
    
    bool m_cursorOn;
    bool m_editingGuestUsername;
    
    bool IsEditingGuestUsername();
    void DisplayCursor();
    void HideCursor();
    void OnPressed(UITouchButton::ButtonState state);
    
    void LogOutGuestUser();
    
    void onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem);

};
