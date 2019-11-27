#include <Core/Entity/Entities/CharacterEntity.h>
#include <Core/Events/GameEvents.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

CONST_STRING_DEF(CharacterEntity, CLASS_ID_CHARACTER_ENTITY)


CharacterEntity::CharacterEntity() : 
Entity(CLASS_ID_CHARACTER_ENTITY)
{
	
	registerForEventType(GameEvents::MOVE_FORWARD_START, this);
	registerForEventType(GameEvents::MOVE_FORWARD_STOP, this);
	registerForEventType(GameEvents::MOVE_BACKWARDS_START, this);
	registerForEventType(GameEvents::MOVE_BACKWARDS_STOP, this);
	registerForEventType(GameEvents::TURN_LEFT_START, this);
	registerForEventType(GameEvents::TURN_LEFT_STOP, this);
	registerForEventType(GameEvents::TURN_RIGHT_START, this);
	registerForEventType(GameEvents::TURN_RIGHT_STOP, this);
	registerForEventType(GameEvents::REQUEST_CHARACTER_INPUT, this);

	IEngine::getEngine()->registerForDeltaTimeEvents(this);
}

CharacterEntity::~CharacterEntity()
{
	IEngine::getEngine()->unregisterForDeltaTimeEvents(this);
}

void CharacterEntity::onRootChanged(Entity* previousParent, Entity* newParent)
{
	if (previousParent != NULL)
	{
	}

	if (newParent != NULL)
	{
	}
}


void CharacterEntity::OnHandleEvent(const Event& event)
{
	switch (event.GetEventType()) 
	{
	case GameEvents::MOVE_FORWARD_START:
		m_desiredMovement.m_moveForward = 1.0;
		break;
	case GameEvents::MOVE_FORWARD_STOP:
		m_desiredMovement.m_moveForward = 0.0;
		break;
	case GameEvents::MOVE_BACKWARDS_START:
		m_desiredMovement.m_moveForward = -1.0;
		break;
	case GameEvents::MOVE_BACKWARDS_STOP:
		m_desiredMovement.m_moveForward = 0.0;
		break;
	case GameEvents::TURN_LEFT_START:
		m_desiredMovement.m_turnRight = -1.0;
		break;
	case GameEvents::TURN_LEFT_STOP:
		m_desiredMovement.m_turnRight = 0.0;
		break;
	case GameEvents::TURN_RIGHT_START:
		m_desiredMovement.m_turnRight = 1.0;
		break;
	case GameEvents::TURN_RIGHT_STOP:
		m_desiredMovement.m_turnRight = 0.0;
		break;
	case GameEvents::GAME_DELTA_TIME_EVENT:
		onDeltaTime();
		break;
	case GameEvents::REQUEST_CHARACTER_INPUT:
		static_cast<const GameEvents::RequestCharacterInputEvent*>(&event)->SetCharacterInput(m_desiredMovement);
		break;
	}
}

void CharacterEntity::updateMovementNoPhysics() {
	glm::vec3 pos = getPosition();
	glm::quat rotation = getRotation();

	//std::cout << "Current camera postion x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;

	glm::mat4 rotationMatrix = glm::toMat4(rotation);
	glm::vec4 rightVect = rotationMatrix[0];
	glm::vec4 upVect = rotationMatrix[1];
	glm::vec4 fwdVect = rotationMatrix[2];

	glm::vec4 combinedDelta(0.0f, 0.0f, 0.0f, 0.0f);

	combinedDelta = 0.1f * fwdVect * m_desiredMovement.m_moveForward;

	glm::quat newRotation = glm::angleAxis<float>(-10.0f * m_desiredMovement.m_turnRight, upVect.x, upVect.y, upVect.z);
	rotation = newRotation * rotation;

	pos.x += combinedDelta.x;
	pos.y += combinedDelta.y;
	pos.z += combinedDelta.z;

	//std::cout << "New camera postion x " << pos.x << " y " << pos.y << " z " << pos.z << std::endl;

	SetPosition(pos);
	SetRotation(glm::normalize(rotation));
}

void CharacterEntity::updateMovementWithPhysics() {
	
}

void CharacterEntity::onDeltaTime(){
	if (PHYSICS_ENTITY == mPhysicsType) {
		updateMovementWithPhysics();
	}
	else {
		updateMovementNoPhysics();
	}
}