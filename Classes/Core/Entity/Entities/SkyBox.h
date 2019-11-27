//
//  Camera.h
//  Projectios1
//
//  Created by Kamil Konecko on 3/20/13.
//
//

#ifndef Projectios1_SkyBox_h
#define Projectios1_SkyBox_h

#include "Core/Entity/Entities/Entity.h"
#include "Core/StringManager/StringManager.h"

class SkyBox : public Entity
{
public:
    
    CONST_STRING_DEC(SKYBOX_TRACK_CAMERA_MODE);
    CONST_STRING_DEC(TRACK_HORIZONTAL_AXIS);
    CONST_STRING_DEC(TRACK_ALL_AXIS);
    
    static const StringManager::StringID CLASS_ID_SKYBOX;
    virtual const glm::mat4& getTransformation();
    
    SkyBox():Entity(CLASS_ID_SKYBOX){};
    
     virtual void init(PropertyCollection const &propertyCollection, Entity* parent = NULL);
    
private:
    
    
    STRING_ID mTrackingMode;
    void onBeginRender(Renderer& renderer);
};

#endif
