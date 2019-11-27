#include "Core/Entity/Entities/PhysicsEntity.h"
#include "Core/Entity/Entities/Camera.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"
#include "Core/Settings/Settings.h"
#include "Core/LevelFactory/LevelFactory.h"
#include "Core/Events/GameEvents.h"
#include "Core/Rendering/MeshManager/Mesh.h"
#include "Core/Rendering/Renderables/Model.h"

CONST_STRING_DEF(PhysicsEntity, CLASS_ID_PHYSICS_ENTITY)
CONST_STRING_DEF(PhysicsEntity, CENTER_OF_MASS_X)
CONST_STRING_DEF(PhysicsEntity, CENTER_OF_MASS_Y)
CONST_STRING_DEF(PhysicsEntity, CENTER_OF_MASS_Z)
CONST_STRING_DEF(PhysicsEntity, MASS)
CONST_STRING_DEF(PhysicsEntity, RIGID_BODY_TYPE)
CONST_STRING_DEF(PhysicsEntity, RIGID_BODY_TYPE_CONTINIOUS)
CONST_STRING_DEF(PhysicsEntity, RIGID_BODY_TYPE_NORMAL)
CONST_STRING_DEF(PhysicsEntity, RIGID_BODY_TYPE_STATIC)
CONST_STRING_DEF(PhysicsEntity, RIGID_BODY_TYPE_CHARACTER_CONTROLLER)

CONST_STRING_DEF(PhysicsEntity, FRICTION)
CONST_STRING_DEF(PhysicsEntity, ROLLING_FRICTION)
CONST_STRING_DEF(PhysicsEntity, RESTITUTION)
CONST_STRING_DEF(PhysicsEntity, LINEAR_SLEEPING_THRESHOLD)
CONST_STRING_DEF(PhysicsEntity, ANGULAR_SLEEPING_THRESHOLD)
CONST_STRING_DEF(PhysicsEntity, LINEAR_DAMPING)
CONST_STRING_DEF(PhysicsEntity, ANGULAR_DAMPING)
CONST_STRING_DEF(PhysicsEntity, BREAK_AWAY_MODE)
CONST_STRING_DEF(PhysicsEntity, ON_ANY_IMPACT)
CONST_STRING_DEF(PhysicsEntity, NEVER)
CONST_STRING_DEF(PhysicsEntity, DAMAGE_ON_IMPACT)
CONST_STRING_DEF(PhysicsEntity, DESTROY)
CONST_STRING_DEF(PhysicsEntity, NONE)

CONST_STRING_DEF(PhysicsEntity, SHAPE_TYPE)
CONST_STRING_DEF(PhysicsEntity, BOX)
CONST_STRING_DEF(PhysicsEntity, BOX2D)
CONST_STRING_DEF(PhysicsEntity, TRIANGLE)
CONST_STRING_DEF(PhysicsEntity, CAPSULE)
CONST_STRING_DEF(PhysicsEntity, BOX_DEPRICATED)
CONST_STRING_DEF(PhysicsEntity, CYLINDER)
CONST_STRING_DEF(PhysicsEntity, CONE)
CONST_STRING_DEF(PhysicsEntity, SPHERE)
CONST_STRING_DEF(PhysicsEntity, BHV_TRIANGLE_MESH)
CONST_STRING_DEF(PhysicsEntity, CONVEX_HULL)
CONST_STRING_DEF(PhysicsEntity, CONVEX_TRIANGLEMESH_SHAPE)

bool PhysicsEntity::RENDER_PHYSICS_OBJECTS = false;

PhysicsEntity::PhysicsEntity():Entity(CLASS_ID_PHYSICS_ENTITY), mBreakAwayMode(ON_ANY_IMPACT),
mDamageOnImpact(NONE), mOwner(NULL), mShapeType(BOX_DEPRICATED)
{
}

PhysicsEntity::~PhysicsEntity()
{
}

const Mesh* PhysicsEntity::GetMesh() const
{
	if (hasRenderable())
	{
		if (m_renderable->getClassID() == Model::CLASS_ID_MODEL_RENDERABLE)
		{
			const Model* modelRenderable = static_cast<const Model*>(m_renderable);
			return modelRenderable->getMesh();
		}
	}

	return NULL;
}

void PhysicsEntity::applyPhysicsTransformation(const glm::mat4 &tranformation, float deltaTime)
{
    glm::vec3 pos = mOwner->getPosition();
    mOwner->SetPosition(glm::vec3(tranformation[3][0], tranformation[3][1], tranformation[3][2]));
    pos = mOwner->getPosition() - pos;
    mOwner->setVelocity(pos * (1.0f / deltaTime));
    
    mOwner->SetRotation(glm::toQuat(tranformation));
}


void PhysicsEntity::calculateGlobalTransformation(glm::mat4& globalTransformation)
{
    if(mOwner != NULL)
    {
        mOwner->calculateGlobalTransformation(globalTransformation);
    }
    
    globalTransformation = globalTransformation * glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation);
}

void PhysicsEntity::calculateModelTransformation(glm::mat4& modelTransformation)
{
    if(mOwner != NULL)
    {
        mOwner->calculateModelTransformation(modelTransformation);
        modelTransformation = modelTransformation * glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation);
    }
}

void PhysicsEntity::getTransformationNoScale(glm::mat4& transformation)const 
{
    transformation = glm::translate(glm::mat4(1.0f), m_position) * glm::toMat4(m_rotation);
}

