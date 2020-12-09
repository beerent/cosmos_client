//
//  UITouchButton.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 2/12/13.
//
//

#include "Core/GUI/Components/UITouchButton.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Rendering/Renderables/UITexturedQuadRenderable.h"

const StringManager::StringID UITouchButton::CLASS_ID_UI_TOUCH_BUTTON = StringManager::getIDForString("CLASS_ID_UI_TOUCH_BUTTON");
const StringManager::StringID UITouchButton::UI_TUCH_BUTTON_DEPRESSED_VISUAL = StringManager::getIDForString("UI_TUCH_BUTTON_DEPRESSED_VISUAL");
const StringManager::StringID UITouchButton::UI_TUCH_BUTTON_PRESSED_VISUAL = StringManager::getIDForString("UI_TUCH_BUTTON_PRESSED_VISUAL");

UITouchButton::UITouchButton(PropertyCollection &propertyCollection, STRING_ID classTypeID):UIComponent(propertyCollection, classTypeID), m_enabled(true)
{
    init(propertyCollection);
}

UITouchButton::UITouchButton(PropertyCollection &propertyCollection):UIComponent(propertyCollection, CLASS_ID_UI_TOUCH_BUTTON), m_enabled(true)
{
    init(propertyCollection);
}

void UITouchButton::init(PropertyCollection &propertyCollection)
{
    m_state = DEPRESSED;
    m_pDePressedVisual = NULL;
    m_pPressedVisual = NULL;
    m_captureInput = true;
    
    int numberOfProperties = propertyCollection.getPropertyCount();
    for(int i=0; i < numberOfProperties; ++i)
    {
        const PropertyCollection::Property *property = propertyCollection.getPropertyAtIndex(i);
        if(property->m_name == UI_TUCH_BUTTON_DEPRESSED_VISUAL)
        {
            setDePressedVisual(property->m_data.StringIDValue);
        }
        else if(property->m_name == UI_TUCH_BUTTON_PRESSED_VISUAL)
        {
            setPressedVisual(property->m_data.StringIDValue);
        }
    }
    
    mToggleMode = false;
   
    DynamicObject::init();
}

void UITouchButton::onBeginRender(Renderer &renderer)
{
    switch (m_state)
    {
        case DEPRESSED:
            if(m_pDePressedVisual != NULL)
            {
                m_pDePressedVisual->onRender(renderer, renderer.getCurrentModelViewProjectionMatrix(), glm::vec3(m_width, m_height, 1.0f));
            }
            break;
        case PRESSED:
            if(m_pPressedVisual != NULL)
            {
                m_pPressedVisual->onRender(renderer, renderer.getCurrentModelViewProjectionMatrix(), glm::vec3(m_width, m_height, 1.0f));
            }
            break;
        default:
            break;
    }
    UIComponent::onBeginRender(renderer);
}

void UITouchButton::registerForButtonEvent(ButtonState state, onButtonStateChangedCallBack callBack)
{
    switch (state)
    {
        case PRESSED:
            m_onPressedListeners.push_back(callBack);
            break;
        case DEPRESSED:
            m_onReleasedListeners.push_back(callBack);
            break;
        default:
            break;
    }
}

void UITouchButton::unregisterForButtonEvent(ButtonState state, onButtonStateChangedCallBack callBack)
{
    switch (state)
    {
        case PRESSED:
            m_onPressedListeners.remove(callBack);
            break;
        case DEPRESSED:
            m_onReleasedListeners.remove(callBack);
            break;
        default:
            break;
    }
}

void UITouchButton::setPressedVisual(StringManager::StringID id)
{
    RenderableManager *renderableManager = RenderableManager::getInstance();
    m_pPressedVisual = renderableManager->getRenderable(id);
}

void UITouchButton::setDePressedVisual(StringManager::StringID id)
{
    RenderableManager *renderableManager = RenderableManager::getInstance();
    m_pDePressedVisual = renderableManager->getRenderable(id);
}

void UITouchButton::handleOnPress() {
	if (false == m_enabled) {
		return;
	} 

    if(mToggleMode == false)
    {
        m_state = PRESSED;
    }
    else
    {
        if(m_state == PRESSED)
        {
            m_state = DEPRESSED;
        }
        else
        {
            m_state = PRESSED;
        }
    }
    
    std::list<onButtonStateChangedCallBack>::iterator it = m_onPressedListeners.begin();
    while(it != m_onPressedListeners.end())
    {
        (*it)(m_state);
        it++;
    }
        
    UIComponent::handleOnPress();
}

void UITouchButton::handleOnRelease() {
	if (false == m_enabled) {
		return;
	}

    if(mToggleMode == false) {
        m_state = DEPRESSED;
    }
    
    std::list<onButtonStateChangedCallBack>::iterator it = m_onReleasedListeners.begin();
    while(it != m_onReleasedListeners.end() && !m_onReleasedListeners.empty()) {
        (*it)(m_state);
        it++;
    }

    UIComponent::handleOnRelease();
}

void UITouchButton::DisableButton() {
	m_enabled = false;
}

void UITouchButton::EnableButton() {
	m_enabled = true;
}
