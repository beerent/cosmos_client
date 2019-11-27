//
//  Entity.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include "Core/Entity/Entities/Entity.h"
#include "Core/Rendering/RenderableManager.h"
#include "Core/Rendering/Renderables/UIFontRenderable.h"
#include "Core/Rendering/Renderer.h"
#include "IEngine.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Core/Entity/Entities/Camera.h"
#include "Core/Rendering/Renderables/AxisRenderable.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Physics/IPhysicsManager.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/Settings/Settings.h"
#include "Core/Entity/Entities/PhysicsEntity.h"
#include "Core/GameLogic/EntityControler/EntityControler.h"
#include "Core/Entity/EntityFactory.h"

CONST_STRING_DEF(Entity, RENDERABLE_NAME)
CONST_STRING_DEF(Entity, CLASS_ID_ENTITY)
CONST_STRING_DEF(Entity, ENTITY_GROUP)
CONST_STRING_DEF(Entity, SHOW_DEBUG_AXIS)
CONST_STRING_DEF(Entity, SHOW_DEBUG_NAME)
CONST_STRING_DEF(Entity, PHYSICS_TYPE)
CONST_STRING_DEF(Entity, PHYSICS_ENTITY)
CONST_STRING_DEF(Entity, PHYSICS_NONE)
CONST_STRING_DEF(Entity, PHYSICS_ENTITY_NAME)

bool Entity::SHOW_ALL_NAMES = false;

Entity::Entity():DynamicObject(CLASS_ID_ENTITY)
{
    init();
}

Entity::Entity(StringManager::StringID classIDType):DynamicObject(classIDType)
{
    init();
}

Entity::~Entity()
{
    setControler(NULL);
}

void Entity::init()
{
    m_debugText = StringManager::UNDEFINED;
    m_namePlate = NULL;
    m_lineRenderable = NULL;
    m_renderCoordinateLines = true;
    m_renderAssetName = true;
    m_valid = false;
    m_renderable = NULL;
    mParent = NULL;
    mPhysicsObject = NULL;
    mPhysicsType = PHYSICS_NONE;
    m_transformationMatrixValid = false;
    mGroupInstanceID = StringManager::UNDEFINED;
    mControler = NULL;
    mActive = true;
    mPhsyicsAttached = false;
}

Entity* Entity::getRoot()
{
    Entity* root = this;
    while(root->getParent() != NULL)
    {
        root = root->getParent();
    }
    
    return root;
}

const Entity* Entity::getRoot() const
{
    const Entity* root = this;
    while(root->getParent() != NULL)
    {
        root = root->getParent();
    }
    
    return root;
}

void Entity::setScale(float scale)
{
    setScale(glm::vec3(scale));
}

void Entity::setScale(const glm::vec3 &scale)
{
	glm::vec3 s(glm::abs(scale.x), glm::abs(scale.y), glm::abs(scale.z));
    m_scale = glm::scale(glm::mat4(1.0f), s);
    m_transformationMatrixValid = false;    
    m_scaleVec = s;
}

void Entity::SetPosition(const glm::vec3 &position)
{
    m_position = position;
    m_transformationMatrixValid = false;
}

void Entity::SetPosition(const glm::vec4 &position)
{
    m_position.x = position.x;
    m_position.y = position.y;
    m_position.z = position.z;

    m_transformationMatrixValid = false;
}

void Entity::SetRotation(const glm::quat &rotation)
{
    m_rotation = rotation;
    m_transformationMatrixValid = false;
}

void Entity::setVelocity(const glm::vec3 &velocity)
{
    mVelocity = velocity;
}

