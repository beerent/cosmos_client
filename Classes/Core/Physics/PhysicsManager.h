//
//  PhysicsManager.h
//  Projectios1
//
//  Created by Kamil Konecko on 2/17/14.
//
//

#ifndef Projectios1_PhysicsManager_h
#define Projectios1_PhysicsManager_h

#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <list>
#include <mutex>
#include <thread>
#include <map>
#include "Core/Physics/IPhysicsManager.h"
#include "Core/Physics/PhysicsDebugDrawer.h"
#include "Core/Input/InputDataTypes.h"
#include "Core/Physics/PhysicsInterfaces.h"
#include "Core/Physics/PhysicsObjectFactory.h"
#include <future>

class PhysicsEntity;
class Entity;
class Mesh;
class btPairCachingGhostObject;
class BulletCharacterController;

class PhysicsManager : public IPhysicsManager, public IActivePhysicsEntityProvider
{
public:
    
    typedef std::list<Entity*> EntityPtrList;
    typedef std::list<PhysicsEntity*> PhysicsEntityPtrList;
    typedef std::map<PhysicsEntity*, btRigidBody*> PhysicsEntityTobtCollisionObjectMap;
    typedef std::map<btCollisionObject*, PhysicsEntity*> btRigidBodyToPhysicsEntityMap;
    typedef std::list<btRigidBody*> btRigidBodyList;
    typedef std::map<PhysicsEntity*, glm::vec3> PhysicsEntityToVec3;
    
    PhysicsManager();
    ~PhysicsManager();
    void initialize(const glm::vec3& gravity, bool hasGround);
    void shutDown();
    void onDeltaTime(float timeStep);

    void addEntity(Entity* entity);
    void removeEntity(Entity* entity);

	void waitUntilPhysicsIsComplete();
    
    PhysicsDebugDrawer* getPhysicsDebugDrawer();     
	virtual PhysicsEntity* getActivePhysicsEntityByCollidable(const btCollisionObject* collisionObject);

private:
    
    enum PhysicsThreadState
    {
        THREAD_BUSY,
        THREAD_WAITING,
        THREAD_SHUTDOWN,
        THREAD_DEAD
    };
    
    bool mInitialized;
    
	PhysicsObjectFactory m_factory;
    PhysicsEntityToVec3 m_entityToCenterOfMassMap;
    btDefaultCollisionConfiguration* m_collisionConfiguration;
	btBroadphaseInterface*	m_broadphase;
	btCollisionDispatcher*	m_dispatcher;
	btConstraintSolver*	m_solver;
 
    btRigidBodyList mFreeList;
    btRigidBodyList mRemoveList;
    
    EntityPtrList mAddQueue;
    EntityPtrList mRemoveQueue;
    
    PhysicsEntityTobtCollisionObjectMap mActiveCollisionObjects;
    btRigidBodyToPhysicsEntityMap mActiveCollisionObjectsReverseMap;
    
    ///this is the most important class
	btDynamicsWorld*		m_dynamicsWorld;
    PhysicsDebugDrawer*     m_debugDrawer;
        
    //We need to better encapsulate this.
    struct PhysicsThreadData
    {
        float mDeltaTime;
        long  mTimeSamples;
        
        PhysicsThreadData(): mDeltaTime(0), mTimeSamples(0) {}
    };
    PhysicsThreadData mPhysicsThreadData;
    
    static PhysicsManager*         mInstance;

	struct CharacterControlerPhysicsData {
		btPairCachingGhostObject* mGhostObject;
		BulletCharacterController* mCharacter;
		CharacterMovementInput mCharacterMovementInput;
		Entity* mCharacterEntity;
	};

	std::map<Entity*, CharacterControlerPhysicsData> mCharacterControllers;
	
	std::future<bool> m_physicsJobFuture;
    
	void removePhysicsEntity(PhysicsEntity* physicsEntity, Entity* entity);
	void removeRootPhysicsEntity(PhysicsEntity* physicsEntity);
	void removePhysicsEntityCharacterController(PhysicsEntity* physicsEntity);

    void addPendingEntities();
    void removePendingEntities();
        
    static bool doPhysicsThread();
	void physicsThreadStep();
	void updateCharacterController(CharacterControlerPhysicsData& characterControllerData);
	void onDeltaTimeCharacterController(CharacterControlerPhysicsData& characterControllerData, float timeStep);
        
	void addCompundShape(btCompoundShape *compoundShape, const PhysicsEntity* physicsEntity, Entity* entity, btTransform& centerMassTransform);
    void _addPhysicsEntity(Entity* entity);
	void addPhysicsCharacterController(Entity *entity);
    void _addSubPhysicsEntity(Entity* entity, btCompoundShape *compoundShape, btTransform& centerMassTransform, int& entityCount, btScalar& mass, bool isDynamic);
    void _removePhysicsEntity(Entity* entity);
};
#endif
