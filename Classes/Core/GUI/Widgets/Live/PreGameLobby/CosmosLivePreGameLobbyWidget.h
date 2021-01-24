#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>
#include <Core/GameLogic/Live/Chat/CosmosLiveChat.h>

class CosmosLivePreGameLobbyWidget {
public:
    enum MenuItems { LOAD_MAIN_MENU };
    typedef fastdelegate::FastDelegate1<MenuItems> onMenuItemSelectedCallBack;
    
    CosmosLivePreGameLobbyWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init();
    void Release();
    
    void SetVisible(bool visible);
    
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
    
    UILabel* m_currentUsername;
};
