//
//  Debug3DMathVisualizer.cpp
//  Projectios2
//
//  Created by Kamil Konecko on 6/25/15.
//
//

#include "Core/Debug/Debug3DMathVisualizer.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/LevelFactory/Level.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/Entity/EntityFactory.h"
#include "../../glm/glm.hpp"
#include "../../glm/gtc/matrix_transform.hpp"
#include "../../glm/gtc/type_ptr.hpp"
#include "../../glm/gtx/quaternion.hpp"
#include "../../glm/gtx/transform.hpp"

Debug3DVisual::Debug3DVisual(STRING_ID type)
{
    mEntity = EntityFactory::getInstance()->createEntity(type);
}

Debug3DVisual::~Debug3DVisual()
{
    //We should clear unused entities here
    //EntityFactory::getInstance()->
}

void Debug3DVisual::SetRotation(const glm::quat& rotation)
{
    mEntity->SetRotation(rotation);
}

void Debug3DVisual::SetPosition(const glm::vec3 &position)
{
    mEntity->SetPosition(position);
}

void Debug3DVisual::SetPosition(const glm::vec4 &position)
{
    mEntity->SetPosition(glm::vec3(position.x, position.y, position.z));
}

void Debug3DVisual::SetScale(float scale)
{
    mEntity->setScale(scale);
}

glm::vec3 const & Debug3DVisual::getPosition()const
{
    return mEntity->getPosition();
}

glm::quat const & Debug3DVisual::getRotation()const
{
    return mEntity->getRotation();
}

Debug3DMathVisualizer::Debug3DMathVisualizer():
mUnusedIDIndex(0),
mActiveLevel(NULL)
{
    
}

int Debug3DMathVisualizer::getOrCreateVisual(StringManager::StringID name, StringManager::StringID type)
{
    int visualId = getIDFromName(name);
    
    if(visualId == -1)
    {
        return createVisual(name, type);
    }
    
    return visualId;
}

int Debug3DMathVisualizer::createVisual(STRING_ID name, STRING_ID type)
{
    ++mUnusedIDIndex;
    
    Debug3DVisual* visual    = new Debug3DVisual(type);
    mVisuals[mUnusedIDIndex] = visual;
    visual->mEntity->setDebugText(name);

    if(getIDFromName(name) != -1)
    {
        Logger::Error("Debug3DMathVisualizer::createVisual name already Exists!");
    }
    
    mNameToIDMap[name] = mUnusedIDIndex;
    mActiveLevel->addEntity(visual->mEntity);
    return mUnusedIDIndex;
}

int Debug3DMathVisualizer::getIDFromName(STRING_ID name) const
{
    StringIDToIntMap::const_iterator it = mNameToIDMap.find(name);
    if(it != mNameToIDMap.end())
    {
        return it->second;
    }
    
    return -1;
}

void Debug3DMathVisualizer::reset()
{
    IntToDebug3DVisualMap::iterator          it = mVisuals.begin();
    const IntToDebug3DVisualMap::iterator itEnd = mVisuals.end();
    
    while(it != itEnd)
    {
        delete(it->second);
        ++it;
    }
    
    mVisuals.clear();
    mNameToIDMap.clear();
}

void Debug3DMathVisualizer::init(Level *activeLevel)
{
    mActiveLevel = activeLevel;
}

Debug3DVisual* Debug3DMathVisualizer::getVisual(int handle)
{
    IntToDebug3DVisualMap::iterator it = mVisuals.find(handle);
    if(it == mVisuals.end())
    {
        Logger::Error("Debug3DMathVisualizer::getVisual Could not find Visualizer with given handle.");
        return NULL;
    }
    
    return it->second;
}

void Debug3DMathVisualizer::rotateVisualAlongLine(Debug3DVisual* visual, const glm::vec3& lineStart, const glm::vec3& lineEnd)
{
    const glm::vec3 v1 = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    const glm::vec3 v2 = glm::normalize(lineEnd - lineStart);
    const glm::vec3 rotation_axis = glm::normalize(glm::cross(v1, v2));
    const float angle = glm::degrees(glm::acos(glm::dot(v1, v2)));
    glm::quat rotation_quat = glm::angleAxis(angle, rotation_axis);
    rotation_quat = glm::normalize(rotation_quat);
    visual->SetRotation(rotation_quat);
}
