//
//  PhysicsManager.cpp
//  Projectios1
//
//  Created by Kamil Konecko on 2/17/14.
//
//
#include "Core/Physics/PhysicsManager.h"
#include "Core/Physics/PhysicsDebugDrawer.h"
#include "Core/Entity/Entities/PhysicsEntity.h"
#include "Core/Debug/Logger/Logger.h"
#include "Core/Settings/Settings.h"
#include <iostream>
#include "Core/Debug/DebugPrintUtil.h"
#include "Core/Rendering/MeshManager/Mesh.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionShapes/btTriangleShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"
#include "btBulletDynamicsCommon.h"
#include "Core/Physics/BulletCharacterController.h"
#include "Core/Physics/CollisionContactDelegate.h"
#include "Core/Physics/MathGLMBulletConversionHelpers.h"
#include <future>

///create 125 (5x5x5) dynamic object
#define ARRAY_SIZE_X 5
#define ARRAY_SIZE_Y 5
#define ARRAY_SIZE_Z 5

//maximum number of objects (and allow user to shoot additional boxes)
#define MAX_PROXIES (ARRAY_SIZE_X*ARRAY_SIZE_Y*ARRAY_SIZE_Z + 1024)

///scaling of the objects (0.1 = 20 centimeter boxes )
#define SCALING 1.
#define START_POS_X 0
#define START_POS_Y 0
#define START_POS_Z 0

PhysicsManager* PhysicsManager::mInstance = NULL;
typedef std::map < btCollisionObject*, int >::iterator CBI_iterator;

CollisionContactDelegate gCollisionContactDelegate;

bool MyContactCallback (btManifoldPoint& cp,
                        const btCollisionObjectWrapper* colObj0Wrap,
                        int partId0,
                        int index0,
                        const btCollisionObjectWrapper* colObj1Wrap,
                        int partId1,
                        int index1)
{
	gCollisionContactDelegate.insertCollisionContact(colObj0Wrap->getCollisionObject(), partId0, index0, 
													 colObj1Wrap->getCollisionObject(), partId1, index1);
    return false;
}

///The MyOverlapCallback is used to show how to collect object that overlap with a given bounding box defined by aabbMin and aabbMax.
///See m_dynamicsWorld->getBroadphase()->aabbTest.
struct	MyOverlapCallback : public btBroadphaseAabbCallback
{
	btVector3 m_queryAabbMin;
	btVector3 m_queryAabbMax;
	
	int m_numOverlap;
	MyOverlapCallback(const btVector3& aabbMin, const btVector3& aabbMax ) : m_queryAabbMin(aabbMin),m_queryAabbMax(aabbMax),m_numOverlap(0)	{}
	virtual bool	process(const btBroadphaseProxy* proxy)
	{
		btVector3 proxyAabbMin,proxyAabbMax;
		btCollisionObject* colObj0 = (btCollisionObject*)proxy->m_clientObject;
		colObj0->getCollisionShape()->getAabb(colObj0->getWorldTransform(),proxyAabbMin,proxyAabbMax);
		if (TestAabbAgainstAabb2(proxyAabbMin,proxyAabbMax,m_queryAabbMin,m_queryAabbMax))
		{
			m_numOverlap++;
		}
		return true;
	}
};

PhysicsManager::PhysicsManager() : mInitialized(false) {
    mInstance = this;
    m_physicsManager = this;
}

PhysicsDebugDrawer* PhysicsManager::getPhysicsDebugDrawer()
{
    return m_debugDrawer;
}

PhysicsManager::~PhysicsManager()
{
    if(mInitialized)
    {
        shutDown();
    }
}

void PhysicsManager::shutDown()
{
    waitUntilPhysicsIsComplete();
    delete m_collisionConfiguration;
    delete m_dispatcher;
    delete m_broadphase;
    delete m_solver;
    delete m_debugDrawer;
    mInitialized  = false;
    //delete m_dynamicsWorld;
    // LEAK LEAK LEAK
}

bool PhysicsManager::doPhysicsThread()
{
	mInstance->physicsThreadStep();
	return true;
}

