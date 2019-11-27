//
//  PropulsionEntity.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 2/24/15.
//
//

#include "PropulsionEntity.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/Rendering/Renderer.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Events/GameEvents.h"
#include "Core/Debug/Debug3DMathVisualizer.h"
#include "Core/Debug/Logger/Logger.h"
#include <string>
#include "Core/StringManager/StringManager.h"

CONST_STRING_DEF(PropulsionEntity, CLASS_ID_PROPULSION_ENTITY)
CONST_STRING_DEF(PropulsionEntity, THRUST_LBS)
CONST_STRING_DEF(PropulsionEntity, FLAME_RENDERABLE_NAME)

PropulsionEntity::PropulsionEntity():Entity(CLASS_ID_PROPULSION_ENTITY),
                                    mActiveThrustPercentage(0), mUseVelocity(false), mDesiredVelocity(0),
                                    m_flameRenderable(NULL), mUseThrusterOffset(false){};


void PropulsionEntity::init(PropertyCollection const &propertyCollection, Entity* parent)
{
    Entity::init(propertyCollection, parent);
    
    mThrustLBS = 0;
    propertyCollection.getValueForProperty(THRUST_LBS, mThrustLBS);

    STRING_ID flameRenderableName = StringManager::UNDEFINED;
    propertyCollection.getValueForProperty(FLAME_RENDERABLE_NAME, flameRenderableName);
    
    if(flameRenderableName != StringManager::UNDEFINED)
    {
        m_flameRenderable = RenderableManager::getInstance()->getRenderable(flameRenderableName);
    }
}

void PropulsionEntity::addVisibleRenderables(Renderer& renderer)
{
    if(m_flameRenderable != NULL && mActiveThrustPercentage > 0)
    {
        renderer.addRenderable(this, m_flameRenderable);
    }
    
    Entity::addVisibleRenderables(renderer);
}

void PropulsionEntity::onRootChanged(Entity* previousParent, Entity* newParent)
{
    if(previousParent != NULL)
    {
        previousParent->unRegisterForEventType(GameEvents::CALCULATE_IMPULSE, this);
        previousParent->unRegisterForEventType(GameEvents::PROPULTION_ENGINE_POWER_EVENT, this);
    }
    
    if(newParent != NULL)
    {
        newParent->registerForEventType(GameEvents::CALCULATE_IMPULSE, this);
        newParent->registerForEventType(GameEvents::PROPULTION_ENGINE_POWER_EVENT, this);
    }
}

void PropulsionEntity::OnHandleEvent(const Event& event)
{
    switch (event.GetEventType()) {
        case GameEvents::CALCULATE_IMPULSE:
        {
            const GameEvents::CalculateImpulseEvent& impulseEvent = static_cast<const GameEvents::CalculateImpulseEvent& >(event);
            
            
            if(mActiveThrustPercentage > 0 && mUseVelocity)
            {
               

                if(mDesiredVelocity < 0 )
                {
                    
                    mActiveThrustPercentage  = 0;
                }
                else
                {
                     mDesiredVelocity = mDesiredVelocity - 1;
                }
            }
            
            
            float force = mThrustLBS * mActiveThrustPercentage;
            

            bool forceApplied = false;
            if(force > 0)
            {
                forceApplied = true;
            }

            glm::mat4 globalTransformation;
            glm::vec4 position(0.0f, 0.0f, 0.0f, 1.0f);
            
            calculateGlobalTransformation(globalTransformation);
            position = globalTransformation * position;

            glm::quat orientation = getRotation();
            glm::quat parentOrientation = getRoot()->getRotation();
            
            glm::vec3 direction(0.0f, 0.0f, -1.0f);
            direction = parentOrientation * orientation * direction;
            glm::vec3 pos3(position);
            glm::vec3 pos3b = pos3 + direction;
            
            glm::vec3 normalizedPosition(position);
            
            normalizedPosition = glm::normalize(direction);
            
            impulseEvent.mAggregator->mForce.x += normalizedPosition.x * force;
            impulseEvent.mAggregator->mForce.y += normalizedPosition.y * force;
            impulseEvent.mAggregator->mForce.z += normalizedPosition.z * force;
            
            glm::mat4 modelTransformation;
            glm::vec4 modelPosition(0.0f, 0.0f, 0.0f, 1.0f);
            mParent->calculateGlobalTransformation(modelTransformation);
            modelPosition = modelTransformation * modelPosition;
            
            glm::vec3 posm = glm::vec3(modelPosition);
            posm = pos3b - posm;
            posm = glm::normalize(posm);
            
            if(mUseThrusterOffset)
            {
                impulseEvent.mAggregator->mRelativePoint = glm::normalize(impulseEvent.mAggregator->mRelativePoint + posm);
            }
            else
            {
                impulseEvent.mAggregator->mRelativePoint = glm::vec3(0.0,0.0,0.0);
            }
        }
            break;
        case GameEvents::PROPULTION_ENGINE_POWER_EVENT:
        {
            const GameEvents::PropultionEnginePowerEvent& propEvent = static_cast<const GameEvents::PropultionEnginePowerEvent&>(event);
            STRING_ID strID = propEvent.getGroupSubID();
            
            sprintf(Logger::GetTempCharBuffer(), "%s PROPULTION_ENGINE_POWER_EVENT subGroup %s targetSubGroup %s currentTargetVel %f", __func__, ID_TO_CSTR(strID),
                    ID_TO_CSTR(mGroupInstanceID), mDesiredVelocity);
            Logger::Log(Logger::SCRIPTING, Logger::GetTempCharBuffer());
            
            if(strID == mGroupInstanceID)
            {
                //HACK HACK
                if(strID == STRING_TO_ID("003"))
                {
                    mUseThrusterOffset = true;
                }
                else
                {
                    mUseThrusterOffset = false;
                }
                
                mActiveThrustPercentage = propEvent.getPower();
                mUseVelocity = propEvent.useVelocity();
                mDesiredVelocity = propEvent.getVelocity();
                sprintf(Logger::GetTempCharBuffer(), "%s PROPULTION_ENGINE_POWER_EVENT adjusting engine power to %f", __func__, mActiveThrustPercentage);
                Logger::Log(Logger::SCRIPTING, Logger::GetTempCharBuffer());
            }
        }
            break;
            
        default:
            break;
    }
}