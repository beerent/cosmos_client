#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"
#include <Core/GUI/Widgets/User/Username/UsernameEditWidget.h>
#include <Core/Net/IRestReceiver.h>
#include <Core/Util/SimpleTimer.h>
#include <vector>
#include <string>

class UIStateMainMenu : public BaseStateDepricated, IEditUsernameCloser, IRestReceiver, Timer::SimpleTimerListener {
public:
    UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener);
    ~UIStateMainMenu();
    
    CONST_STRING_DEC(UI_STATE_MAINMENU)
    
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID(){ return UI_STATE_MAINMENU; }

    virtual void CloseEditUsername(User newUser);
    virtual void OnTimerEvent(Timer::TimerType type);
    virtual void RestReceived(const std::string& rest);
private:
    MainMenuWidget* m_mainMenuWidget;
    UsernameEditWidget* m_usernameEditWidget;
    std::string m_messagesString;
    int m_currentMessageScrollIndex;
    
    IRestConnector* m_restConnector;
    
    Timer::SimpleTimer m_timer;
    
    void LogOutGuestUser();
    void SendGetMessagesRequest();
    std::vector<std::string> JsonToMessages(const json11::Json& json);
    
    void AdvanceMessageScroll();
    
    void RegisterTimers();
    void DeregisterTimers();
    
    void BuildMessagesString(std::vector<std::string> messages);
    
    void OnUsernamePressed(UITouchButton::ButtonState state);
    void onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem);
    
    void InitUsernameEditWidget();
    void ReleaseUsernameEditWidget();
    
    void HandleNewUser(User newUser);
};
