#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>

class LoadingWidget {
public:
    
    LoadingWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init();
    void Release();
    void SetVisible(bool visible);
    
private:
    void AddLoadingMessage();
    void AddProfileWindow();
    void AddProfileFrame();
    void AddTitle();
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;
    UIComponent* m_loadingWindow;
    UIComponent* m_loadingFrame;

    UILabel* m_title;
    UILabel* m_loading;
};
