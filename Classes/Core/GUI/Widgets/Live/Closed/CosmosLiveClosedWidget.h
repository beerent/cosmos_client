#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>

class CosmosLiveClosedWidget {
public:
    enum MenuItems { LOAD_MAIN_MENU };
    typedef fastdelegate::FastDelegate1<MenuItems> onMenuItemSelectedCallBack;
    
    CosmosLiveClosedWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init();
    void Release();
    
    void RegisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback);
    void UnregisterForChallengeMenuItemSelectedEvent(onMenuItemSelectedCallBack callback);
    
private:
    void AddProfileWindow();
    void AddProfileFrame();
    void AddHomeButton();
    void AddTitleButton();
    void AddMessageButton();
    void AddUsername();
    
    void OnHomePressed(UITouchButton::ButtonState state);
    
    onMenuItemSelectedCallBack m_onHomeMenuItemSelectedListener;
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    
    UIComponent* m_profileWindow;
    UIComponent* m_profileFrame;
    UILabel* m_title;
    UILabel* m_message;
    UILabel* m_home;
    
    UILabel* m_currentUsername;
};