void PhysicsEntity::init(const PropertyCollection &propertyCollection, Entity* parent)
{
    Entity::init(propertyCollection, parent);
    
    mOwner = NULL;
    mMass = 1.0f;
    mFriction = 0.5f;
    mRollingFriction = 0.0f;
    mRestitution = 0.0f;
    mLinearSleepingThreshold = 0.8f;
    mAngularSleepingThreshold = 1.0f;
    mLinearDamping = 0.0f;
    mAngularDamping = 0.0f;
    
    mCenterOfMass = glm::vec3(0.,0.,0.);
    mRigidBodyType = RIGID_BODY_TYPE_NORMAL;
    mBreakAwayMode = ON_ANY_IMPACT;
 
    propertyCollection.getValueForProperty(CENTER_OF_MASS_X, mCenterOfMass.x);
    propertyCollection.getValueForProperty(CENTER_OF_MASS_Y, mCenterOfMass.y);
    propertyCollection.getValueForProperty(CENTER_OF_MASS_Z, mCenterOfMass.z);
    propertyCollection.getValueForProperty(MASS, mMass);
    propertyCollection.getValueForProperty(RIGID_BODY_TYPE, mRigidBodyType);
    propertyCollection.getValueForProperty(FRICTION, mFriction);
    propertyCollection.getValueForProperty(ROLLING_FRICTION, mRollingFriction);
    propertyCollection.getValueForProperty(RESTITUTION, mRestitution);
    propertyCollection.getValueForProperty(LINEAR_SLEEPING_THRESHOLD, mLinearSleepingThreshold);
    propertyCollection.getValueForProperty(ANGULAR_SLEEPING_THRESHOLD, mAngularSleepingThreshold);
    propertyCollection.getValueForProperty(LINEAR_DAMPING, mLinearDamping);
    propertyCollection.getValueForProperty(ANGULAR_DAMPING, mAngularDamping);
    propertyCollection.getValueForProperty(BREAK_AWAY_MODE, mBreakAwayMode);
    propertyCollection.getValueForProperty(DAMAGE_ON_IMPACT, mDamageOnImpact);
	propertyCollection.getValueForProperty(SHAPE_TYPE, mShapeType);
}

void PhysicsEntity::onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView)
{
    if(RENDER_PHYSICS_OBJECTS == true || Settings::getToggleSetting(Settings::SETTING_SHOW_PHYSICS_VOLUME_OBJECTS))
    {
        renderable.onRender(renderer, modelViewProjectionMatrix, modelView);
    }
}

void PhysicsEntity::OnImpact(void* collisionOwner, PhysicsEntity* otherCollisionParty)
{
	if (otherCollisionParty->GetRigidBodyType() == PhysicsEntity::RIGID_BODY_TYPE_CHARACTER_CONTROLLER) {
		return;
	}

    Entity* collisionOwnerEntity = static_cast<Entity*>(collisionOwner);

    PhysicsEntity* collisionOwnerPhysicsEntity = static_cast<PhysicsEntity*>(collisionOwnerEntity->getPhysicsObject());
    		
	if (mRigidBodyType == RIGID_BODY_TYPE_CHARACTER_CONTROLLER) {
		return;
	}

    if(collisionOwnerPhysicsEntity->mBreakAwayMode == ON_ANY_IMPACT && collisionOwnerEntity->getParent() != NULL)
    {
        collisionOwnerEntity->breakAway();
    }
    
    if(collisionOwnerPhysicsEntity->mDamageOnImpact == DESTROY)
    {
        collisionOwnerEntity->setActive(false);
    }
}

bool PhysicsEntity::isStatic() const
{
    return mRigidBodyType == RIGID_BODY_TYPE_STATIC;
}

bool PhysicsEntity::isRootPhysicsEntity()
{
    if(mOwner->getRoot()->getPhysicsObject() == this)
    {
        return true;
    }
    
    return false;
}

const glm::vec3 PhysicsEntity::GetCenterOfMass(bool total)
{
    if(total == false)
    {
        return mCenterOfMass;
    }
    
    if(isRootPhysicsEntity())
    {
        glm::vec3 centerOfMass = mCenterOfMass;
        float totalMass = mMass;
        _calculateCenterOfMass(centerOfMass, totalMass);
   
        return centerOfMass;
    }
    
    return mOwner->getRoot()->getPhysicsObject()->GetCenterOfMass(total);
}

void PhysicsEntity::_calculateCenterOfMass(glm::vec3& centerOfMass, float& totalMass)
{
    EntityList& entityList = mOwner->getChildren();
    EntityList::iterator it  = entityList.begin();
    const EntityList::iterator end = entityList.end();
    
    while(it != end)
    {
        PhysicsEntity* physicsEntity = (*it)->getPhysicsObject();
        if(physicsEntity != NULL)
        {
            
            float localMass = physicsEntity->GetMass();
            if(localMass > 0)
            {
                totalMass += localMass;
                float percentageOfTotalMass = localMass / totalMass;
                glm::vec3 localCenterOfMass = physicsEntity->GetCenterOfMass();

                if(localCenterOfMass != centerOfMass)
                {
                    glm::vec3 delta = localCenterOfMass - centerOfMass;
                    delta = glm::normalize(delta);
                    float distance = glm::distance(localCenterOfMass, centerOfMass);
                    centerOfMass += delta * (distance * percentageOfTotalMass);
                }
            }
            
            physicsEntity->_calculateCenterOfMass(centerOfMass, totalMass);
        }
        ++it;
    }
}

void PhysicsEntity::GetForce(glm::vec3& force, glm::vec3& relativePoint)
{
    GameEvents::CalculateImpulseEvent::ImpulseAggregator impulseAggregator;
    mOwner->dispatchEvent(GameEvents::CalculateImpulseEvent(&impulseAggregator));
    
    force = impulseAggregator.mForce;;
    relativePoint = impulseAggregator.mRelativePoint;
}

void PhysicsEntity::GetDesiredMovement(CharacterMovementInput& input)
{	
	mOwner->dispatchEvent(GameEvents::RequestCharacterInputEvent(input));
}