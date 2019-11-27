#pragma once
#include "core/GameLogic/AI/AIBehavior/AIBehavior.h"
#include "core/GameLogic/EntityControler/CharacterControler.h"

class LookAtTargetAIBehavior : public AIBehavior {
public:
	CONST_STRING_DEC(TARGET_PROPERTY_NAME)
    CONST_STRING_DEC(OWNER_PROPERTY_NAME)

	LookAtTargetAIBehavior(const NPCPropertiesProvider& npcProperties, ILevelView& levelView, const PropertyCollection& properties) : AIBehavior(npcProperties, levelView) {
		properties.getValueForProperty(TARGET_PROPERTY_NAME, mTargetPropertyName);
		properties.getValueForProperty(OWNER_PROPERTY_NAME, mOwnerPropertyName);
	}

	Entity* GetOwnerEntity() {
		STRING_ID ownerName = GetNPCProperty(mOwnerPropertyName);
		return getLevelEntityByName(ownerName);
	}

	const Entity* GetTargetEntity() {
		STRING_ID targetName = GetNPCProperty(mTargetPropertyName);
		return getLevelEntityByName(targetName);
	}

	void OnTick() {
		Entity *owner = GetOwnerEntity();
		if (!owner)
		{
			return;
		}

		const Entity *target = GetTargetEntity();
		if (!target)
		{
			return;
		}
		glm::vec3 ownerPosition = owner->getPosition();
		glm::vec3 targetPosition = target->getPosition();

		const glm::quat& rotation = owner->getRotation();
		glm::vec3 unitVector(0.0, 0.0, -1.0);
		glm::vec3 lookAtVector = glm::normalize(rotation * unitVector);
		glm::vec3 desiredLookAtVec = targetPosition - ownerPosition;
		desiredLookAtVec.y = 0;
		desiredLookAtVec = glm::normalize(desiredLookAtVec);


		vec2 ownerPosition2d(ownerPosition.x, ownerPosition.z);
		vec2 targetPosition2d(targetPosition.x, targetPosition.z);
		vec2 lookAtPosition2d(lookAtVector.x + ownerPosition.x, lookAtVector.z + ownerPosition.z);

		InputManager::InputEventData data;

		bool turnLeft = shouldTurnLeft(ownerPosition2d, targetPosition2d, lookAtPosition2d);

		if (turnLeft) {
			data.setCharValue('A');
		}
		else {
			data.setCharValue('D');
		}

		CharacterControler* controler = static_cast<CharacterControler*>(owner->getControler());

		controler->onInputEvent(InputManager::ON_KEY_DOWN, data);
	}

	void StartBehavior() {};
	void StopBehavior() {
		Entity *owner = GetOwnerEntity();
		if (!owner) {
			return;
		}

		CharacterControler* controler = static_cast<CharacterControler*>(owner->getControler());
		InputManager::InputEventData data;
		data.setCharValue('A');
		controler->onInputEvent(InputManager::ON_KEY_UP, data);
	};

private:
	STRING_ID mTargetPropertyName;
	STRING_ID mOwnerPropertyName;

	bool shouldTurnLeft(const vec2& ownerPosition, const vec2& targetPosition, const vec2& lookAtPosition);
};