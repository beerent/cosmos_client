#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/GameLogic/Live/Chat/CosmosLiveChat.h>
#include <Core/Keyboard/IKeyboardListener.h>
#include <Core/Util/SimpleTimer.h>

class ICosmosLiveChatReceiver {
public:
    virtual void ChatReceived(const std::string& chat) = 0;
};

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
    
    void RegisterCosmosLiveChatReceiver(ICosmosLiveChatReceiver* receiver);
    void DeregisterCosmosLiveChatReceiver();
    
    void UpdateActiveUsers(int users);
    void UpdateTimeUntilGametime(double secondsRemaining);
    std::string SecondsToMinutesSecondsString(double secondsRemaining) const;
    void UpdateChats(const std::vector<CosmosLiveChat>& chats);
    
private:
    void AddChatWindow();
    void AddChatFrame();
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
    void SendChat();
    
    void SetCursorTimer();
    void ResetCursorTimer();
    void UnsetCursorTimer();
    
    void MoveFrameUp();
    void MoveFrameDown();
    void DisplayChatBox();
    void DisplayCursor();
    void HideCursor();
    std::string GetChatLengthAsString() const;
    std::string getChatString(const CosmosLiveChat& chat, bool usePadding) const;
    void UpdateChatFrameWidth(const std::vector<CosmosLiveChat>& chats);
    void UpdateChatMessageWidth(const std::vector<CosmosLiveChat>& chats);
    
    void HideMenuBar();
    void ShowMenuBar();
    
    std::string CreateTimeAgoString(int secondsAgo) const;
    
    glm::vec3 getTextColorFromChatMessage(const std::string& chat) const;

    onMenuItemSelectedCallBack m_onHomeMenuItemSelectedListener;
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    
    UIComponent* m_preGameLobbyWindow;
    UIComponent* m_chatFrame;
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
    int m_chatWidth;
    
    UILabel* m_addChatButton;
    std::string m_chat;
    UILabel* m_chatText;
    bool m_cursorOn;
    bool m_editingChat;
    KeyboardManager* m_keyboardManager;
    
    Timer::SimpleTimer m_timer;
    
    UILabel* m_currentUsername;
    
    ICosmosLiveChatReceiver* m_cosmosLiveChatReceiver;
};
