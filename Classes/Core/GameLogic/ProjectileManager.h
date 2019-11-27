//
//  ProjectileManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/4/14.
//
//

#ifndef Projectios1_ProjectileManager_h
#define Projectios1_ProjectileManager_h

#include "Core/StringManager/StringManager.h"
#include <list>
#include <map>

class Entity;

class ProjectileManager
{
public:
    const unsigned int MAX_PROJECTILES = 30;
    
    static ProjectileManager* getInstance();
    Entity* getProjectile(STRING_ID name);
    
    void Reset(){mMapOfQueues.clear();};
    
protected:
    static ProjectileManager* mInstance;
    
    typedef std::list<Entity*> EntityPtrList;
    typedef std::map<STRING_ID, EntityPtrList> StrIDToEntityPtrListMap;
    StrIDToEntityPtrListMap mMapOfQueues;
    
};


#endif
