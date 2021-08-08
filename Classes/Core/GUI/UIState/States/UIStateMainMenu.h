#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"
#include <Core/GUI/Widgets/User/Username/UsernameEditWidget.h>
#include <Core/GameLogic/Alert/Alert.h>
#include <Core/Net/IRestReceiver.h>
#include <Core/Util/SimpleTimer.h>
#include <vector>
#include <string>

class UIStateMainMenu : public BaseStateDepricated, IPopupCloser, IEditUsernameCloser, IRestReceiver, Timer::SimpleTimerListener {
public:
    UIStateMainMenu(IStateChanageListenerDepricated* stateChangeListener);
    ~UIStateMainMenu();
    
    CONST_STRING_DEC(UI_STATE_MAINMENU)
    
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID(){ return UI_STATE_MAINMENU; }

    virtual void ClosePopup(const std::string& key);
    virtual void CloseEditUsername(User newUser);
    virtual void OnTimerEvent(Timer::TimerType type);
    virtual void RestReceived(const std::string& rest);
private:
    MainMenuWidget* m_mainMenuWidget;
    UsernameEditWidget* m_usernameEditWidget;
    PopupWidget* m_popupWidget;
    std::vector<std::string> m_messages;
    std::string m_messagesRequestKey;
    int m_currentMessageIndex;
    int m_currentMessageScrollIndex;
    bool m_easterEggPending;
    
    IRestConnector* m_restConnector;
    
    Timer::SimpleTimer m_timer;
    
    void LogOutGuestUser();
    void SendGetMessagesRequest();
    void SendGetAlertRequest();
    std::vector<std::string> JsonToMessages(const json11::Json& json);
    Alert JsonToAlert(const json11::Json& json);
    
    void AdvanceMessageScroll();
    void AdvanceMessageIndex();

    void DisplayPopup(const Alert& alert);
    void InitPopup(const Alert& alert);
    void ReleasePopup();
    
    void RegisterTimers();
    void DeregisterTimers();
    
    void OnUsernamePressed(UITouchButton::ButtonState state);
    void onMainMenuItemSelected(MainMenuWidget::MainMenuItems selectedItem);
    
    void InitUsernameEditWidget();
    void ReleaseUsernameEditWidget();
    
    bool EasterEggIsActive() const;
    bool ShouldActivateEasterEgg() const;
    void ActivateEasterEgg();
    
    bool AlertHasBeenRead(const std::string& alertKey) const;
    bool IsValidAlert(const Alert& alert) const;
    void MarkAlertAsRead(const std::string& alertKey);
    
    bool TutorialHasBeenRead() const;
    void DisplayTutorial();
    
    void HandleNewUser(User newUser);
};
