#include "Core/Physics/PhysicsManager.h"
#include "Core/Entity/Entities/PhysicsEntity.h"
#include "BulletCollision/CollisionShapes/btBox2dShape.h"
#include "BulletCollision/CollisionShapes/btTriangleShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"
#include "btBulletDynamicsCommon.h"
#include "Core/Physics/BulletCharacterController.h"
#include "Core/Physics/CollisionContactDelegate.h"
#include "Core/Physics/MathGLMBulletConversionHelpers.h"
#include "Core/Physics/PhysicsObjectFactory.h"

PhysicsObjectFactory::~PhysicsObjectFactory() {
	for (auto it = m_collisionShapes.begin(); it != m_collisionShapes.end(); ++it) {
		delete(*it);
	}

	for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end(); ++it) {
		delete(*it);
	}
}

btStaticPlaneShape* PhysicsObjectFactory::createGroundShape() {
	btStaticPlaneShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 100);
	m_collisionShapes.insert(groundShape);
	return groundShape;
}

btCompoundShape* PhysicsObjectFactory::createCompoundShape() {
	btCompoundShape* compoundShape = new btCompoundShape(true);
	m_collisionShapes.insert(compoundShape);
	return compoundShape;
}

btRigidBody* PhysicsObjectFactory::createRigidBody(const btRigidBody::btRigidBodyConstructionInfo& constructionInfo) {
	btRigidBody* rigidBody = new btRigidBody(constructionInfo);
	m_rigidBodies.insert(rigidBody);
	return rigidBody;
}

void PhysicsObjectFactory::removeRigidBody(btRigidBody* bodyToRemove) {
	auto it = m_rigidBodies.find(bodyToRemove);
	if (it != m_rigidBodies.end()) {
		m_rigidBodies.erase(it);
		delete(bodyToRemove);
	}	
}

void PhysicsObjectFactory::removeCollisionShape(btCollisionShape* shapeToRemove) {
	auto it = m_collisionShapes.find(shapeToRemove);
	if (it != m_collisionShapes.end()) {
		m_collisionShapes.erase(it);
		delete(shapeToRemove);
	}
}

namespace
{
	btCollisionShape* createConvexHullShapeFromPhysicsEntity(const PhysicsEntity* physicsEntity)
	{
		btConvexHullShape* hullShape = new btConvexHullShape();
		const Mesh* mesh = physicsEntity->GetMesh();
		const GLushort* indices = mesh->getIndexes();
		const Mesh::VertexData* vertexData = mesh->getVertexData();

		//There may be a faster way of doing this! OPTIMIZATION OPERTUNITY!
		//Creating these point by point can't be cheap!
		//Can shapes be cloned like prototypes
		for (unsigned int i = 0; i < mesh->getIndexCount(); ++i)
		{
			const GLfloat *vert = vertexData[indices[i]].m_vertice;
			btVector3 v(vert[0], vert[1], vert[2]);
			sprintf(Logger::GetTempCharBuffer(), "Vec i%d x%f y%f z%f ", i, vert[0], vert[1], vert[2]);
			Logger::Log(Logger::MATH_3D, Logger::GetTempCharBuffer());
			hullShape->addPoint(v, false);
		}
		hullShape->recalcLocalAabb();
		return hullShape;
	}
}

