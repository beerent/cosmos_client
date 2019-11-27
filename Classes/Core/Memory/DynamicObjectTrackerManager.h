//
//  DynamicObjectManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/18/13.
//
//

#ifndef Projectios1_DynamicObjectManager_h
#define Projectios1_DynamicObjectManager_h
#include <iostream>
#include <fstream>
#include <list>
#include "Core/StringManager/StringManager.h"

class DynamicObject;

class DynamicObjectTrackerManager
{
public:
    struct DynamicObjectTracker
    {
        bool m_destroyed;
        bool m_initialized;
        bool m_reported;
        StringManager::StringID m_classTypeID;
        DynamicObject *m_dynamicObject;
    };
    
    struct DynamicObjectCount
    {
        int m_activeObjects;
        int m_removedLastPass;
        int m_removedTotal;
        int m_uninitialized;
    };
    
private:
    static std::list<DynamicObjectTracker*> m_ActiveDynamicObjectTrackers;
    static DynamicObjectTrackerManager *m_instance;
    
    DynamicObjectCount m_dynamicObjectCount;    
    DynamicObjectTrackerManager();

    void printRemoveInfoForObject(const DynamicObjectTracker &trackerObject)const;
    void printErrorInfoForObject(const DynamicObjectTracker &trackerObject)const;
    void printDebugInfoForObject(const DynamicObjectTracker &trackerObject)const;
   
public:
    static DynamicObjectTrackerManager* getInstance();
    
    DynamicObjectTracker* trackObject(DynamicObject * object);
    void clean();
    void printReport() const;
    const DynamicObjectCount& getDynamicObjectCount(){ return m_dynamicObjectCount;};

};
#endif
