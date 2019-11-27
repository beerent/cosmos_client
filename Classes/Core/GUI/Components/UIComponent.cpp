//
//  UIComponent.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 2/12/13.
//
//

#include "Core/GUI/Components/UIComponent.h"
#include <sstream>
#include "Core/OpenSource/Vector.hpp"
#include "Core/Rendering/RenderableManager.h"

CONST_STRING_DEF(UIComponent, CLASS_ID_UI_COMPONENT)
CONST_STRING_DEF(UIComponent, UI_COMPONENT_HEIGHT)
CONST_STRING_DEF(UIComponent, UI_COMPONENT_WIDTH)
CONST_STRING_DEF(UIComponent, VISUAL)

int UIComponent::m_uniqueID = 0;

StringManager::StringID UIComponent::getID()
{
    if(m_id == -1)
    {
        std::ostringstream conversionNumber;
        conversionNumber << m_uniqueID;
        m_id = StringManager::getIDForString("UIComponent" + conversionNumber.str());
        m_uniqueID ++;
    }
    
    return m_id;
}

UIComponent::UIComponent(PropertyCollection &propertyCollection):DynamicObject(CLASS_ID_UI_COMPONENT)
{
    init(propertyCollection);
}

UIComponent::UIComponent(PropertyCollection &propertyCollection, StringManager::StringID classTypeID):DynamicObject(classTypeID)
{
    init(propertyCollection);
}

void UIComponent::init(PropertyCollection &propertyCollection)
{
    m_visual = NULL;
    mVisible = true;
    m_captureInput = false;
    m_width = 0;
    m_height = 0;
    m_x = 0;
    m_y = 0;
    m_id = -1;
    m_transformationMatrixValid = false;
    m_layout = ANCHOR;
    m_anchor = ANCHOR_TOP_LEFT;
    m_parent = NULL;
    m_waitingForReleaseEvent = false;
    
    propertyCollection.getValueForProperty(UI_COMPONENT_HEIGHT, m_height);
    propertyCollection.getValueForProperty(UI_COMPONENT_WIDTH,  m_width);
    
    STRING_ID visualName = StringManager::UNDEFINED;
    if(propertyCollection.getValueForProperty(VISUAL, visualName))
    {
        m_visual = RenderableManager::getInstance()->getRenderable(visualName);
    }

    DynamicObject::init();
}

bool UIComponent::addChild(UIComponent *component)
{
    if(m_children.find(component->getID()) != m_children.end())
    {
        //KKonecko: A UI Component of that ID is already attached
        return false;
    }
    
    component->onAddedToParent(this);
    m_listOfChildren.push_back(component);
    m_children[component->getID()] = component;
    return true;
}

bool UIComponent::removeChild(StringManager::StringID componentID)
{
    if(m_children.find(componentID) == m_children.end())
    {
        //KKonecko: Can't remove a component that we do not have.
        return false;
    }
    
    UIComponent * pComponent = m_children[componentID];
    pComponent->onRemovedFromParent();
    
    m_children.erase(componentID);
    std::list<UIComponent*>::iterator it = m_listOfChildren.begin();
    while(it != m_listOfChildren.end())
    {
        if((*it)->getID() == componentID)
        {
            m_listOfChildren.remove((*it));
            return true;
        }
        it++;
    }
    
    return false;
}


void UIComponent::onBeginRender(Renderer& renderer)
{
    if(mVisible == false)
    {
        return;
    }
    
    if(m_visual != NULL)
    {
        m_visual->onRender(renderer, renderer.getCurrentModelViewProjectionMatrix(), glm::vec3(m_width, m_height, 1.0f));
    }
    
    std::list<UIComponent*>::iterator it = m_listOfChildren.begin();
    while(it != m_listOfChildren.end())
    {
        renderer.pushTransformation((*it)->getTransformation());
        (*it)->onBeginRender(renderer);
        renderer.popTransformation();
        it++;
    }
}

void UIComponent::onLayout()
{
    onLayout(0,0);
}

void UIComponent::onLayout(double globalOffsetX, double globalOffsetY)
{
    if(mVisible == false)
    {
        return;
    }
    
    //KKonecko: Noboby is managing me! I need to do my own layout!
    if(m_parent == NULL)
    {
        m_postLayoutX = m_x;
        m_postLayoutY = m_y;
    }
    
    m_xGlobal = globalOffsetX + m_postLayoutX;
    m_yGlobal = globalOffsetY + m_postLayoutY;
    
    switch(m_layout)
    {
        case ANCHOR:
            doAnchorBasedLayout();
            break;
    }
}