btCollisionShape*  PhysicsObjectFactory::createShape(const PhysicsEntity* physicsEntity, Entity* entity) {
	// this can't be a compund shape!
	btCollisionShape* colShape = NULL;

	STRING_ID shapeType = physicsEntity->GetShapeType();

	if (PhysicsEntity::BOX_DEPRICATED == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btBoxShape(btVector3(.15 * scale.x, .15 * scale.y, .15 * scale.z));
	}
	else if (PhysicsEntity::BOX == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
	}
	else if (PhysicsEntity::BOX2D == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btBox2dShape(btVector3(scale.x, scale.y, 0.0));
	}
	else if (PhysicsEntity::TRIANGLE == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		auto xS = scale.x;
		auto yS = scale.y;
		colShape = new btTriangleShape(btVector3(-1.0 * xS, -0.5 * yS, 0.0), btVector3(1.0 * xS, -0.5 * yS, 0.0), btVector3(0.0 * xS, 0.5 * yS, 0.0));
	}
	else if (PhysicsEntity::CYLINDER == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btCylinderShape(btVector3(scale.x, scale.y, scale.z));
	}
	else if (PhysicsEntity::CAPSULE == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btCapsuleShape(scale.x, (2 * scale.y) - (2 * scale.x));
	}
	else if (PhysicsEntity::CONE == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btConeShape(scale.x, scale.y);
	}
	else if (PhysicsEntity::SPHERE == shapeType)
	{
		glm::vec3 scale = physicsEntity->getScaleVec();
		colShape = new btSphereShape(scale.x);
	}
	else if (PhysicsEntity::BHV_TRIANGLE_MESH == shapeType)
	{
		colShape = createBHVTriangleMeshFromPhysicsEntity(physicsEntity);
	}
	else if (PhysicsEntity::CONVEX_HULL == shapeType)
	{
		colShape = createConvexHullShapeFromPhysicsEntity(physicsEntity);
	}
	else if (PhysicsEntity::CONVEX_TRIANGLEMESH_SHAPE == shapeType)
	{
		colShape = createConvexTriangleMeshFromPhysicsEntity(physicsEntity);
	}

	colShape->setUserPointer(static_cast<void*>(entity));
	//btCollisionShape* colShape = new btSphereShape(btScalar(1.));
	m_collisionShapes.insert(colShape);
	/// Create Dynamic Objects
	return colShape;
}

btTriangleIndexVertexArray* PhysicsObjectFactory::getbtTirangleIndexVertexArrayForPhysicsEntity(const PhysicsEntity* physicsEntity) {
	const Mesh* mesh = physicsEntity->GetMesh();

	auto it = mMeshToBtTriangleIndexVertexArrayCache.find(mesh);

	if (it != mMeshToBtTriangleIndexVertexArrayCache.end())
	{
		return it->second;
	}

	const GLushort* indices = mesh->getIndexes();

	int totalTriangles = mesh->getIndexCount() / 3;//2 * (NUM_VERTS_X - 1)*(NUM_VERTS_Y - 1);
	int* gIndices = new int[totalTriangles * 3];

	//this is slow
	for (int i = 0; i < totalTriangles * 3; ++i)
	{
		gIndices[i] = indices[i];
	}

	int indexStride = 3 * sizeof(int);

	int totalVerts = mesh->getVertexCount();

	btVector3* vertices = new btVector3[totalVerts];
	const Mesh::VertexData* vertexData = mesh->getVertexData();

	for (int i = 0; i < totalVerts; ++i)
	{
		const Mesh::VertexData& v = vertexData[i];
		vertices[i].setValue(v.m_vertice[0], v.m_vertice[1], v.m_vertice[2]);
	}

	int vertStride = sizeof(btVector3);

	btTriangleIndexVertexArray* vertexArray = new btTriangleIndexVertexArray(totalTriangles,
		gIndices,
		indexStride,
		totalVerts, (btScalar*)&vertices[0].x(), vertStride);

	mMeshToBtTriangleIndexVertexArrayCache[mesh] = vertexArray;
	return vertexArray;
}

btCollisionShape* PhysicsObjectFactory::createConvexTriangleMeshFromPhysicsEntity(const PhysicsEntity* physicsEntity) {
	return new btConvexTriangleMeshShape(getbtTirangleIndexVertexArrayForPhysicsEntity(physicsEntity), true);
}

btCollisionShape* PhysicsObjectFactory::createBHVTriangleMeshFromPhysicsEntity(const PhysicsEntity* physicsEntity) {
	return new btBvhTriangleMeshShape(getbtTirangleIndexVertexArrayForPhysicsEntity(physicsEntity), true);
}