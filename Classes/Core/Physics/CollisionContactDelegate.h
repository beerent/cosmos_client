#ifndef PhysicsCollisionContactDelegate_h
#define PhysicsCollisionContactDelegate_h

#include "Core/Physics/IPhysicsManager.h"
#include "Core/Physics/PhysicsInterfaces.h"
#include <list>

class CollisionContactDelegate
{
public:
	void insertCollisionContact(const btCollisionObject* colObj0, int partId0, int index0,
		const btCollisionObject* colObj1, int partId1, int index1);
	void signalCollisionParties();
	void purge();
	void setActiveCollisionObjectReverseMap(IActivePhysicsEntityProvider& activePhysicsEntityProvider);

private:
	IActivePhysicsEntityProvider* mActiveCollisionObjectsReverseMap;

	struct CollisionContact {
		const btCollisionObject* colObj;
		int partId;
		int index;
	};

	void signalCollisionParty(const CollisionContact& contactTarget, const CollisionContact& contactSource);

	struct CollisionContactPair {
		CollisionContactPair(const btCollisionObject* colObj0, int partId0, int index0,
				     		 const btCollisionObject* colObj1, int partId1, int index1);
		CollisionContact contacts[2];
	};

	std::list<CollisionContactPair> mCollisionContacts;
};
#endif
