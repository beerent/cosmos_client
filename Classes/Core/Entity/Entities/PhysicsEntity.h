//
//  PhysicsEntity.h
//  Projectios1
//
//  Created by Kamil Konecko on 6/29/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef Projectios1_PhysicsEntity_h
#define Projectios1_PhysicsEntity_h

#include "Core/Entity/Entities/Entity.h"
#include "Core/Input/InputDataTypes.h"

class UIFontRenderable;
class AxisRenderable;
class Mesh;

class PhysicsEntity : public Entity
{
friend class PhysicsManager;
    
public:
    CONST_STRING_DEC(CLASS_ID_PHYSICS_ENTITY)
    CONST_STRING_DEC(CENTER_OF_MASS_X)
    CONST_STRING_DEC(CENTER_OF_MASS_Y)
    CONST_STRING_DEC(CENTER_OF_MASS_Z)
    CONST_STRING_DEC(MASS)

	CONST_STRING_DEC(SHAPE_TYPE)
	CONST_STRING_DEC(BOX_DEPRICATED)
	CONST_STRING_DEC(BOX)
    CONST_STRING_DEC(BOX2D)
	CONST_STRING_DEC(TRIANGLE)
	CONST_STRING_DEC(CYLINDER)
    CONST_STRING_DEC(CAPSULE)
	CONST_STRING_DEC(CONE)
    CONST_STRING_DEC(SPHERE)
	CONST_STRING_DEC(BHV_TRIANGLE_MESH)
	CONST_STRING_DEC(CONVEX_HULL)
	CONST_STRING_DEC(CONVEX_TRIANGLEMESH_SHAPE)

    CONST_STRING_DEC(RIGID_BODY_TYPE)
    CONST_STRING_DEC(RIGID_BODY_TYPE_CONTINIOUS)
    CONST_STRING_DEC(RIGID_BODY_TYPE_NORMAL)
    CONST_STRING_DEC(RIGID_BODY_TYPE_STATIC)
	CONST_STRING_DEC(RIGID_BODY_TYPE_CHARACTER_CONTROLLER)

    CONST_STRING_DEC(FRICTION)
    CONST_STRING_DEC(ROLLING_FRICTION)
    CONST_STRING_DEC(RESTITUTION)
    CONST_STRING_DEC(LINEAR_SLEEPING_THRESHOLD)
    CONST_STRING_DEC(ANGULAR_SLEEPING_THRESHOLD)
    CONST_STRING_DEC(LINEAR_DAMPING)
    CONST_STRING_DEC(ANGULAR_DAMPING)
    
    CONST_STRING_DEC(BREAK_AWAY_MODE)
    CONST_STRING_DEC(ON_ANY_IMPACT)
    CONST_STRING_DEC(NEVER)

    CONST_STRING_DEC(DAMAGE_ON_IMPACT)
    CONST_STRING_DEC(DESTROY)
    CONST_STRING_DEC(NONE)
    
    
    static bool RENDER_PHYSICS_OBJECTS;
    
    PhysicsEntity();
    ~PhysicsEntity();
    
    virtual void applyPhysicsTransformation(const glm::mat4 &tranformation, float deltaTime);

    virtual void calculateGlobalTransformation(glm::mat4& globalTransformation);
    virtual void calculateModelTransformation(glm::mat4& modelTransformation);

    virtual void getTransformationNoScale(glm::mat4& transformation) const;

    virtual void init(PropertyCollection const &propertyCollection, Entity* parent = NULL);

    STRING_ID GetRigidBodyType()const {return mRigidBodyType;};
	STRING_ID GetShapeType() const { return mShapeType; };

    float GetMass()const {return mMass;};
    
    void OnImpact(void* collisionOwner, PhysicsEntity* otherCollisionParty);
    bool CanCauseImpact(){return mMass;};
    
    const glm::vec3 GetCenterOfMass(bool total = false);

    float GetFriction(){return mFriction;};
    float GetRollingFriction(){return mRollingFriction;};
    float GetRestitution(){return mRestitution;};
    float GetLinearSleepingThreshold(){return mLinearSleepingThreshold;};
    float GetAngularSleepingThreshold(){return mAngularSleepingThreshold;};
    float GetLinearDamping(){return mLinearDamping;};
    float GetAngularDamping(){return mAngularDamping;};
    
    void GetForce(glm::vec3& force, glm::vec3& relativePoint);
	void GetDesiredMovement(CharacterMovementInput& input);
    
    virtual void onRender(Renderer& renderer, RenderableObj& renderable, const glm::mat4& modelViewProjectionMatrix, const glm::mat4& modelView);

    bool isRootPhysicsEntity();
    bool isStatic() const;
    
    void setOwner(Entity* owner){mOwner = owner;}
	Entity* getOwner() { return mOwner; }

	const Mesh* GetMesh() const;
    
protected:
    
    void _calculateCenterOfMass(glm::vec3& centerOfMass, float& totalMass);
    
    Entity* mOwner;
    
private:
    float mMass;
    float mFriction;
    float mRollingFriction;
    float mRestitution;
    float mLinearSleepingThreshold;
    float mAngularSleepingThreshold;
    float mLinearDamping;
    float mAngularDamping;
    
    glm::vec3 mCenterOfMass;
    STRING_ID mRigidBodyType;
    STRING_ID mBreakAwayMode;
    STRING_ID mDamageOnImpact;
	STRING_ID mShapeType;
 };
#endif
