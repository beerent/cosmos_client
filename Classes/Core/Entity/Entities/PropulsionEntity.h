//
//  PropulsionEntity.h
//  Projectios2
//
//  Created by Kamil Konecko on 2/24/15.
//
//

#ifndef __Projectios2__PropulsionEntity__
#define __Projectios2__PropulsionEntity__

#include <stdio.h>
#include "Core/Entity/Entities/Entity.h"
#include "Core/StringManager/StringManager.h"
class Renderable;

class PropulsionEntity : public Entity
{
public:
    CONST_STRING_DEC(CLASS_ID_PROPULSION_ENTITY)
    CONST_STRING_DEC(THRUST_LBS)
    CONST_STRING_DEC(FLAME_RENDERABLE_NAME)
    
    PropulsionEntity();
    
    void OnHandleEvent(const Event& event);
    
protected:
    void onRootChanged(Entity* previousParent, Entity* newParent);
    
private:
    double mThrustLBS;
    float  mActiveThrustPercentage;
    float  mDesiredVelocity;
    bool   mUseVelocity;
    bool   mUseThrusterOffset;
    
    RenderableObj* m_flameRenderable;
    
    virtual void init(PropertyCollection const &propertyCollection, Entity* parent = NULL);
    virtual void addVisibleRenderables(Renderer& renderer);
};
#endif /* defined(__Projectios2__PropulsionEntity__) */
