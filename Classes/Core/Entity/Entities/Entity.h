//
//  Entity.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Projectios1_Entity_h
#define Projectios1_Entity_h

#include <list>
#include "Core/OpenSource/Vector.hpp"
#include "Core/Rendering/Renderables/RenderableObj.h"
#include "glm/gtc/quaternion.hpp"
#include "Core/Memory/DynamicObject.h"
#include "Core/StringManager/StringManager.h"
#include "Core/Rendering/IRenderTarget.h"
#include "Core/Events/EventDispatcher.h"

class UIFontRenderable;
class AxisRenderable;
class PhysicsEntity;
class EntityControler;

class Entity : public DynamicObject, public IRenderTarget, public IEventHandler
{
public:
    CONST_STRING_DEC(RENDERABLE_NAME)
    CONST_STRING_DEC(CLASS_ID_ENTITY)
    CONST_STRING_DEC(ENTITY_GROUP)
    CONST_STRING_DEC(SHOW_DEBUG_AXIS)
    CONST_STRING_DEC(SHOW_DEBUG_NAME)
    CONST_STRING_DEC(PHYSICS_TYPE)
    CONST_STRING_DEC(PHYSICS_ENTITY)
    CONST_STRING_DEC(PHYSICS_NONE)
    CONST_STRING_DEC(PHYSICS_ENTITY_NAME)
    
    static bool SHOW_ALL_NAMES;
    
    Entity();
    Entity(StringManager::StringID classIdType);
    virtual ~Entity();
    
    typedef std::list<Entity*> EntityList;
    
    friend class EntityFactory;
 
    Entity* getParent() const {return mParent;};
    EntityList& getChildren() {return mChildren;}
    
    void addChild(Entity* child);
    void removeChild(Entity* child);
    
    void breakAway();
    
    //This is not ideal, entity should not know anything about physics objects.
    virtual PhysicsEntity* getPhysicsObject(){return mPhysicsObject;}
    
    virtual Entity* getRoot();
    virtual const Entity* getRoot() const;
    
    virtual void setScale(float scale);
    virtual void setScale(const glm::vec3&  scale);
    virtual void setVelocity(const glm::vec3& velocity);

    virtual void SetRotation(const glm::quat &rotation);
    virtual void SetPosition(const glm::vec3 &position);
    virtual void SetPosition(const glm::vec4 &position);
    
    virtual void SetRenderable(RenderableObj *renderable);

    virtual const glm::vec3& getPosition()const {return m_position;};
    virtual const glm::quat& getRotation()const {return m_rotation;};
    virtual const glm::mat4& getScale()   const {return m_scale;};
    virtual const glm::vec3& getScaleVec()const {return m_scaleVec;};
    virtual const glm::vec3& getVelocity()const {return mVelocity;};
    
    virtual const glm::mat4& getTransformation();
    virtual void calculateGlobalTransformation(glm::mat4& globalTransformation);
    virtual void calculateModelTransformation(glm::mat4& modelTransformation);

    virtual void init(PropertyCollection const &propertyCollection, Entity* parent);
    virtual void onBeginRender(Renderer& renderer);
    virtual void onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);

    virtual StringManager::StringID getRenderTargetID(){return mRenderTargetID;}

    virtual void release();
    void init();
    
    void ResetPhysics();
    
    AxisRenderable *m_lineRenderable;
    
    void dispatchEvent(const Event& event) const;
    void registerForEventType(int eventType, IEventHandler* handler);
    void unRegisterForEventType(int eventType, IEventHandler* handler);
    
    void OnHandleEvent(const Event& event){};
    
    //Displayed in 3d when debug text is enabled
    void setDebugText(STRING_ID text){m_debugText = text;}
    
    STRING_ID getGroupInstanceID(){return mGroupInstanceID;}
    

    void setControler(EntityControler* controler);
    EntityControler* getControler(){return mControler;}
    
    void setActive(bool active);
    bool isActive()const;
    
    void teleportEntity(const glm::vec3& position, const glm::quat& rotation);
    
    void lookAt(const glm::vec3& targetPos, float percent = 1.0);
    void lookAt(const glm::vec3& targetPos, const glm::quat& orignalRotation, float percent = 1.0);

    void lookAt(const glm::vec3& targetPos, const glm::vec3& up, float percent = 1.0);
    void lookAt(const glm::vec3& targetPos, const glm::quat& orignalRotation, const glm::vec3& up, float percent = 1.0);

protected:
    
    
    EntityControler* mControler;
    STRING_ID mGroupInstanceID;
    STRING_ID mRenderTargetID;
    glm::mat4 m_scale;
    glm::vec3 m_scaleVec;
    glm::vec3 m_position;
    glm::quat m_rotation;
    glm::mat4 m_transformation;
    glm::vec3 mVelocity;
    bool m_transformationMatrixValid;
    
    RenderableObj* m_renderable;
    UIFontRenderable * m_namePlate;
    void validateTransformationMatrix();
    
    bool m_renderCoordinateLines;
    bool m_renderAssetName;
    bool m_valid;
    STRING_ID mPhysicsType;
    
    EntityList mChildren;
    Entity* mParent;
    PhysicsEntity* mPhysicsObject;
    
    void setParent(Entity* entity);

	bool hasRenderable() const;
    virtual void addVisibleRenderables(Renderer& renderer);
    virtual void initPhysics();
    virtual void onRootChanged(Entity* previousParent, Entity* newParent){};
    
private:
    
    bool mActive;
    bool mPhsyicsAttached;
    
    STRING_ID m_debugText;

    EventDispatcher mEventDispatcher;
    void processRootChanged(Entity* previousParent, Entity* newParent);
    void enablePhysics(bool enable);
    
};
#endif
