#ifndef __CharacterEntity__
#define __CharacterEntity__
#include <Core/StringManager/StringManager.h>
#include <Core/Entity/Entities/Entity.h>
#include <Core/Input/InputDataTypes.h>

class CharacterEntity : public Entity
{
public:
	CONST_STRING_DEC(CLASS_ID_CHARACTER_ENTITY)
	CharacterEntity();
	~CharacterEntity();
	void OnHandleEvent(const Event& event);

protected:
	void onRootChanged(Entity* previousParent, Entity* newParent);

private:
	CharacterMovementInput m_desiredMovement;

	void onDeltaTime();

	void updateMovementNoPhysics();
	void updateMovementWithPhysics();
};


#endif