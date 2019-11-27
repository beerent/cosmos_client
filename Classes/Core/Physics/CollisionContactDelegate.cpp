#include "Core/Physics/CollisionContactDelegate.h"
#include "Core/Entity/Entities/PhysicsEntity.h"
#include "btBulletDynamicsCommon.h"

void CollisionContactDelegate::insertCollisionContact(const btCollisionObject* colObj0, int partId0, int index0,
	const btCollisionObject* colObj1, int partId1, int index1) {
	mCollisionContacts.push_back(CollisionContactPair(colObj0, partId0, index0, colObj1, partId1, index1));
}

void CollisionContactDelegate::signalCollisionParties() {
	std::list<CollisionContactPair>::iterator cbiIter = mCollisionContacts.begin();
	while (cbiIter != mCollisionContacts.end()) {
		signalCollisionParty(cbiIter->contacts[0], cbiIter->contacts[1]);
		signalCollisionParty(cbiIter->contacts[1], cbiIter->contacts[0]);
		cbiIter++;
	}
}

void CollisionContactDelegate::purge() {
	mCollisionContacts.clear();
}

void CollisionContactDelegate::setActiveCollisionObjectReverseMap(IActivePhysicsEntityProvider& activePhysicsEntityProvider) {
	mActiveCollisionObjectsReverseMap = &activePhysicsEntityProvider;
}

void CollisionContactDelegate::signalCollisionParty(const CollisionContact& contactTarget, const CollisionContact& contactSource) {
	const btCollisionObject* collisionObjectTarget = contactTarget.colObj;
	const btCollisionObject* collisionObjectSource = contactSource.colObj;

	int childIndex = contactTarget.index;

	PhysicsEntity* physicsEnityTarget = mActiveCollisionObjectsReverseMap->getActivePhysicsEntityByCollidable(collisionObjectTarget);
	PhysicsEntity* physicsEnitySource = mActiveCollisionObjectsReverseMap->getActivePhysicsEntityByCollidable(collisionObjectSource);

	if (physicsEnityTarget && physicsEnitySource) {
		const btCompoundShape* compound = static_cast<const btCompoundShape*>(collisionObjectTarget->getCollisionShape());
		if (compound != NULL && childIndex < compound->getNumChildShapes() &&
			compound->getShapeType() == COMPOUND_SHAPE_PROXYTYPE) {
			const btCollisionShape* childShape = compound->getChildShape(childIndex);
			if (childShape != NULL && childShape->getUserPointer()) {
				if (physicsEnityTarget) {
					physicsEnityTarget->OnImpact(childShape->getUserPointer(), physicsEnitySource);
				}
			}
		}
	}
}

CollisionContactDelegate::CollisionContactPair::CollisionContactPair(const btCollisionObject* colObj0, int partId0, int index0,
	const btCollisionObject* colObj1, int partId1, int index1) {
	contacts[0].colObj = colObj0;
	contacts[0].partId = partId0;
	contacts[0].index = index0;
	contacts[1].colObj = colObj1;
	contacts[1].partId = partId1;
	contacts[1].index = index1;
}
