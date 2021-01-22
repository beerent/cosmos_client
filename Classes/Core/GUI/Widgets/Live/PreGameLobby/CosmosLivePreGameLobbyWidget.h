#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>

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
    
private:
    void AddProfileWindow();
    void AddProfileFrame();
    void AddHomeButton();
    void AddTitleButton();
    void AddActiveUsers();
    void AddUsername();
    
    void OnHomePressed(UITouchButton::ButtonState state);
    
    onMenuItemSelectedCallBack m_onHomeMenuItemSelectedListener;
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    
    UIComponent* m_profileWindow;
    UIComponent* m_profileFrame;
    UILabel* m_title;
    UILabel* m_activeUsers;
    UILabel* m_home;
    
    UILabel* m_currentUsername;
};
