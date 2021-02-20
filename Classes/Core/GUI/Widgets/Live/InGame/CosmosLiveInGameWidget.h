#pragma once

#include <Core/GUI/Components/UIComponentFactory.h>

class CosmosLiveInGameWidget {
public:
    enum MenuItems { LOAD_MAIN_MENU };
    typedef fastdelegate::FastDelegate1<MenuItems> onMenuItemSelectedCallBack;
    
    CosmosLiveInGameWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

    void Init();
    void Release();
    
    void SetVisible(bool visible);
};