void Entity::onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
    if(SHOW_ALL_NAMES == true || renderable.getClassID() == UIFontRenderable::CLASS_ID_FONT_RENDERABLE)
    {
        const char *assetNameStr = NULL;
        if(m_debugText != StringManager::UNDEFINED)
        {
            assetNameStr = ID_TO_CSTR(m_debugText);
        }
        else
        {
            assetNameStr = StringManager::getStringFromID(m_renderable->getAssetName())->c_str();
        }
    
        GLfloat fontScale = 0.1f;
        m_namePlate->onRender(renderer, modelViewProjectionMatrix, glm::vec3(m_namePlate->getFont()->getWidthForString(assetNameStr) * fontScale, fontScale, fontScale), assetNameStr, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.0f, 0.0f);
    }
    else
    {
        renderable.onRender(renderer, modelViewProjectionMatrix, modelView);
    }
}

void Entity::addVisibleRenderables(Renderer& renderer)
{
    if(m_renderable != NULL)
    {
        renderer.addRenderable(this, m_renderable);
    }
}

bool Entity::hasRenderable() const 
{
	return m_renderable == NULL ? false : true;
}

void Entity::setParent(Entity* entity)
{
    if(mParent == entity)
    {
        Logger::Error("Entity::setParent Already is parent");
    }
    
    Entity* previousParent = mParent;
    
    if(mParent != NULL)
    {
        mParent = NULL;
        previousParent->removeChild(this);
    }

    mParent = entity;
    
    processRootChanged(previousParent, mParent);
}

void Entity::processRootChanged(Entity* previousParent, Entity* newParent)
{
    EntityList::iterator it = mChildren.begin();
    const EntityList::iterator endIt = mChildren.end();
    
    onRootChanged(previousParent, newParent);
    
    while(it != endIt)
    {
        (*it)->processRootChanged(previousParent, newParent);
        ++it;
    }
}

void Entity::initPhysics()
{
    if(PHYSICS_ENTITY == mPhysicsType)
    {
        bool canInitializePhysics = false;
        
        if(mParent == NULL)
        {
            canInitializePhysics = true;
        }
        else
        {
            Entity* topParent = mParent;
            while(topParent->getParent() != NULL)
            {
                topParent = topParent->getParent();
            }
            
            if(topParent->mPhysicsType != PHYSICS_ENTITY)
            {
                canInitializePhysics = true;
            }
        }
        
        if(canInitializePhysics)
        {
            IPhysicsManager::getPhysicsManager()->addEntity(this);
            mPhsyicsAttached = true;
        }
    }
}

void Entity::onBeginRender(Renderer& renderer)
{
    if(isActive() == false)
    {
        return;
    }
    
    renderer.pushTransformation(getTransformation());
   
    addVisibleRenderables(renderer);

    if(m_lineRenderable != NULL && Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_AXIS))
    {
        renderer.addRenderable(this, m_lineRenderable);
    }
    else if(Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_AXIS))
    {
        m_lineRenderable = static_cast<AxisRenderable*>(RenderableManager::getInstance()->getRenderable(StringManager::getIDForString("axisRenderable")));
    }
    
    //KKonecko: Render all the children
    EntityList::iterator it = mChildren.begin();
    
    while(it != mChildren.end())
    {
        (*it)->onBeginRender(renderer);
        it++;
    }

    if(mPhysicsType == PHYSICS_ENTITY)
    {
        mPhysicsObject->onBeginRender(renderer);
    }

    renderer.popTransformation();
    
    if(m_namePlate != NULL && m_renderable != NULL && Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_NAMES))
    {
        glm::vec4 pos = glm::vec4(getPosition(), 1.0f);
        
        if(mParent != NULL)
        {
            pos = renderer.getCurrentTransformation() * pos;
        }
        
        glm::vec4 cameraUp4 = IEngine::getEngine()->getActiveCamera()->getTransformation()[1];
        glm::vec4 cameraRight4 = IEngine::getEngine()->getActiveCamera()->getTransformation()[0];
        glm::vec4 cameraFoward4 = IEngine::getEngine()->getActiveCamera()->getTransformation()[2];
        
        glm::mat4 billboard;
        billboard[0] = cameraRight4;
        billboard[1] = cameraUp4;
        billboard[2] = cameraFoward4;
        billboard[3].x = pos.x + 0.2f;
        billboard[3].y = pos.y + 0.2f;
        billboard[3].z = pos.z;
        billboard[3].w = 1.0f;
        
        renderer.pushTransformation(billboard, true);
        renderer.addRenderable(this, m_namePlate);
        renderer.popTransformation();
    }
    else if(m_renderable != NULL && Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_NAMES)&& CLASS_ID_ENTITY == m_instanceName)
    {
       m_namePlate = static_cast<UIFontRenderable*>(RenderableManager::getInstance()->getRenderable(StringManager::getIDForString("uiAnadaleMonoRenderable3d")));
    }
    
}

