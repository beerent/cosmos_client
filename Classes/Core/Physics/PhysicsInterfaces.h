#ifndef PhysicsInterfaces_h
#define PhysicsInterfaces_h

class btCollisionObject;
class PhysicsEntity;

class IActivePhysicsEntityProvider {
public:
	virtual PhysicsEntity* getActivePhysicsEntityByCollidable(const btCollisionObject* collisionObject) = 0;
};
#endif
