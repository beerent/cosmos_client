//
//  DynamicObjectTrackerManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/18/13.
//
//

#include "Core/Memory/DynamicObjectTrackerManager.h"
#include "Core/Memory/DynamicObject.h"
#include "Core/Debug/Logger/Logger.h"

DynamicObjectTrackerManager* DynamicObjectTrackerManager::m_instance = NULL;
std::list<DynamicObjectTrackerManager::DynamicObjectTracker*> DynamicObjectTrackerManager::m_ActiveDynamicObjectTrackers;

DynamicObjectTrackerManager::DynamicObjectTrackerManager()
{
    m_dynamicObjectCount.m_activeObjects = 0;
    m_dynamicObjectCount.m_removedLastPass = 0;
    m_dynamicObjectCount.m_removedTotal = 0;
    m_dynamicObjectCount.m_uninitialized = 0;
}

DynamicObjectTrackerManager* DynamicObjectTrackerManager::getInstance()
{
    if(m_instance == NULL)
    {
        m_instance = new DynamicObjectTrackerManager();
    }
    
    return m_instance;
}

DynamicObjectTrackerManager::DynamicObjectTracker* DynamicObjectTrackerManager::trackObject(DynamicObject * object)
{
    DynamicObjectTracker *dynamicObjectTracker = new DynamicObjectTracker();
    dynamicObjectTracker->m_initialized = false;
    dynamicObjectTracker->m_destroyed = false;
    dynamicObjectTracker->m_dynamicObject = object;
    dynamicObjectTracker->m_classTypeID = object->m_instanceName;
    dynamicObjectTracker->m_reported = false;
    m_ActiveDynamicObjectTrackers.push_back(dynamicObjectTracker);
    
    return dynamicObjectTracker;
}

void DynamicObjectTrackerManager::clean()
{
    std::list<DynamicObjectTracker*>::iterator it = m_ActiveDynamicObjectTrackers.begin();
    
    m_dynamicObjectCount.m_removedLastPass = 0;
    m_dynamicObjectCount.m_uninitialized = 0;
    m_dynamicObjectCount.m_activeObjects = 0;
    
    while(it != m_ActiveDynamicObjectTrackers.end())
    {
        DynamicObjectTracker * dynamicObjectTracker = *it;
     
        if(dynamicObjectTracker->m_initialized == false && dynamicObjectTracker->m_destroyed == false)
        {
            m_dynamicObjectCount.m_uninitialized++;
            printErrorInfoForObject(*dynamicObjectTracker);
            dynamicObjectTracker->m_reported = true;
        }
        if(dynamicObjectTracker->m_destroyed)
        {
            printRemoveInfoForObject(*dynamicObjectTracker);
            dynamicObjectTracker->m_reported = true;
            
            m_ActiveDynamicObjectTrackers.erase(it++);
            delete(dynamicObjectTracker);
            m_dynamicObjectCount.m_removedLastPass++;
        }
        else
        {
            m_dynamicObjectCount.m_activeObjects++;
            it++;
        }
        
    }
    
    m_dynamicObjectCount.m_removedTotal += m_dynamicObjectCount.m_removedLastPass;
}

void DynamicObjectTrackerManager::printRemoveInfoForObject(const DynamicObjectTrackerManager::DynamicObjectTracker &trackerObject)const
{
    if(trackerObject.m_reported)
    {
        return;
    }

    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("Removed OBJECT *****************************************"));
    printDebugInfoForObject(trackerObject);
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("********************************************************"));
}

void DynamicObjectTrackerManager::printErrorInfoForObject(const DynamicObjectTrackerManager::DynamicObjectTracker &trackerObject)const
{
    if(trackerObject.m_reported)
    {
        return;
    }

    if(trackerObject.m_initialized == false)
    {
        Logger::Warning(Logger::DYNAMIC_OBJECT, std::string("Object unitilialized: " + *StringManager::getStringFromID(trackerObject.m_classTypeID)));
    }
    printDebugInfoForObject(trackerObject);
    
}

void DynamicObjectTrackerManager::printDebugInfoForObject(const DynamicObjectTrackerManager::DynamicObjectTracker &trackerObject)const
{
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("Object Class Type: " + *StringManager::getStringFromID(trackerObject.m_classTypeID)));   
    
    if(trackerObject.m_destroyed != true)
    {
        Logger::Log(Logger::DYNAMIC_OBJECT, std::string("Instance: " + *StringManager::getStringFromID(trackerObject.m_dynamicObject->m_instanceName)));
    }
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("Destroyed: ") + std::string(trackerObject.m_destroyed == true ? "true" : "false"));
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("Initizlied: ") + std::string(trackerObject.m_initialized == true ? "true" : "false"));
}

void DynamicObjectTrackerManager::printReport()const
{
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("*********************************************************************"));
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("Print Dynamic Object Report *****************************************"));
    Logger::Log(Logger::DYNAMIC_OBJECT, std::string("*********************************************************************"));

    std::cout << "Active Objects: " << m_dynamicObjectCount.m_activeObjects << std::endl;
    std::cout << "Removed Last Pass: " << m_dynamicObjectCount.m_removedLastPass << std::endl;
    std::cout << "Unitialized: " << m_dynamicObjectCount.m_uninitialized << std::endl;
    std::cout << "RemovedTotal: " << m_dynamicObjectCount.m_removedTotal << std::endl;
    std::cout << "" << std::endl;

    std::list<DynamicObjectTracker*>::iterator it = m_ActiveDynamicObjectTrackers.begin();

/*
    std::cout << "Active Objects: " << std::endl;

 
    while(it != m_ActiveDynamicObjectTrackers.end())
    {
        DynamicObjectTracker * dynamicObjectTracker = *it;
        
        if(dynamicObjectTracker->m_initialized == true && dynamicObjectTracker->m_destroyed == false)
        {
            printDebugInfoForObject(*dynamicObjectTracker);
            std::cout << "" << std::endl;
        }
        it++;
    }

*/
    it = m_ActiveDynamicObjectTrackers.begin();


    std::cout << "Uninitialized Objects: " << std::endl;
    
    while(it != m_ActiveDynamicObjectTrackers.end())
    {
        DynamicObjectTracker * dynamicObjectTracker = *it;
        
        if(dynamicObjectTracker->m_initialized == false && dynamicObjectTracker->m_destroyed == false)
        {
            printDebugInfoForObject(*dynamicObjectTracker);
            std::cout << "" << std::endl;
        }
        it++;
    }
    
    std::cout << "*********************************************************************" << std::endl;

    std::cout << "*********************************************************************" << std::endl;

}

