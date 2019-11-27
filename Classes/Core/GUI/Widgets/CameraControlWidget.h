//
//  CameraControlWidget.h
//  Projectios1
//
//  Created by Kamil Konecko on 3/23/13.
//
//

#ifndef Projectios1_CameraControlWidget_h
#define Projectios1_CameraControlWidget_h

#include "Core/GUI/Components/UITouchButton.h"

class ActiveCameraController;
class UIComponentFactory;
class UIComponent;

class CameraControlWidget
{
private:
    UIComponent *m_movementGroup;
    UIComponent *m_rotationGroup;

    ActiveCameraController *m_activeCameraController;
    
public:
    void init(ActiveCameraController *activeCameraController, UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
    void release();
    
    void onForwardButtonStateChanged(UITouchButton::ButtonState state);
    void onBackwardButtonStateChanged(UITouchButton::ButtonState state);
    void onUpButtonStateChanged(UITouchButton::ButtonState state);
    void onDownButtonStateChanged(UITouchButton::ButtonState state);
    void onLeftButtonStateChanged(UITouchButton::ButtonState state);
    void onRightButtonStateChanged(UITouchButton::ButtonState state);
    
    void onRotateUpButtonStateChanged(UITouchButton::ButtonState state);
    void onRotateDownButtonStateChanged(UITouchButton::ButtonState state);
    void onRotateLeftButtonStateChanged(UITouchButton::ButtonState state);
    void onRotateRightButtonStateChanged(UITouchButton::ButtonState state);

    void onRollRightButtonStateChanged(UITouchButton::ButtonState state);
    void onRollLeftButtonStateChanged(UITouchButton::ButtonState state);

private:
    void setUpMovementButtons(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
    void setUPRotationButtons(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);    
};

#endif
