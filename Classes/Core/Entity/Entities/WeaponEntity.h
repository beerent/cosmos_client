//
//  WeaponEntity.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/18/15.
//
//

#ifndef __Projectios2__WeaponEntity__
#define __Projectios2__WeaponEntity__

#include <stdio.h>
#include "Core/Entity/Entities/Entity.h"
#include "Core/StringManager/StringManager.h"
#include "../../../glm/glm.hpp"

class WeaponEntity : public Entity
{
public:
    CONST_STRING_DEC(CLASS_ID_WEAPON_ENTITY)
    CONST_STRING_DEC(FIRE_RATE)
    CONST_STRING_DEC(FIRE_VELOCITY)
    CONST_STRING_DEC(PROJECTILE_NAME)
    
    WeaponEntity();
    
    void OnHandleEvent(const Event& event);
    
protected:
    void onRootChanged(Entity* previousParent, Entity* newParent);
    
private:
    long       mFireRate;
    STRING_ID  mProjectileName;
    float      mFireVelocity;
    bool       mTargeted;
    glm::vec3  mTargetPos;
    
    virtual void init(PropertyCollection const &propertyCollection, Entity* parent = NULL);
};


#endif /* defined(__Projectios2__WeaponEntity__) */
