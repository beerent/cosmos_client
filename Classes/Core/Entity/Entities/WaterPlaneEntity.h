//
//  WaterPlaneEntity.h
//  Projectios1
//
//  Created by Kamil Konecko on 4/22/13.
//
//

#ifndef Projectios1_WaterPlaneEntity_h
#define Projectios1_WaterPlaneEntity_h

#include "Core/Entity/Entities/Entity.h"

class WaterPlaneEntity : public Entity
{
public:
    static const StringManager::StringID CLASS_ID_WATER_PLANE_ENTITY;
    WaterPlaneEntity():Entity(CLASS_ID_WATER_PLANE_ENTITY){};
private:
    glm::vec3 m_renderPosition;

public:
    virtual const glm::mat4& getTransformation();
    
private:
    void onBeginRender(Renderer& renderer);
    virtual void onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);
    void validateTransformationMatrix();
};

#endif
