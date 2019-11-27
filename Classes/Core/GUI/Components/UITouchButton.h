#pragma once

#include "Core/GUI/Components/UIComponent.h"
#include "Core/OpenSource/FastDelegate.h"
#include <list>


class UITouchButton: public UIComponent {
public:
    static const StringManager::StringID CLASS_ID_UI_TOUCH_BUTTON;
    enum ButtonState{DEPRESSED = 0, PRESSED};
    typedef fastdelegate::FastDelegate1<ButtonState> onButtonStateChangedCallBack;

	UITouchButton(PropertyCollection &propertyCollection);
	UITouchButton(PropertyCollection &propertyCollection, STRING_ID classTypeID);
	void DisableButton();
	void EnableButton();
	void setToggleMode(bool toggle) { mToggleMode = toggle; };
	bool getToggleState() { return m_state == PRESSED; };
	void setToggleState(bool toggled) { m_state = toggled == true ? PRESSED : DEPRESSED; };

	void init(PropertyCollection &propertyCollection);
	virtual void onBeginRender(Renderer& renderer);
	void setPressedVisual(StringManager::StringID id);
	void setDePressedVisual(StringManager::StringID id);
	void registerForButtonEvent(ButtonState state, onButtonStateChangedCallBack callBack);
	void unregisterForButtonEvent(ButtonState state, onButtonStateChangedCallBack callBack);

protected:
	void handleOnPress();
	void handleOnRelease();

private:
    static const StringManager::StringID UI_TUCH_BUTTON_DEPRESSED_VISUAL;
    static const StringManager::StringID UI_TUCH_BUTTON_PRESSED_VISUAL;
    RenderableObj *m_pDePressedVisual;
    RenderableObj *m_pPressedVisual;
    ButtonState m_state;

    bool mToggleMode;
	bool m_enabled;
    
    std::list<onButtonStateChangedCallBack> m_onPressedListeners;
    std::list<onButtonStateChangedCallBack> m_onReleasedListeners;  
};