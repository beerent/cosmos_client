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
    
    UIComponentFactory* m_uiComponentFactory;
    UIComponent* m_parentComponent;

    UILabel* m_loading;
};