//KKonecko: Breaks away from a parent Entity and moves to the currently active level
void Entity::breakAway()
{
    if(mParent != NULL)
    {
        if(mPhysicsType == PHYSICS_ENTITY)
        {
            IPhysicsManager::getPhysicsManager()->removeEntity(this);
        }

        glm::mat4 glmTransofrm(1.0);
        calculateGlobalTransformation(glmTransofrm);
        //mParent->removeChild(this);
        setParent(NULL);
        m_transformation = glmTransofrm;
        LevelFactory::getActiveLevel()->addEntity(this);

        enablePhysics(true);

        //ResetPhysics();
        mParent = NULL;
    }
    else
    {
        Logger::Error("Entity::breakAway Trying to break away an entity that has no parent!");
    }
}

void Entity::addChild(Entity *child)
{
    //this should be a debug only check!
    EntityList::iterator it = mChildren.begin();
    const EntityList::iterator itEnd = mChildren.end();
    while (it != itEnd)
    {
        if((*it) == child)
        {
            Logger::Error("Entity::addChild Attempting to add duplicate child!!");
        }
        ++it;
    }
    
    mChildren.push_back(child);
    child->setParent(this);
    
}

void Entity::removeChild(Entity *child)
{
    EntityList::iterator it = mChildren.begin();
    while(it != mChildren.end())
    {
        if((*it) == child)
        {
            if(child->mParent != NULL)
            {
                child->setParent(NULL);
            }
            
            mChildren.erase(it);
            return;
        }
        
        ++it;
    }
    
    Logger::Error("Entity::removeChild Trying to remove a child that is not present");
}

void Entity::init(const PropertyCollection &propertyCollection, Entity* parent = NULL)
{
    DynamicObject::init();
  
    if(parent)
    {
        parent->addChild(this);
    }
    
    setScale(1.0f);
    m_transformationMatrixValid = false;
    STRING_ID renderableName = StringManager::UNDEFINED;
    STRING_ID entityGroupName = StringManager::UNDEFINED;
    STRING_ID physicsEntityName = StringManager::UNDEFINED;
    
    m_renderAssetName = false;
    m_renderCoordinateLines = false;
    
    int numberOfProperties = propertyCollection.getPropertyCount();
    for(int i=0; i < numberOfProperties; ++i)
    {
        const PropertyCollection::Property *property = propertyCollection.getPropertyAtIndex(i);
        if(property->m_name == RENDERABLE_NAME)
        {
            renderableName = property->m_data.StringIDValue;
        }
        else if(property->m_name == ENTITY_GROUP)
        {
            entityGroupName = property->m_data.StringIDValue;
        }
        else if(property->m_name == SHOW_DEBUG_AXIS)
        {
            m_renderCoordinateLines = property->m_data.intValue == 1 ? true : false;
        }
        else if(property->m_name == SHOW_DEBUG_NAME)
        {
            m_renderAssetName = property->m_data.intValue == 1 ? true : false;
        }
        else if(property->m_name == PHYSICS_TYPE)
        {
            mPhysicsType = property->m_data.StringIDValue;
        }
        else if(property->m_name == PHYSICS_ENTITY_NAME)
        {
            physicsEntityName = property->m_data.StringIDValue;
        }
    }
    
    if(renderableName != StringManager::UNDEFINED)
    {
        m_renderable = RenderableManager::getInstance()->getRenderable(renderableName);
    }
   
    if(entityGroupName != StringManager::UNDEFINED)
    {
        EntityFactory::getInstance()->createEntities(entityGroupName, this);
    }
    
    if(physicsEntityName != StringManager::UNDEFINED && PHYSICS_ENTITY == mPhysicsType)
    {
        //Physics objects ARE NOT child objects!
        mPhysicsObject = static_cast<PhysicsEntity*>(EntityFactory::getInstance()->createEntity(physicsEntityName, NULL));
        mPhysicsObject->setOwner(this);
    }
    
    if(m_renderCoordinateLines || Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_AXIS))
    {
        m_lineRenderable = static_cast<AxisRenderable*>(RenderableManager::getInstance()->getRenderable(StringManager::getIDForString("axisRenderable")));
    }
    
    if(m_renderAssetName || (Settings::getToggleSetting(Settings::SETTING_SHOW_OBJECT_NAMES)&& CLASS_ID_ENTITY == m_instanceName))
    {
        m_namePlate = static_cast<UIFontRenderable*>(RenderableManager::getInstance()->getRenderable(StringManager::getIDForString("uiAnadaleMonoRenderable3d")));
    }
    
    m_valid = true;
    
    std::string renderTargetIDStr = std::string("Instance: " + *StringManager::getStringFromID( m_instanceName ) +
                                                " Renderable: " + *StringManager::getStringFromID(renderableName));
    mRenderTargetID = StringManager::getIDForString(renderTargetIDStr);
    
    initPhysics();
}

