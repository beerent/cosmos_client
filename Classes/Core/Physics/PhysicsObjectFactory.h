#ifndef PhysicsObjectFactory_h
#define PhysicsObjectFactory_h

#include "Core/Rendering/MeshManager/Mesh.h"
#include "btBulletDynamicsCommon.h"
#include <map>
#include <set>

class PhysicsObjectFactory {
    public:    
		btStaticPlaneShape* createGroundShape();
		btCompoundShape* createCompoundShape();
		btCollisionShape* createShape(const PhysicsEntity* physicsEntity, Entity* entity);
		btRigidBody* createRigidBody(const btRigidBody::btRigidBodyConstructionInfo& constructionInfo);
		void removeRigidBody(btRigidBody* bodyToRemove);
		void removeCollisionShape(btCollisionShape* shapeToRemove);

		~PhysicsObjectFactory();

	private:
		std::set<btCollisionShape*>	m_collisionShapes;
		std::map<const Mesh*, btTriangleIndexVertexArray*> mMeshToBtTriangleIndexVertexArrayCache;
		std::set<btRigidBody*> m_rigidBodies;

		btTriangleIndexVertexArray* getbtTirangleIndexVertexArrayForPhysicsEntity(const PhysicsEntity* physicsEntity);
		btCollisionShape* createConvexTriangleMeshFromPhysicsEntity(const PhysicsEntity* physicsEntity);
		btCollisionShape* createBHVTriangleMeshFromPhysicsEntity(const PhysicsEntity* physicsEntity);
};

#endif
