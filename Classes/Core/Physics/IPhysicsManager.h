//
//  IPhysicsManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 2/17/14.
//
//

#ifndef Projectios1_IPhysicsManager_h
#define Projectios1_IPhysicsManager_h

#include "glm/glm.hpp"

class Entity;

class IPhysicsManager
{
    public:
        static IPhysicsManager* getPhysicsManager(){return m_physicsManager;}
        virtual void initialize(const glm::vec3& gravity, bool hasGround) = 0;
        virtual void addEntity(Entity* entity) = 0;
        virtual void removeEntity(Entity* entity) = 0;
    
    protected:
        static IPhysicsManager* m_physicsManager;
};

#endif
