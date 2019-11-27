//
//  DebugOverLayWidget.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 2/11/15.
//
//

#include <stdio.h>
#include "DebugOverLayWidget.h"
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "IEngine.h"

void DebugOverLayWidget::init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent)
{
    mDebugToggleButton = static_cast<UITouchButton*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UITuchDeBugButtonArchetype")));
    
    mDebugToggleButton->setAnchor(UIComponent::ANCHOR_LEFT);
    mDebugToggleButton->setWidth(50);
    mDebugToggleButton->setHeight(50);
    mDebugToggleButton->setX(0);
    mDebugToggleButton->setToggleMode(true);
    parentComponent->addChild(mDebugToggleButton);
    
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &DebugOverLayWidget::onToggleDebugModeButton);
    mDebugToggleButton->registerForButtonEvent(UITouchButton::DEPRESSED,  callBack);
    
    
    mFrameRateLabel = static_cast<UILabel*>(uiComponentFactory->createUIComponent(StringManager::getIDForString("UILabelArchetype")));
    
    
    mFrameRateLabel->setAnchor(UIComponent::ANCHOR_TOP_LEFT);
    mFrameRateLabel->setWidth(300);
    mFrameRateLabel->setHeight(25);
    mFrameRateLabel->setX(25.0f);
    mFrameRateLabel->setY(15.0f);
    mFrameRateLabel->setFontSize(30.0f);
    mFrameRateLabel->setDropShadow(true);
    mFrameRateLabel->setDropShadowColor(50.0f/255.0f, 50.0f/255.0f, 50.0f/255.0f, 0.5f);
    mFrameRateLabel->setColor(200.0f/255.0f, 200.0f/255.0f, 200.0f/255.0f);
    
    STRING_ID textStringID = StringManager::getIDForString("Calculating");
    mFrameRateLabel->setText(StringManager::getStringFromID(textStringID)->c_str());
    
    parentComponent->addChild(mFrameRateLabel);
    
    m_onDeltaTimeCallBack.bind(this, &DebugOverLayWidget::onDeltaTimeUpdate);
    IEngine::getEngine()->registerForDeltaTimeEvents(m_onDeltaTimeCallBack);
    
    for(unsigned i = 0; i < FRAME_SAMPLE_COUNT; ++i)
    {
        mFrameSamples[i] = 0;
    }
}

void DebugOverLayWidget::onToggleDebugModeButton(UITouchButton::ButtonState state)
{
    IEngine::getEngine()->toggleDebugMenu();
}

void DebugOverLayWidget::release()
{
    mDebugToggleButton->release();
    delete(mDebugToggleButton);
    
    mFrameRateLabel->release();
    delete(mFrameRateLabel);
    
    IEngine::getEngine()->unregisterForDeltaTimeEvents(m_onDeltaTimeCallBack);
}

void DebugOverLayWidget::onDeltaTimeUpdate(float deltaTime)
{
    const long currentFrameCount = IEngine::getEngine()->getCurrentFrameCount();
    mFrameSamples[currentFrameCount % FRAME_SAMPLE_COUNT] = deltaTime;
    
    if(currentFrameCount % FRRAME_RATE_COUNTER_UPDATE_NUMBER_OF_FRAMES)
    {
        float sampleAverage = 0;
        for(unsigned i = 0; i < FRAME_SAMPLE_COUNT; ++i)
        {
            sampleAverage += mFrameSamples[i];
        }
        
        sampleAverage /= static_cast<float>(FRAME_SAMPLE_COUNT);
        if(sampleAverage != 0)
        {
            char buff[100];
            sprintf(buff, "%d", static_cast<int>(1.0 / sampleAverage));
            STRING_ID textStringID = StringManager::getIDForString(buff);
            mFrameRateLabel->setText(StringManager::getStringFromID(textStringID)->c_str());

        }
    }
}