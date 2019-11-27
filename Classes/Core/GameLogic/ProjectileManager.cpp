//
//  ProjectileManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/4/14.
//
//

#include "Core/GameLogic/ProjectileManager.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/Entity/EntityFactory.h"

ProjectileManager* ProjectileManager::mInstance;

ProjectileManager* ProjectileManager::getInstance()
{
    if(mInstance == NULL)
    {
        mInstance = new ProjectileManager();
    }
    
    return mInstance;
}


Entity* ProjectileManager::getProjectile(STRING_ID name)
{
    Entity* projectile = NULL;
    EntityPtrList& projectilesQueue = mMapOfQueues[name];
    
    if(projectilesQueue.size() < MAX_PROJECTILES)
    {
        projectile = EntityFactory::getInstance()->createEntity(name);
        projectilesQueue.push_back(projectile);
        LevelFactory::getInstance()->getActiveLevel()->addEntity(projectile);
        return projectile;
    }

    projectile = projectilesQueue.front();
    projectilesQueue.pop_front();
    projectilesQueue.push_back(projectile);
   
    if(projectile->isActive())
    {
        projectile->ResetPhysics();
    }
    else
    {
        projectile->setActive(true);
    }
    return projectile;
}