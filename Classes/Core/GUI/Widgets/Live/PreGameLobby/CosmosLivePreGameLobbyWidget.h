#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/GameLogic/Live/Chat/CosmosLiveChat.h>
#include <Core/Keyboard/IKeyboardListener.h>
#include <Core/Util/SimpleTimer.h>

class CosmosLivePreGameLobbyWidget : public IKeyboardListener, Timer::SimpleTimerListener {
public:
    enum MenuItems { LOAD_MAIN_MENU };
    typedef fastdelegate::FastDelegate1<MenuItems> onMenuItemSelectedCallBack;
    
    CosmosLivePreGameLobbyWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init();
    void Release();
    void SetVisible(bool visible);
    
    virtual void OnDeletePressed();
    virtual void OnCharacterPressed(char c);
    virtual void OnEnterPressed();
    
    virtual void OnTimerEvent(Timer::TimerType type);
    
    void RegisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback);
    void UnregisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback);
    
    void UpdateActiveUsers(int users);
    void UpdateTimeUntilGametime(double secondsRemaining);
    std::string SecondsToMinutesSecondsString(double secondsRemaining) const;
    void UpdateChats(const std::vector<CosmosLiveChat>& chats);
    
private:
    void AddProfileWindow();
    void AddProfileFrame();
    void AddHomeButton();
    void AddTitleButton();
    void AddActiveUsers();
    void AddTimeUntilGametime();
    void AddUsername();
    void AddChats();
    void UpdateChat(const CosmosLiveChat& chat, int position);
    
    void OnHomePressed(UITouchButton::ButtonState state);
    
    void AddAddChatButton();
    void OnAddChatPressed(UITouchButton::ButtonState state);
    void OnSendChat();
    
    void MoveFrameUp();
    void MoveFrameDown();
    void DisplayChatBox();
    void DisplayCursor();
    void HideCursor();
    
    void HideMenuBar();
    void ShowMenuBar();
    
    onMenuItemSelectedCallBack m_onHomeMenuItemSelectedListener;
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    
    UIComponent* m_profileWindow;
    UIComponent* m_profileFrame;
    UILabel* m_title;
    UILabel* m_activeUsers;
    UILabel* m_timeUntilGametime;
    UILabel* m_home;

    UILabel* m_chat0;
    UILabel* m_chat1;
    UILabel* m_chat2;
    UILabel* m_chat3;
    UILabel* m_chat4;
    UILabel* m_chat5;
    UILabel* m_chat6;
    UILabel* m_chat7;
    UILabel* m_chat8;
    UILabel* m_chat9;
    
    UILabel* m_addChatButton;
    std::string m_chat;
    UILabel* m_chatText;
    bool m_cursorOn;
    bool m_editingChat;
    KeyboardManager* m_keyboardManager;
    
    Timer::SimpleTimer m_timer;
    
    UILabel* m_currentUsername;
};