void PhysicsManager::physicsThreadStep() {
	float dt = mPhysicsThreadData.mDeltaTime;
	for (auto it = mCharacterControllers.begin(); it != mCharacterControllers.end(); ++it) {
		updateCharacterController(it->second);
	}
	m_dynamicsWorld->stepSimulation(dt);// , 0, 0.03);
}

void PhysicsManager::updateCharacterController(CharacterControlerPhysicsData& characterControllerData) {
	btMatrix3x3 oriantation = characterControllerData.mGhostObject->getWorldTransform().getBasis();

	oriantation *= btMatrix3x3(btQuaternion(btVector3(0, 1, 0), characterControllerData.mCharacterMovementInput.m_turnRight * -0.1));
	characterControllerData.mGhostObject->getWorldTransform().setBasis(oriantation);

	btTransform worldTransform = characterControllerData.mGhostObject->getWorldTransform();
	btVector3 forwardDirection = worldTransform.getBasis()[2];
	btVector3 upDirection = worldTransform.getBasis()[1];
	btVector3 rightDirection = worldTransform.getBasis()[0];
	forwardDirection[0] = -forwardDirection[0];
	forwardDirection.normalize();
	upDirection.normalize();
	rightDirection.normalize();

	btVector3 walkDirection(0.0, 0.0, 0.0);
	btScalar walkVelocity(characterControllerData.mCharacterMovementInput.m_moveForward);
	btScalar walkSpeed = walkVelocity / 10.0;

	characterControllerData.mCharacter->setWalkDirection(btVector3(0.0, 0.001, 0.0) + forwardDirection * walkSpeed);
}

void PhysicsManager::initialize(const glm::vec3& gravity, bool hasGround)
{    
    gContactAddedCallback = &MyContactCallback;

   	///collision configuration contains default setup for memory, collision setup
	m_collisionConfiguration = new btDefaultCollisionConfiguration();

    m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);
    
	//m_broadphase = new btDbvtBroadphase();
	btVector3 worldMin(-1000, -1000, -1000);

	btVector3 worldMax(1000, 1000, 1000);

	m_broadphase = new btAxisSweep3(worldMin, worldMax);

    ///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
	btSequentialImpulseConstraintSolver* sol = new btSequentialImpulseConstraintSolver;
	m_solver = sol;
    
    m_debugDrawer = new PhysicsDebugDrawer();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	m_dynamicsWorld->setDebugDrawer(m_debugDrawer);
	
    m_dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));

	//What is this? Its in the character controller demo.
	m_dynamicsWorld->getDispatchInfo().m_allowedCcdPenetration = 0.0001f;
	m_broadphase->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

	btCollisionShape* groundShape = m_factory.createGroundShape();	    
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-100,0));
    
	//We can also use DemoApplication::localCreateRigidBody, but for clarity it is provided here:
	if (hasGround)
	{
		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		m_dynamicsWorld->addRigidBody(body);
	}

	gCollisionContactDelegate.setActiveCollisionObjectReverseMap(*this);
    mInitialized = true;
}

void PhysicsManager::waitUntilPhysicsIsComplete()
{
	if (m_physicsJobFuture.valid()) {
		m_physicsJobFuture.get();
	}
}

void PhysicsManager::addEntity(Entity* entity)
{
    mAddQueue.push_back(entity);
}

void PhysicsManager::removeEntity(Entity* entity)
{
    PhysicsEntity* physicsEntity = static_cast<PhysicsEntity*>(entity->getPhysicsObject());
    
    if(physicsEntity == NULL)
    {
        return;
    }

	if (physicsEntity->GetRigidBodyType() == PhysicsEntity::RIGID_BODY_TYPE_CHARACTER_CONTROLLER) {
		removePhysicsEntityCharacterController(physicsEntity);
	}
	else 
	{
		if (physicsEntity->isRootPhysicsEntity())
		{
			removeRootPhysicsEntity(physicsEntity);
		}
		else
		{
			removePhysicsEntity(physicsEntity, entity);
		}
	}
}

