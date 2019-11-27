//
//  DynamicObject.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/18/13.
//
//

#include "Core/Memory/DynamicObject.h"
#include <assert.h>
#include "Core/Debug/Logger/Logger.h"
#include <stdio.h>
#include "Core/StringManager/StringManager.h"

int DynamicObject::mUniqueIdCounter = 0;

DynamicObject::DynamicObject(StringManager::StringID instanceName)
{
    m_instanceName = instanceName;
    m_dynamicObjectTracker = DynamicObjectTrackerManager::getInstance()->trackObject(this);
    mDynamicObjectID = mUniqueIdCounter++;
}

DynamicObject::~DynamicObject()
{
    if(m_dynamicObjectTracker->m_initialized == true)
    {
        std::cout<< "Deleting an initialized object!!!! " << *StringManager::getStringFromID(m_instanceName) << " " << mDynamicObjectID << std::endl;
        assert(false);
    }
    
    m_dynamicObjectTracker->m_destroyed = true;
    sprintf(Logger::GetTempCharBuffer(), "DynamicObject::~DynamicObject m_instanceName %s mDynamicObjectID %d", ID_TO_CSTR(m_instanceName), mDynamicObjectID);
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
}

void DynamicObject::init()
{
    m_dynamicObjectTracker->m_initialized = true;
}

void DynamicObject::release()
{
    sprintf(Logger::GetTempCharBuffer(), "DynamicObject::release m_instanceName %s mDynamicObjectID %d", ID_TO_CSTR(m_instanceName), mDynamicObjectID);
    Logger::Log(Logger::MEMORY, Logger::GetTempCharBuffer());
    
    m_dynamicObjectTracker->m_initialized = false;
}