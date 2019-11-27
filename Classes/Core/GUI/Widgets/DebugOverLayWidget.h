//
//  DebugOverLayWidget.h
//  Projectios2
//
//  Created by Kamil Konecko on 2/11/15.
//
//

#ifndef Projectios2_DebugOverLayWidget_h
#define Projectios2_DebugOverLayWidget_h

#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include "IEngine.h"

class UIComponentFactory;
class UIComponent;

class DebugOverLayWidget
{
public:
    static const unsigned int FRAME_SAMPLE_COUNT = 100;
    static const unsigned int FRRAME_RATE_COUNTER_UPDATE_NUMBER_OF_FRAMES = 10;
    
    void init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
    void release();

private:
    float mFrameSamples[FRAME_SAMPLE_COUNT];
    UITouchButton *mDebugToggleButton;
    UILabel *mFrameRateLabel;
    IEngine::onDeltaTimeEventCallBack m_onDeltaTimeCallBack;
    
    void onToggleDebugModeButton(UITouchButton::ButtonState state);
    void onDeltaTimeUpdate(float deltaTime);
};
#endif