void PhysicsManager::addPhysicsCharacterController(Entity *entity) {
	PhysicsEntity* physicsEntity = static_cast<PhysicsEntity*>(entity->getPhysicsObject());

	if (physicsEntity == NULL) {
		return;
	}

	btTransform transofrm;
	glm::mat4 glmTransofrm(1.0);
	entity->calculateGlobalTransformation(glmTransofrm);
	convertGLMtoBTTransform(glmTransofrm, transofrm);

	//leak
	btPairCachingGhostObject* ghostObject = new btPairCachingGhostObject();
	ghostObject->setWorldTransform(transofrm);	
	btConvexShape *compoundShape = NULL;

	//Note because btKinematicCharacterController requires a convex shape, there is no way to use a compound shape as a mechanism 
	//for setting an offset
	if (false == physicsEntity->hasRenderable()) {
		Entity::EntityList::iterator entityIt = physicsEntity->getChildren().begin();
		for (; entityIt != physicsEntity->getChildren().end(); ++entityIt) {

			Entity* pentity = *entityIt;
			PhysicsEntity* subPhysicsEntity = static_cast<PhysicsEntity*>(pentity);

			if (subPhysicsEntity->hasRenderable()) {
				compoundShape = static_cast<btConvexShape*>(m_factory.createShape(subPhysicsEntity, entity));
				break;
			}
		}
	}
	else {
		Logger::Error("PhysicsManager::addPhysicsCharacterController Physics character controllers require physics objects using entity groups with only one convex shape");
	}

	ghostObject->setCollisionShape(compoundShape);
	ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT | btCollisionObject::CF_NO_CONTACT_RESPONSE);

	btScalar stepHeight = btScalar(0.35);

	//leak
	BulletCharacterController* character = new BulletCharacterController(ghostObject, compoundShape, stepHeight);
	m_dynamicsWorld->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter | btBroadphaseProxy::DefaultFilter);
	m_dynamicsWorld->addAction(character);
	CharacterControlerPhysicsData& data = mCharacterControllers[entity];
	
	data.mGhostObject = ghostObject;
	data.mCharacter = character;
	data.mCharacterEntity = entity;
}

