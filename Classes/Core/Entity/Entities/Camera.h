//
//  Camera.h
//  Projectios1
//
//  Created by Kamil Konecko on 3/20/13.
//
//

#ifndef Projectios1_Camera_h
#define Projectios1_Camera_h

#include "Core/Entity/Entities/Entity.h"
#include "Core/StringManager/StringManager.h"
#include "Core/Events/EventDispatcher.h"
#include "glm/glm.hpp"

class Camera : public Entity
{
public:
    Camera();
    ~Camera();
    static const StringManager::StringID CLASS_ID_CAMERA;
    void setIsActiveCamera(bool isActive){m_isActive = isActive;};

    void lookAtAnimated(const glm::vec3& targetPos, float rate, bool easeIn = true);
    void lookAtAnimated(const glm::vec3& targetPos, const glm::vec3& up, float rate, bool easeIn = true);

    void trackEntity(STRING_ID entityName, float rateOfRotation = 1.0, bool easeIn = true );
    void trackEntity(STRING_ID entityName, const glm::vec3& up, float rateOfRotation = 1.0, bool easeIn = true );

    void OnHandleEvent(const Event& event);
    void followEntity(STRING_ID entityName, const glm::vec3& offset, float rate = 0.3f);
	void followEntity(STRING_ID entityName, float minDistance, const glm::vec3& offset, float rate = 0.3f);

	void stopTracking();
    void stopFollow();
    
private:
    bool m_isActive = false;
    STRING_ID mFollowEntity;
    glm::vec3 mFollowOffset;
    float     mFollowRate;
	float	  mFollowMinDistance;
    
    enum RotationType{ ROTATION_TRACK, ROTATION_TARGET};
    
    struct AnimatedRotation
    {
        RotationType    mType;
        bool            mActive;
        bool            mEaseIn;
        bool            mUseUpVec;
        float           mRateOfRotation;
        float           mPercentComplete;
        float           mStartTime;
        
        glm::vec3       mTarget;
        glm::vec3       mUP;
        glm::quat       mOriginalRotation;
        STRING_ID       mTrackEntity;
    };
    
    AnimatedRotation mAnimatedRotation;
};

#endif
