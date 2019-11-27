//
//  WeaponEntity.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 10/18/15.
//
//

#include "WeaponEntity.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Events/GameEvents.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include "Core/StringManager/StringManager.h"
#include "Core/GameLogic/ProjectileManager.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "../../../glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"


CONST_STRING_DEF(WeaponEntity, CLASS_ID_WEAPON_ENTITY)
CONST_STRING_DEF(WeaponEntity, FIRE_RATE)
CONST_STRING_DEF(WeaponEntity, FIRE_VELOCITY)
CONST_STRING_DEF(WeaponEntity, PROJECTILE_NAME)

WeaponEntity::WeaponEntity():Entity(CLASS_ID_WEAPON_ENTITY), mFireRate(1), mFireVelocity(1), mTargeted(false), mTargetPos(0,0,0), mProjectileName(StringManager::UNDEFINED){}

void WeaponEntity::init(PropertyCollection const &propertyCollection, Entity* parent)
{
    Entity::init(propertyCollection, parent);
    
    propertyCollection.getValueForProperty(FIRE_RATE, mFireRate);
    propertyCollection.getValueForProperty(PROJECTILE_NAME, mProjectileName);
    propertyCollection.getValueForProperty(FIRE_VELOCITY, mFireVelocity);
}

void WeaponEntity::onRootChanged(Entity* previousParent, Entity* newParent)
{
    if(previousParent != NULL)
    {
        previousParent->unRegisterForEventType(GameEvents::FIRE_EVENT, this);
    }
    
    if(newParent != NULL)
    {
        newParent->registerForEventType(GameEvents::FIRE_EVENT, this);
    }
}

void WeaponEntity::OnHandleEvent(const Event& event)
{
    switch (event.GetEventType()) {
        case GameEvents::FIRE_EVENT:
        {
            const GameEvents::FireEvent& fireEvent = static_cast<const GameEvents::FireEvent&>(event);
            
            if(fireEvent.getGroupSubID() == mGroupInstanceID)
            {
                
                STRING_ID type = mProjectileName;
                
                if(fireEvent.getProjectileType() != StringManager::UNDEFINED)
                {
                    type = fireEvent.getProjectileType();
                }
                
                if(fireEvent.isTargeted())
                {
                    // Potentially significant optimization target
                    glm::mat4 globalTransformation;
                    glm::vec4 position(0.0f, 0.0f, 0.0f, 1.0f);
                    
                    calculateGlobalTransformation(globalTransformation);
                    position = globalTransformation * position;
                
                    glm::vec3 dir2 = glm::normalize(fireEvent.getTargetPos() - glm::vec3(position));
                    
                    
                    const glm::vec3 v1 = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
                    const glm::vec3 v2 = glm::normalize(glm::vec3(position) - fireEvent.getTargetPos());
                    const glm::vec3 rotation_axis = glm::normalize(glm::cross(v1, v2));
                    const float angle = glm::degrees(glm::acos(glm::dot(v1, v2)));
                    glm::quat rotation_quat = glm::angleAxis(angle, rotation_axis);
                    rotation_quat = glm::normalize(rotation_quat);
                    
                    
                    Entity* projectile = ProjectileManager::getInstance()->getProjectile(type);
                    projectile->SetPosition(glm::vec3(position) + dir2);
                    projectile->SetRotation(rotation_quat);
                    projectile->setVelocity(dir2 * mFireVelocity /* this->getRoot()->getVelocity() */);
                    
                }
                else
                {
                    // Potentially significant optimization target
                    glm::mat4 globalTransformation;
                    glm::vec4 position(0.0f, 0.0f, 0.0f, 1.0f);
                    
                    calculateGlobalTransformation(globalTransformation);
                    position = globalTransformation * position;
                    
                    glm::quat orientation = getRotation();
                    glm::quat parentOrientation = getRoot()->getRotation();
                    
                    glm::vec3 direction(0.0f, 0.0f, -1.0f);
                    direction = parentOrientation * orientation * direction;
                    
                    Entity* projectile = ProjectileManager::getInstance()->getProjectile(type);
                    projectile->SetPosition(glm::vec3(position) + direction);
                    projectile->SetRotation(parentOrientation * orientation);
                    projectile->setVelocity(direction * mFireVelocity /*+ this->getRoot()->getVelocity()*/);
                }
            }
        }
        break;
        
        default:
            break;
    }
}