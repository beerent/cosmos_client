//
//  UIStateDebugControls.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 1/24/15.
//
//

#include "Core/GUI/UIState/States/UIStateDebugControls.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"
#include "Core/GameState/GameStateMachine.h"
#include "Core/GUI/Components/UIComponent.h"
#include "Core/Debug/DebugLogic.h"
#include "Core/GameState/States/GameMainMenuState.h"
#include "Core/Input/ActiveCameraController.h"

CONST_STRING_DEF(UIStateDebugControls, UI_STATE_DEBUG_CONTROLS)

void UIStateDebugControls::OnEnterState()
{
   
    UIComponent *root = IEngine::getEngine()->getUIRoot();

    m_backButton = static_cast<UITouchButton*>(UIComponentFactory::getInstance()->createUIComponent(StringManager::getIDForString("UIReturnToMainMenuButtonArchetype")));
    
    m_backButton->setAnchor(UIComponent::ANCHOR_TOP_RIGHT);
    m_backButton->setWidth(50);
    m_backButton->setHeight(50);
    m_backButton->setY(50);
    m_backButton->setX(50);
    
    root->addChild(m_backButton);
    UITouchButton::onButtonStateChangedCallBack onButtonStateChangedCallBack;
    onButtonStateChangedCallBack.bind(this, &UIStateDebugControls::onDebugMenuBackButtonStateChanged);
    m_backButton->registerForButtonEvent(UITouchButton::DEPRESSED, onButtonStateChangedCallBack);

    m_cameraControlWidget = new CameraControlWidget();
    m_cameraControlWidget->init(IEngine::getEngine()->getActiveCameraController(), UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());
	IEngine::getEngine()->getActiveCameraController()->setAllowUserControl(true);

    mFireButton = static_cast<UITouchButton*>(UIComponentFactory::getInstance()->createUIComponent(StringManager::getIDForString("UIFireButtonArchetype")));
    
    mFireButton->setAnchor(UIComponent::ANCHOR_RIGHT);
    mFireButton->setWidth(100);
    mFireButton->setHeight(100);
    mFireButton->setY(0);
    mFireButton->setX(50);
    
    root->addChild(mFireButton);
    UITouchButton::onButtonStateChangedCallBack onFireProjectile;
    onButtonStateChangedCallBack.bind(this, &UIStateDebugControls::onFireProjectile);
    mFireButton->registerForButtonEvent(UITouchButton::DEPRESSED, onButtonStateChangedCallBack);
}

void UIStateDebugControls::OnExitState()
{
	IEngine::getEngine()->getActiveCameraController()->setAllowUserControl(false);
    mFireButton->release();
    m_cameraControlWidget->release();
    delete m_cameraControlWidget;
    delete mFireButton;
     m_backButton->release();
    delete m_backButton;
    BaseStateDepricated::OnExitState();
}

void UIStateDebugControls::onFireProjectile(UITouchButton::ButtonState state)
{
    DebugLogic::getInstance()->fireProjectileFromCamera(STRING_TO_ID("techionCannonArchetype"));
}

void UIStateDebugControls::onDebugMenuBackButtonStateChanged(UITouchButton::ButtonState state)
{
    //switch (state)
    //{
        //case UITouchButton::DEPRESSED:
            //GameStateMachine::GetInstance()->PushStateParameters(GamePlayState::LEVEL_NAME, StringManager::getIDForString("MainMenuLevel"));
            //GameStateMachine::GetInstance()->SetState(GameMainMenuState::MAIN_MENU_STATE);
            //break;
    //}
}