void UIComponent::doAnchorBasedLayout()
{
    std::list<UIComponent*>::iterator it = m_listOfChildren.begin();
    while(it != m_listOfChildren.end())
    {
        UIComponent* pComponent = (*it);
        
        switch(pComponent->m_anchor)
        {
            case ANCHOR_TOP_LEFT:
                pComponent->m_postLayoutX = pComponent->m_x;
                pComponent->m_postLayoutY = pComponent->m_y;
                break;
            case ANCHOR_TOP_CENTER:
                pComponent->m_postLayoutX = pComponent->m_x + m_width / 2.0 - pComponent->m_width / 2.0;
                pComponent->m_postLayoutY = pComponent->m_y;
                break;
            case ANCHOR_TOP_RIGHT:
                pComponent->m_postLayoutX = m_width - pComponent->m_x - pComponent->m_width;
                pComponent->m_postLayoutY = pComponent->m_y;
                break;
            case ANCHOR_LEFT:
                pComponent->m_postLayoutX = pComponent->m_x;
                pComponent->m_postLayoutY = pComponent->m_y + m_height / 2.0 - pComponent->m_height / 2.0;
                break;
            case ANCHOR_CENTER:
                pComponent->m_postLayoutX = pComponent->m_x + m_width / 2.0 - pComponent->m_width / 2.0;
                pComponent->m_postLayoutY = pComponent->m_y + m_height / 2.0 - pComponent->m_height / 2.0;
                break;
            case ANCHOR_RIGHT:
                pComponent->m_postLayoutX = m_width - pComponent->m_x - pComponent->m_width;
                pComponent->m_postLayoutY = pComponent->m_y + m_height / 2.0 - pComponent->m_height / 2.0;
                break;
            case ANCHOR_BOTTOM_LEFT:
                pComponent->m_postLayoutX = pComponent->m_x;
                pComponent->m_postLayoutY = m_height - pComponent->m_y - pComponent->m_height;
                break;
            case ANCHOR_BOTTOM_CENTER:
                pComponent->m_postLayoutX = pComponent->m_x + m_width / 2.0 - pComponent->m_width / 2.0;
                pComponent->m_postLayoutY = m_height - pComponent->m_y - pComponent->m_height;
                break;
            case ANCHOR_BOTTOM_RIGHT:
                pComponent->m_postLayoutX = m_width - pComponent->m_x - pComponent->m_width;
                pComponent->m_postLayoutY = m_height - pComponent->m_y - pComponent->m_height;
                break;
        }        
        
        pComponent->onLayout(m_xGlobal, m_yGlobal);
        it++;
    }
}

void UIComponent::validateTransformationMatrix()
{
    //KKonecko fix me, need to disable setting of m_postLayout directly
    //if(m_transformationMatrixValid == false)
    {
        m_transformationMatrixValid = true;
        m_transformation = glm::translate(glm::mat4(1.0f), glm::vec3(m_postLayoutX, m_postLayoutY, 0)) /* * glm::toMat4(m_rotation)*/;
     }
}

const glm::mat4& UIComponent::getTransformation()
{
    validateTransformationMatrix();
    return m_transformation;
}

void UIComponent::onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data)
{
    bool captured = false;
    onInputEvent(event, data, captured);
}

void UIComponent::onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data, bool &captured)
{
    if(mVisible == false)
    {
        return;
    }
    
    switch (event) {
        case InputManager::ON_PRESS:
            double x, y;
            convertToLocal(data.getPosition().x, data.getPosition().y, x, y);
            if(containsPoint(x, y))
            {
                std::list<UIComponent*>::iterator it = m_listOfChildren.begin();
                while(it != m_listOfChildren.end())
                {
                    (*it)->onInputEvent(event, data, captured);
                    if(captured)
                    {
                        return;
                    }
                    it++;
                }
                
                if(m_captureInput)
                {
                    m_tuchID = data.getID();
                    captured = true;
                    handleOnPress();
                }
            }
            break;
        case InputManager::ON_RELEASE:
            
            if(m_tuchID == data.getID())
            {
                handleOnRelease();
            }
            
            break;
        default:
            break;
    }
}

void UIComponent::handleOnPress()
{
    if(m_waitingForReleaseEvent == false)
    {
        InputManager::onInputEventCallBack callBack;
        callBack.bind(this, &UIComponent::onInputEvent);
        InputManager::getInstance()->registerForInputEvents(callBack);
        m_waitingForReleaseEvent = true;
    }
}

void UIComponent::handleOnRelease()
{
    InputManager::onInputEventCallBack callBack;
    callBack.bind(this, &UIComponent::onInputEvent);
    InputManager::getInstance()->unregisterForInputEvents(callBack);
    m_waitingForReleaseEvent = false;
}

void UIComponent::convertToLocal(double x, double y, double & outX, double & outY)
{
    outX = x - m_xGlobal;
    outY = y - m_yGlobal;
}

bool UIComponent::containsPoint(double x, double y)
{
    if(x < 0) return false;
    if(y < 0) return false;
    if(x > m_width) return false;
    if(y > m_height) return false;
    
    return true;
}

void UIComponent::release()
{
    if(m_waitingForReleaseEvent)
    {
        m_waitingForReleaseEvent = false;
        InputManager::onInputEventCallBack callBack;
        callBack.bind(this, &UIComponent::onInputEvent);
        InputManager::getInstance()->unregisterForInputEvents(callBack);
    }
    
    //KKonecko: There may be better ways of doing this!
    while(!m_listOfChildren.empty())
    {
        UIComponent *component = m_listOfChildren.back();
        component->release();
        delete(component);
    }    
    
    if(m_parent != NULL)
    {
        m_parent->removeChild(m_id);
    }
    
    DynamicObject::release();
}