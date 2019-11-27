//
//  CameraControlWidget.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 3/23/13.
//
//

#include "Core/GUI/Widgets/CameraControlWidget.h"
#include "Core/Input/ActiveCameraController.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/GUI/Components/UITouchButton.h"

void CameraControlWidget::init(ActiveCameraController *activeCameraController, UIComponentFactory *uiComponentFactory, UIComponent *parentComponent)
{
    m_activeCameraController = activeCameraController;
    setUpMovementButtons(uiComponentFactory, parentComponent);
    setUPRotationButtons(uiComponentFactory, parentComponent);

}

void CameraControlWidget::setUpMovementButtons(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent)
{
    m_movementGroup = uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_movementGroup->setAnchor(UIComponent::ANCHOR_BOTTOM_LEFT);
    m_movementGroup->setWidth(150);
    m_movementGroup->setHeight(150);
    m_movementGroup->setX(50);
    m_movementGroup->setY(50);
    
    parentComponent->addChild(m_movementGroup);
    
    //KKonecko: Forward
    
    UITouchButton* component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavMoveUpButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_CENTER);
    component->setWidth(50);
    component->setHeight(50);
    
    m_movementGroup->addChild(component);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &CameraControlWidget::onForwardButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED,  callBack);
    
    
    //KKonecko: BackWard
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavMoveDownButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_BOTTOM_CENTER);
    component->setWidth(50);
    component->setHeight(50);
    
    m_movementGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onBackwardButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: UP
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavMoveUpButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_RIGHT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_movementGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onUpButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED,  callBack);
    
    //KKonecko: Down
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavMoveDownButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_BOTTOM_RIGHT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_movementGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onDownButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: Left
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavMoveLeftButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_LEFT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_movementGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onLeftButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: Right
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavMoveRightButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_RIGHT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_movementGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onRightButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);    
}

void CameraControlWidget::setUPRotationButtons(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent)
{
    m_rotationGroup = uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
    m_rotationGroup->setAnchor(UIComponent::ANCHOR_BOTTOM_RIGHT);
    m_rotationGroup->setWidth(150);
    m_rotationGroup->setHeight(150);
    m_rotationGroup->setX(50);
    m_rotationGroup->setY(50);
    
    parentComponent->addChild(m_rotationGroup);
    
    //KKonecko: up
    
    UITouchButton* component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavRotateUpButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_CENTER);
    component->setWidth(50);
    component->setHeight(50);
    
    m_rotationGroup->addChild(component);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &CameraControlWidget::onRotateUpButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED,  callBack);
    
    
    //KKonecko: down
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavRotateDownButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_BOTTOM_CENTER);
    component->setWidth(50);
    component->setHeight(50);
    
    m_rotationGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onRotateDownButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: Left
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavRotateLeftButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_LEFT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_rotationGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onRotateLeftButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: Right
    
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavRotateRightButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_RIGHT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_rotationGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onRotateRightButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: Roll Left
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavRollLeftButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_rotationGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onRollLeftButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    //KKonecko: Roll Right
    component = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UINavRollRightButtonArchetype")));
    
    component->setAnchor(UIComponent::ANCHOR_TOP_RIGHT);
    component->setWidth(50);
    component->setHeight(50);
    
    m_rotationGroup->addChild(component);
    
    callBack.bind(this, &CameraControlWidget::onRollRightButtonStateChanged);
    component->registerForButtonEvent(UITouchButton::PRESSED,  callBack);
    component->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
}

void CameraControlWidget::release()
{
    m_movementGroup->release();
    delete(m_movementGroup);
    m_rotationGroup->release();
    delete(m_rotationGroup);
}

void CameraControlWidget::onForwardButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraForwardUp();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraForwardDown();
            break;
    }
}

void CameraControlWidget::onBackwardButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraBackwardUp();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraBackwardDown();
            break;
    }
}

void CameraControlWidget::onUpButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraUpDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraUpPressed();
            break;
    }
}

void CameraControlWidget::onDownButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraDownDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraDownPressed();
            break;
    }
}

void CameraControlWidget::onLeftButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraLeftUp();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraLeftDown();
            break;
    }
}

void CameraControlWidget::onRightButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRightUp();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRightDown();
            break;
    }
}

void CameraControlWidget::onRotateUpButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRotateUpDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRotateUpPressed();
            break;
    }
}

void CameraControlWidget::onRotateDownButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRotateDownDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRotateDownPressed();
            break;
    }
}

void CameraControlWidget::onRotateLeftButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRotateLeftDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRotateLeftPressed();
            break;
    }
}

void CameraControlWidget::onRotateRightButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRotateRightDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRotateRightPressed();
            break;
    }
}

void CameraControlWidget::onRollRightButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRollRightDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRollRightPressed();
            break;
    }
}

void CameraControlWidget::onRollLeftButtonStateChanged(UITouchButton::ButtonState state)
{
    switch (state)
    {
        case UITouchButton::DEPRESSED:
            m_activeCameraController->onCameraRollLeftDepressed();
            break;
        case UITouchButton::PRESSED:
            m_activeCameraController->onCameraRollLeftPressed();
            break;
    }
}