void PhysicsManager::_addPhysicsEntity(Entity* entity)
{
    //create a few dynamic rigidbodies
    // Re-using the same collision is better for memory usage and performance
    
    btTransform transofrm;

    btVector3 centerMassVector;
    int entityCount = 0;

    btScalar	mass;
    
    PhysicsEntity* physicsEntity = static_cast<PhysicsEntity*>(entity->getPhysicsObject());
    
    if(physicsEntity == NULL)
    {
        return;
    }
    
	if (physicsEntity->GetRigidBodyType() == PhysicsEntity::RIGID_BODY_TYPE_CHARACTER_CONTROLLER) {
		addPhysicsCharacterController(entity);
		return;
	}

    mass = physicsEntity->GetMass();
    
    //rigidbody is dynamic if and only if mass is non zero, otherwise static
    bool isDynamic = !physicsEntity->isStatic();
    
    Logger::Log(Logger::MATH_3D, std::string("PhysicsManager::_addPhysicsEntity"));
    
    const glm::vec3 centerVec = physicsEntity->GetCenterOfMass(true);
    
    m_entityToCenterOfMassMap[physicsEntity] = centerVec;
    
    //DebugPrintUtil::printGLVector(centerVec);
    centerMassVector.setX(centerVec.x);
    centerMassVector.setY(centerVec.y);
    centerMassVector.setZ(centerVec.z);
    
    btMatrix3x3 identity;
    identity.setIdentity();
     
    btTransform centerMassTransform(identity, centerMassVector);
    btTransform centerMassTransformInverse(identity, centerMassVector);
    centerMassTransformInverse = centerMassTransformInverse.inverse();

    glm::mat4 glmTransofrm(1.0);
    entity->calculateGlobalTransformation(glmTransofrm);
    convertGLMtoBTTransform(glmTransofrm, transofrm);
    
    //DebugPrintUtil::printGlTransform(glmTransofrm);
    
	btCollisionShape *collisionShape = NULL;
	if (physicsEntity->hasRenderable())
	{
		collisionShape = m_factory.createShape(physicsEntity, entity);		
		btVector3 scale;
		const glm::vec3& scaleGLM = entity->getScaleVec();
		scale.setX(scaleGLM.x);
		scale.setY(scaleGLM.y);
		scale.setZ(scaleGLM.z);
		collisionShape->setLocalScaling(scale);
	}
	else
	{
		btCompoundShape* compoundShape = m_factory.createCompoundShape();
		collisionShape = compoundShape;
		btTransform scaleMat;
		convertGLMtoBTTransform(entity->getScale(), scaleMat);
        scaleMat = centerMassTransform * scaleMat;
        _addSubPhysicsEntity(entity, compoundShape, scaleMat, entityCount, mass, isDynamic);
	}
    
    btVector3 localInertia(0.0f,0.f,0.f);
    if (isDynamic)
    {
        collisionShape->calculateLocalInertia(mass,localInertia);
    }

    //using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
    btDefaultMotionState* myMotionState = new btDefaultMotionState(transofrm * centerMassTransformInverse);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,collisionShape,localInertia);
    
    rbInfo.m_rollingFriction = physicsEntity->GetRollingFriction();
    rbInfo.m_friction = physicsEntity->GetFriction();
    rbInfo.m_linearDamping = physicsEntity->GetLinearDamping();
    rbInfo.m_linearSleepingThreshold = physicsEntity->GetLinearSleepingThreshold();
    rbInfo.m_angularDamping = physicsEntity->GetAngularDamping();
    rbInfo.m_angularSleepingThreshold = physicsEntity->GetAngularSleepingThreshold();
    rbInfo.m_restitution = physicsEntity->GetRestitution();
    
	btRigidBody* body = m_factory.createRigidBody(rbInfo);
    body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    
    if(entity->getVelocity() != glm::vec3(0.0,0.0,0.0))
    {
        const glm::vec3& velocity = entity->getVelocity();
        
        localInertia.setX(velocity.x);
        localInertia.setY(velocity.y);
        localInertia.setZ(velocity.z);
        body->setLinearVelocity(localInertia);
    }
    
    mActiveCollisionObjects[physicsEntity] = body;
    mActiveCollisionObjectsReverseMap[body] = physicsEntity;
    
    m_dynamicsWorld->addRigidBody(body);
}

//leak
void PhysicsManager::addCompundShape(btCompoundShape *compoundShape, const PhysicsEntity* physicsEntity, Entity* entity, btTransform& centerMassTransform)
{
	btCollisionShape* colShape = m_factory.createShape(physicsEntity, entity);

	glm::mat4 childGlmTransoform(1.0);

	btTransform transofrm;
	transofrm.setIdentity();

	physicsEntity->getTransformationNoScale(childGlmTransoform);
	convertGLMtoBTTransform(childGlmTransoform, transofrm);

	compoundShape->addChildShape(centerMassTransform * transofrm, colShape);
}

void PhysicsManager::_addSubPhysicsEntity(Entity* entity, btCompoundShape *compoundShape, btTransform& centerMassTransform, int& entityCount, btScalar& mass, bool isDynamic)
{
    PhysicsEntity* physicsEntity = static_cast<PhysicsEntity*>(entity->getPhysicsObject());
    
    if(physicsEntity == NULL)
    {
        return;
    }
    
    if(isDynamic)
    {
        mass += physicsEntity->GetMass();
    }

	if (physicsEntity->hasRenderable())
	{
		addCompundShape(compoundShape, physicsEntity, entity, centerMassTransform);
	}
    
    Entity::EntityList::iterator entityIt = physicsEntity->mChildren.begin();
    for(;entityIt != physicsEntity->mChildren.end(); ++entityIt)
    {
		addCompundShape(compoundShape, static_cast<PhysicsEntity*>(*entityIt), entity, centerMassTransform);
    }
    
    entityIt = entity->getChildren().begin();
    for(;entityIt != entity->getChildren().end(); ++entityIt)
    {
        glm::mat4 localTransform(1.0);
        
        //KKonecko: this needs to be cleaned up a bit. 
        localTransform = (*entityIt)->getTransformation();
        btTransform btTransofrm;
        convertGLMtoBTTransform(localTransform, btTransofrm);
        btTransofrm = centerMassTransform * btTransofrm;
        
        _addSubPhysicsEntity((*entityIt), compoundShape, btTransofrm, entityCount, mass, isDynamic);
    }
}

