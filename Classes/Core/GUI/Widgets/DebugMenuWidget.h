//
//  DebugMenuWidget.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/3/14.
//
//

#ifndef Projectios1_DebugMenuWidget_h
#define Projectios1_DebugMenuWidget_h
#include "Core/GUI/Components/UITouchButton.h"
class UIComponentFactory;
class UIComponent;

class DebugMenuWidget
{
private:
    UIComponent *mSettingsGroup;
    
public:
    void init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
    void release();
    void onSettingShowPhysicsDebugInfoButtonStateChanged(UITouchButton::ButtonState state);
    void onSettingShowPhysicsVolumeObjectsButtonStateChanged(UITouchButton::ButtonState state);
    void onSettingShowObjectAxisButtonStateChanged(UITouchButton::ButtonState state);
    void onSettingShowObjectNamesButtonStateChanged(UITouchButton::ButtonState state);

private:   
    void setupSettingsButtons(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);

};
#endif
