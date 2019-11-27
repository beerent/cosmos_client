//
//  LightingManager.h
//  Projectios2
//
//  Created by Kamil Konecko on 10/14/15.
//
//

#ifndef __Projectios2__LightingManager__
#define __Projectios2__LightingManager__

#include <stdio.h>
#include "../../../glm/glm.hpp"

class LightingManager
{
public:
    void setDiffuseColor(const glm::vec3& color){mDiffuseColor = color;}
    void setAmbientColor(const glm::vec3& color){mAmbientColor = color;}
    void setSpecularColor(const glm::vec3& color){mSpecularColor = color;}
    void setInfinitLightSourceVector(const glm::vec3& vector){mInifiniteLightSourceVector = glm::normalize(vector);}
    
    const glm::vec3& getDiffuseColor()const {return mDiffuseColor;}
    const glm::vec3& getAmbientColor()const {return mAmbientColor;}
    const glm::vec3& getSpecularColor()const {return mSpecularColor;}
    const glm::vec3& getInfinitLightSourceVector()const {return mInifiniteLightSourceVector;}
    
    LightingManager()
    {
        reset();
    }
    
    void reset()
    {
        mInifiniteLightSourceVector = glm::vec3(1.0, 0.0, 0.0);
        mAmbientColor = glm::vec3(0.2, 0.2, 0.2);
        mDiffuseColor = glm::vec3(0.75, 0.75, 0.75);
        mSpecularColor = glm::vec3(0.5, 0.5, 0.5);

    }
    
private:
    glm::vec3 mInifiniteLightSourceVector;
    glm::vec3 mAmbientColor;
    glm::vec3 mDiffuseColor;
    glm::vec3 mSpecularColor;
};

#endif /* defined(__Projectios2__LightingManager__) */