void Entity::SetRenderable(RenderableObj* renderable)
{ 
    m_renderable = renderable;
}

void Entity::validateTransformationMatrix()
{
    if(m_transformationMatrixValid == false)
    {
        m_transformationMatrixValid = true;
        m_transformation = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation) * m_scale;
    }
}

//KKonecko: This may be something worth optimizing later.
void Entity::calculateGlobalTransformation(glm::mat4& globalTransformation)
{
    if(mParent != NULL)
    {
        mParent->calculateGlobalTransformation(globalTransformation);
    }
    
    globalTransformation *= getTransformation();
}

//KKonecko: This may be something worth optimizing later.
void Entity::calculateModelTransformation(glm::mat4& modelTransformation)
{
    if(mParent != NULL)
    {
        mParent->calculateModelTransformation(modelTransformation);
        modelTransformation *= getTransformation();
    }
}

const glm::mat4& Entity::getTransformation()
{
    validateTransformationMatrix();
    return m_transformation;
}

void Entity::release()
{
    if(m_valid == false)
    {
        Logger::Error("Release called on an unitiialized Entity");
    }
    
    m_valid = false;
    
    if(m_renderable != NULL)
    {
        m_renderable->release();
        m_renderable = NULL;
    }
    
    if(m_namePlate != NULL)
    {
        m_namePlate->release();
        m_namePlate = NULL;
    }

    if(m_lineRenderable != NULL)
    {
        m_lineRenderable ->release();
        m_lineRenderable = NULL;
    }

    if(PHYSICS_ENTITY == mPhysicsType && mPhysicsObject != NULL)
    {
        IPhysicsManager::getPhysicsManager()->removeEntity(this);
        mPhysicsObject->release();
        delete(mPhysicsObject);
        mPhysicsObject = NULL;
    }
    
    EntityList::iterator it = mChildren.begin();
    
    while(it != mChildren.end())
    {
        (*it)->release();
        delete(*it);
        it++;
    }
    
    DynamicObject::release();
}

void Entity::ResetPhysics()
{
    if(mPhysicsType == PHYSICS_ENTITY)
    {
        enablePhysics(false);
        mVelocity = glm::vec3(0.0,0.0,0.0);
        enablePhysics(true);
    }
}

void Entity::dispatchEvent(const Event& event) const
{
    if(mParent == NULL)
    {
        mEventDispatcher.dispatchEvent(event);
    }
    else
    {
        getRoot()->dispatchEvent(event);
    }
}

