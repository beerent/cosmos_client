//
//  DynamicObject.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/18/13.
//
//

#ifndef Projectios1_DynamicObject_h
#define Projectios1_DynamicObject_h
#include <list>
#include "Core/Memory/DynamicObjectTrackerManager.h"
#include "Core/StringManager/StringManager.h"

class DynamicObject
{
friend class DynamicObjectTrackerManager;
    
public:
    DynamicObject(StringManager::StringID instanceName);
    virtual ~DynamicObject();
    virtual void init();
    virtual void release();

    int getObjectId(){return mDynamicObjectID;}
private:
    DynamicObjectTrackerManager::DynamicObjectTracker* m_dynamicObjectTracker;

    static int mUniqueIdCounter;

protected:
    StringManager::StringID m_instanceName;
    int mDynamicObjectID;
    
};
#endif
