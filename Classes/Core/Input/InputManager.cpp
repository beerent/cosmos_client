//
//  ImputManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 5/27/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Core/Input/InputManager.h"

InputManager* InputManager::m_pInstance = NULL;

InputManager::InputManager()
{
    m_pInstance = this;
};

InputManager::~InputManager(){};

void InputManager::registerForInputEvents(onInputEventCallBack callBack)
{    
    std::list<onInputEventCallBack>::iterator it = m_inputListeners.begin();

#ifdef debug
    while(it != m_inputListeners.end())
    {
        if(*it == callBack)
        {
            std::cout << "***********************************************************" << std::endl; 
            std::cout << "Warning Adding Duplicate Entry In RegisterForINputEvents" << std::endl; 
            std::cout << "***********************************************************" << std::endl; 
        }
        it++;
    }
#endif

    m_inputListeners.push_back(callBack);
}

void InputManager::unregisterForInputEvents(onInputEventCallBack callBack)
{
    //KKonecko: We can't remove here becouse this remove could be the result of an event dispatch. Instead we will remove before
    //dispatching events.
    m_delayedRemoveList.push_back(callBack);
}

void InputManager::onHardwareInputEvent(DeviceInputEvent deviceInputEvent, InputEventData data)
{
    switch (deviceInputEvent)
    {
        case DEVICE_TOUCH_DOWN:
            dispatchEvent(ON_PRESS, data);
            break;
        case DEVICE_TOUCH_UP:
            dispatchEvent(ON_RELEASE, data);
            break;
        case DEVICE_TOUCH_MOVE:
            dispatchEvent(ON_DRAG, data);
            break;
		case DEVICE_KEY_DOWN:
			dispatchEvent(ON_KEY_DOWN, data);
			break;
		case DEVICE_KEY_UP:
			dispatchEvent(ON_KEY_UP, data);
			break;
        default:
            break;
    }
}

void InputManager::dispatchEvent(InputManager::InputEvent event, InputEventData data)
{
    //KKonecko; Clean up any events that have been removed
    if(!m_inputListeners.empty())
    {
        std::list<onInputEventCallBack>::iterator it = m_delayedRemoveList.begin();
        while(it != m_delayedRemoveList.end())
        {
            m_inputListeners.remove(*it);
            it++;
        }
        m_delayedRemoveList.clear();
    }
   
    
    std::list<onInputEventCallBack>::iterator it = m_inputListeners.begin();
    while(it != m_inputListeners.end())
    {
        (*it)(event, data);
        it++;
    }
}

InputManager* InputManager::getInstance()
{
    return m_pInstance;
};