void Entity::registerForEventType(int eventType, IEventHandler* handler)
{
    if(mParent == NULL)
    {
        mEventDispatcher.registerForEventType(eventType, handler);
    }
    else
    {
        getRoot()->registerForEventType(eventType, handler);
    }
    
}

void Entity::unRegisterForEventType(int eventType, IEventHandler* handler)
{
    if(mParent == NULL)
    {
        mEventDispatcher.unRegisterForEventType(eventType, handler);
    }
    else
    {
        getRoot()->unRegisterForEventType(eventType, handler);
    }
}

void Entity::setControler(EntityControler* controler)
{
    if(mControler != NULL)
    {
        delete(mControler);
    }
    if(controler)
    {
        controler->setSlaveEntity(this);
    }
    mControler = controler;
}

void Entity::setActive(bool active)
{
    if(mParent == NULL)
    {
        if(mActive != active)
        {
            mActive = active;
            enablePhysics(active);
        }
    }
    else
    {
        getRoot()->setActive(active);
    }
}

bool Entity::isActive()const
{
    if(mParent == NULL)
    {
        return mActive;
    }
    else
    {
        return getRoot()->isActive();
    }
}

void Entity::enablePhysics(bool enable)
{
   if( mPhysicsType == PHYSICS_ENTITY && mPhsyicsAttached != enable)
   {
       if(enable && mActive)
       {
           IPhysicsManager::getPhysicsManager()->addEntity(this);
           mPhsyicsAttached = true;
       }
       else
       {
           IPhysicsManager::getPhysicsManager()->removeEntity(this);
           mPhsyicsAttached = false;
       }
   }
}

void Entity::teleportEntity(const glm::vec3& position, const glm::quat& rotation)
{
    //this needs work, we can't remove and add the same entaty to the physics engine!
    if(mPhsyicsAttached)
    {
        enablePhysics(false);
    }
}

void Entity::lookAt(const glm::vec3& targetPos, const glm::vec3& up, float percent)
{
    lookAt(targetPos, getRotation(), up, percent);
}

void Entity::lookAt(const glm::vec3& targetPos, const glm::quat& orignalRotation, const glm::vec3& up,  float percent)
{
	glm::vec3 cameraDirection(m_position - targetPos);
	cameraDirection = glm::normalize(cameraDirection);
	
	glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0,1,0), cameraDirection));
	glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	glm::mat4 cs(cameraRight.x, cameraRight.y, cameraRight.z, 0,
		cameraUp.x, cameraUp.y, cameraUp.z, 0,
		cameraDirection.x, cameraDirection.y, cameraDirection.z, 0,
		0, 0, 0, 1);

	glm::quat rotation_quat = glm::normalize(glm::quat(cs));

    if(percent < 1)
    {
        rotation_quat = glm::mix3(orignalRotation, rotation_quat, percent);
    }

	
    SetRotation(rotation_quat);
}

void Entity::lookAt(const glm::vec3& targetPos, float percent)
{
    lookAt(targetPos, getRotation(), percent);
}

void Entity::lookAt(const glm::vec3& targetPos, const glm::quat& orignalRotation, float percent)
{
    if(mPhsyicsAttached)
    {
        //Logger::Error("Using lookat while attached to physics");
    }
    
    const glm::vec3 v1 = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    const glm::vec3 v2 = glm::normalize(m_position - targetPos);
    const glm::vec3 rotation_axis = glm::normalize(glm::cross(v1, v2));
    const float angle = glm::degrees(glm::acos(glm::dot(v1, v2)));
    glm::quat rotation_quat = glm::angleAxis(angle, rotation_axis);
    rotation_quat = glm::normalize(rotation_quat);
    
    if(percent < 1)
    {
        rotation_quat = glm::mix3(orignalRotation, rotation_quat, percent);
    }
    
    SetRotation(rotation_quat);
}