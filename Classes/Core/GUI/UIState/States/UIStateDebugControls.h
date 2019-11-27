//
//  UIStateDebugControls.h
//  Projectios2
//
//  Created by Kamil Konecko on 1/24/15.
//
//

#ifndef __Projectios2__UIStateDebugControls__
#define __Projectios2__UIStateDebugControls__

#include <stdio.h>
#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Widgets/CameraControlWidget.h"
#include "Core/GUI/Widgets/MainMenuWidget.h"
#include "Core/GUI/Components/UITouchButton.h"

class UIStateDebugControls : public BaseStateDepricated
{
public:
    UIStateDebugControls(IStateChanageListenerDepricated* stateChangeListener): BaseStateDepricated(stateChangeListener){};
    CONST_STRING_DEC(UI_STATE_DEBUG_CONTROLS)
    
    virtual void OnEnterState();
    virtual void OnExitState();
    
    virtual STRING_ID GetStateID(){return UI_STATE_DEBUG_CONTROLS;}
    
private:
    
    CameraControlWidget* m_cameraControlWidget;
    MainMenuWidget* m_mainMenuWidget;
    UITouchButton *m_backButton;
    UITouchButton *mFireButton;

    void onFireProjectile(UITouchButton::ButtonState state);
    void onDebugMenuBackButtonStateChanged(UITouchButton::ButtonState state);
};

#endif /* defined(__Projectios2__UIStateDebugControls__) */