void PhysicsManager::_removePhysicsEntity(Entity* entity)
{
}

void PhysicsManager::addPendingEntities()
{
    EntityPtrList::iterator it = mAddQueue.begin();
    for(; it != mAddQueue.end(); ++it)
    {
        Entity* entity = (*it);
        _addPhysicsEntity(entity);
    }
    
    mAddQueue.clear();
}

void PhysicsManager::removePendingEntities()
{
    EntityPtrList::iterator it = mRemoveQueue.begin();
    for(; it != mRemoveQueue.end(); ++it)
    {
        Entity* entity = (*it);
        
        PhysicsEntityToVec3::iterator centerMassIt = m_entityToCenterOfMassMap.find(entity->getPhysicsObject());
        if(centerMassIt != m_entityToCenterOfMassMap.end())
        {
            m_entityToCenterOfMassMap.erase(centerMassIt);
        }
        
        Entity::EntityList::iterator entityIt = entity->getChildren().begin();
        for(;entityIt != entity->getChildren().end(); ++entityIt)
        {
            _removePhysicsEntity(*entityIt);
        }
    }

    mRemoveQueue.clear();
}

void PhysicsManager::removePhysicsEntity(PhysicsEntity* physicsEntity, Entity* entity) {	
	PhysicsEntity* rootPhysicsEntity = static_cast<PhysicsEntity*>(entity->getRoot()->getPhysicsObject());
	PhysicsEntityTobtCollisionObjectMap::iterator it = mActiveCollisionObjects.find(rootPhysicsEntity);

	if (it != mActiveCollisionObjects.end()) {
		btRigidBodyToPhysicsEntityMap::iterator revIt = mActiveCollisionObjectsReverseMap.find((*it).second);
		if (revIt != mActiveCollisionObjectsReverseMap.end()) {
			btCollisionObject* collisionObject = it->second;
			btCompoundShape* compoundShape = static_cast<btCompoundShape*>(collisionObject->getCollisionShape());

			int i = compoundShape->getNumChildShapes();
			while (i--) {
				btCollisionShape* child = compoundShape->getChildShape(i);
				if (child->getUserPointer() == entity) {
					compoundShape->removeChildShape(child);
					m_factory.removeCollisionShape(child);
				}
			}
		}
	}
}

void PhysicsManager::removeRootPhysicsEntity(PhysicsEntity* physicsEntity) {
	PhysicsEntityTobtCollisionObjectMap::iterator it = mActiveCollisionObjects.find(physicsEntity);

	if (it != mActiveCollisionObjects.end())
	{
		btRigidBodyToPhysicsEntityMap::iterator revIt = mActiveCollisionObjectsReverseMap.find((*it).second);
		if (revIt != mActiveCollisionObjectsReverseMap.end())
		{
			mActiveCollisionObjectsReverseMap.erase(revIt);
		}

		mRemoveList.push_back((*it).second);
		mActiveCollisionObjects.erase(it);
	}
}

void PhysicsManager::removePhysicsEntityCharacterController(PhysicsEntity* physicsEntity) {
	// LEADS TO A CRASH
	
	//m_dynamicsWorld->removeAction(mCharacter);
	//delete(mCharacter);
	//m_dynamicsWorld->removeCollisionObject(mGhostObject);
	//delete(mGhostObject);
}

