//
//  Camera.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 3/20/13.
//
//

#include "Core/Entity/Entities/Camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"

#include "Core/LevelFactory/LevelFactory.h"
#include "Core/Events/GameEvents.h"

const StringManager::StringID Camera::CLASS_ID_CAMERA = StringManager::getIDForString("CLASS_ID_CAMERA");
Camera::Camera():Entity(CLASS_ID_CAMERA), mFollowEntity(StringManager::UNDEFINED)
{
    mAnimatedRotation.mActive = false;
    IEngine::getEngine()->registerForDeltaTimeEvents(this);
};

Camera::~Camera()
{
    IEngine::getEngine()->unregisterForDeltaTimeEvents(this);
}

void Camera::lookAtAnimated(const glm::vec3& targetPos, const glm::vec3& up, float rate,  bool easeIn)
{
    mAnimatedRotation.mType = ROTATION_TARGET;
    mAnimatedRotation.mActive = true;
    mAnimatedRotation.mPercentComplete = 0;
    mAnimatedRotation.mRateOfRotation = rate;
    mAnimatedRotation.mTarget = targetPos;
    mAnimatedRotation.mOriginalRotation = getRotation();
    mAnimatedRotation.mStartTime = 0;
    mAnimatedRotation.mEaseIn = easeIn;
    mAnimatedRotation.mUseUpVec = true;
    mAnimatedRotation.mUP = up;
}

void Camera::lookAtAnimated(const glm::vec3& targetPos, float rate,  bool easeIn)
{
    lookAtAnimated(targetPos, glm::vec3(0,0,0), rate, easeIn);
    mAnimatedRotation.mUseUpVec = false;
}

void Camera::followEntity(STRING_ID entityName, const glm::vec3& offset, float rate)
{
	followEntity(entityName, 0.0f,offset, rate);
}

void Camera::followEntity(STRING_ID entityName, float minDistance, const glm::vec3& offset, float rate)
{
	mFollowEntity = entityName;
	mFollowOffset = offset;
	mFollowRate = rate;
	mFollowMinDistance = minDistance;
}

void Camera::trackEntity(StringManager::StringID entityName, const glm::vec3& up, float rateOfRotation , bool easeIn)
{
    mAnimatedRotation.mType = ROTATION_TRACK;
    mAnimatedRotation.mActive = true;
    mAnimatedRotation.mPercentComplete = 0;
    mAnimatedRotation.mRateOfRotation = rateOfRotation;
    mAnimatedRotation.mOriginalRotation = getRotation();
    mAnimatedRotation.mTrackEntity = entityName;
    mAnimatedRotation.mStartTime = 0;
    mAnimatedRotation.mEaseIn = easeIn;
    mAnimatedRotation.mUP = up;
    mAnimatedRotation.mUseUpVec = true;
}

void Camera::trackEntity(StringManager::StringID entityName,  float rateOfRotation , bool easeIn)
{
    trackEntity(entityName, glm::vec3(0,0,0), rateOfRotation, easeIn);
    mAnimatedRotation.mUseUpVec = false;
}

void Camera::stopTracking()
{
    mAnimatedRotation.mActive = false;
}

void Camera::stopFollow()
{
    mFollowEntity = StringManager::UNDEFINED;
}

void Camera::OnHandleEvent(const Event& event)
{
    if(event.GetEventType() == GameEvents::GAME_DELTA_TIME_EVENT)
    {
        const GameEvents::GameDeltaTimeEvent& timeEvent = static_cast<const GameEvents::GameDeltaTimeEvent&>(event);
        if(mAnimatedRotation.mActive && mAnimatedRotation.mType == ROTATION_TRACK)
        {
            Entity* entity = LevelFactory::getInstance()->getActiveLevel()->getEntityByName(mAnimatedRotation.mTrackEntity, false);
            if(entity != NULL)
            {
                mAnimatedRotation.mStartTime += timeEvent.getDeltaTime();
                float easeInVal = 1.0;
                if(mAnimatedRotation.mEaseIn)
                {
                    easeInVal = mAnimatedRotation.mStartTime / 3.0;
                    easeInVal = easeInVal > 1.0 ? 1.0 : easeInVal;
                }
                
                float percent = mAnimatedRotation.mRateOfRotation * easeInVal * timeEvent.getDeltaTime();
                
                if(mAnimatedRotation.mUseUpVec)
                {
                    lookAt(entity->getPosition(), mAnimatedRotation.mUP, percent);
                }
                else
                {
                    lookAt(entity->getPosition(), percent);
                }
                
                if(percent >= 1)
                {
                    mAnimatedRotation.mActive = false;
                }
                
            }
            else
            {
                mAnimatedRotation.mActive = false;
            }
        }
        
        if(mFollowEntity != StringManager::UNDEFINED)
        {
            Entity* entity = LevelFactory::getInstance()->getActiveLevel()->getEntityByName(mFollowEntity, false);
            if(entity != NULL)
            {
                glm::vec3 newPos = entity->getPosition() + mFollowOffset;
				glm::vec3 delta = newPos - m_position;

				glm::vec3 uintVector = glm::normalize(delta);
				uintVector = uintVector * mFollowMinDistance;
				delta = delta - uintVector;

				if (glm::length(delta) > mFollowMinDistance)
				{
				}

				delta = (mFollowRate * timeEvent.getDeltaTime() * delta) + m_position;
				SetPosition(delta);

            }
            else
            {
                mFollowEntity = StringManager::UNDEFINED;
            }
        }

		if(mAnimatedRotation.mActive && mAnimatedRotation.mType == ROTATION_TARGET)
        {
            if(mAnimatedRotation.mUseUpVec)
            {
                lookAt(mAnimatedRotation.mTarget, mAnimatedRotation.mOriginalRotation, mAnimatedRotation.mUP, mAnimatedRotation.mPercentComplete);
            }
            else
            {
                lookAt(mAnimatedRotation.mTarget, mAnimatedRotation.mOriginalRotation, mAnimatedRotation.mPercentComplete);
            }
            
            mAnimatedRotation.mStartTime += timeEvent.getDeltaTime();
            float easeInVal = 1.0;
            if(mAnimatedRotation.mEaseIn)
            {
                easeInVal = mAnimatedRotation.mStartTime / 3.0;
                easeInVal = easeInVal > 1.0 ? 1.0 : easeInVal;
            }
            
            mAnimatedRotation.mPercentComplete += easeInVal * timeEvent.getDeltaTime() * mAnimatedRotation.mRateOfRotation;
            
            if(mAnimatedRotation.mPercentComplete > 1)
            {
                mAnimatedRotation.mActive = false;
            }
        }
    }
    
    Entity::OnHandleEvent(event);
}