void PhysicsManager::onDeltaTime(float timeStep)
{
    //We are in the process of shutting down don't do anything
    if( mInitialized != true ){
        return;
    }
 
	waitUntilPhysicsIsComplete();

    //Pass the delta time to the physics thread.
    mPhysicsThreadData.mDeltaTime = timeStep;
    
    btRigidBodyList::iterator bodyIt = mRemoveList.begin();
    for(; bodyIt != mRemoveList.end(); ++bodyIt) {
        m_dynamicsWorld->removeRigidBody((*bodyIt));
		btCollisionShape* collisionShape = (*bodyIt)->getCollisionShape();
		m_factory.removeRigidBody(*bodyIt);
		m_factory.removeCollisionShape(collisionShape);
    }
    
    mRemoveList.clear();
 
    removePendingEntities();
	addPendingEntities();
    
 	///step the simulation
	if (m_dynamicsWorld)
	{

		PhysicsEntityTobtCollisionObjectMap::iterator it = mActiveCollisionObjects.begin();
		for (; it != mActiveCollisionObjects.end(); ++it)
		{
			btCollisionObject* object = (*it).second;
			PhysicsEntity* physicsEntity = static_cast<PhysicsEntity*>((*it).first);

			if (object->isActive())
			{

				btVector3 centerMassVector;
				const glm::vec3 centerVec = m_entityToCenterOfMassMap[physicsEntity];
				centerMassVector.setX(centerVec.x);
				centerMassVector.setY(centerVec.y);
				centerMassVector.setZ(centerVec.z);

				btMatrix3x3 identity;
				identity.setIdentity();

				btTransform centerMassTransform(identity, centerMassVector);

				const btTransform& transform = object->getWorldTransform();
				glm::mat4 glmTransofrm(1.0);
				convertBTTransformToGLM(transform * centerMassTransform, glmTransofrm);

				physicsEntity->applyPhysicsTransformation(glmTransofrm, timeStep);
			}

			glm::vec3 force;
			glm::vec3 collisionPoint;
			physicsEntity->GetForce(force, collisionPoint);
			btRigidBody* body = static_cast<btRigidBody*>(object);
			if (force.x > 0.5 || force.y > 0.5 || force.z > 0.5)
			{
				body->activate();
			}

			body->applyImpulse(btVector3(force.x, force.y, force.z), btVector3(collisionPoint.x, collisionPoint.y, collisionPoint.z));
		}

		//optional but useful: debug drawing
		if (Settings::getToggleSetting(Settings::SETTING_SHOW_PHYSICS_DEBUG_INFO))
		{
			m_dynamicsWorld->debugDrawWorld();
		}

		btVector3 aabbMin(0.1, 0.1, 0.1);
		btVector3 aabbMax(.2, .2, .2);

		MyOverlapCallback aabbOverlap(aabbMin, aabbMax);
		m_dynamicsWorld->getBroadphase()->aabbTest(aabbMin, aabbMax, aabbOverlap);

		gCollisionContactDelegate.signalCollisionParties();

		for (auto it = mCharacterControllers.begin(); it != mCharacterControllers.end(); ++it) {
			onDeltaTimeCharacterController(it->second, timeStep);
		}
	}

	gCollisionContactDelegate.purge();

	m_physicsJobFuture = std::async(std::launch::async, doPhysicsThread);
}

void PhysicsManager::onDeltaTimeCharacterController(CharacterControlerPhysicsData& characterControllerData, float timeStep) {
	//Need center of mass calculation here
	const btTransform& transform = characterControllerData.mGhostObject->getWorldTransform();
	glm::mat4 glmTransofrm(1.0);
	convertBTTransformToGLM(transform, glmTransofrm);
	characterControllerData.mCharacterEntity->getPhysicsObject()->applyPhysicsTransformation(glmTransofrm, timeStep);
	characterControllerData.mCharacterEntity->getPhysicsObject()->GetDesiredMovement(characterControllerData.mCharacterMovementInput);
}

PhysicsEntity* PhysicsManager::getActivePhysicsEntityByCollidable(const btCollisionObject* collisionObject) {
	btRigidBodyToPhysicsEntityMap::iterator bIT = mActiveCollisionObjectsReverseMap.find(const_cast<btCollisionObject*>(collisionObject));

	if (bIT != mActiveCollisionObjectsReverseMap.end())
	{
		return (*bIT).second;
	}
	
	for (auto it = mCharacterControllers.begin(); it != mCharacterControllers.end(); ++it) {
		CharacterControlerPhysicsData& data = it->second;

		if (collisionObject == data.mGhostObject) {
			return data.mCharacterEntity->getPhysicsObject();
		}
	}

	return NULL;
}
