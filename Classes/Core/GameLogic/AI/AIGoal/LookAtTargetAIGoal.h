#pragma once
#include "core/GameLogic/AI/AIGoal/AIGoal.h"
#include "core/Properties/PropertyCollection.h"
#include "core/StringManager/StringManager.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/Math/OjerMath.h"

class LookAtTargetAIGoal : public AIGoal {
public:
	CONST_STRING_DEC(TARGET_PROPERTY_NAME)
	CONST_STRING_DEC(OWNER_PROPERTY_NAME)

	LookAtTargetAIGoal(const NPCPropertiesProvider& npcProperties, const ILevelView& levelView, const PropertyCollection& properties) :
		 AIGoal(npcProperties, levelView) {
		properties.getValueForProperty(TARGET_PROPERTY_NAME, mTargetPropertyName);
		properties.getValueForProperty(OWNER_PROPERTY_NAME, mOwnerPropertyName);
	}

	const Entity* GetOwnerEntity() {
		STRING_ID ownerName = GetNPCProperty(mOwnerPropertyName);
		return getLevelEntityByName(ownerName);
	}

	const Entity* GetTargetEntity() {
		STRING_ID targetName = GetNPCProperty(mTargetPropertyName);
		return getLevelEntityByName(targetName);
	}

	bool Achievalble() {		
		const Entity *owner = GetOwnerEntity();
		if (!owner)
		{
			return false;
		}

		const Entity *target = GetTargetEntity();
		if (!target)
		{
			return false;
		}

		glm::vec3 ownerPosition = owner->getPosition();
		glm::quat ownerDirection = owner->getRotation();
		glm::vec3 fwrd(0.0, 0.0, 1.0);
		fwrd = ownerDirection * fwrd;
		glm::vec3 targetPosition = target->getPosition();
		fwrd += ownerPosition;

		vec2 ownerPosition2d(ownerPosition.x, ownerPosition.z);
		vec2 targetPosition2d(targetPosition.x, targetPosition.z);
		vec2 ownerDirection2d(fwrd.x, fwrd.z);
		float angle = Math::LookAt2d(ownerPosition2d, ownerDirection2d, targetPosition2d);

		if (angle < 0.1) {
			return false;
		}

		return true;
	}

	bool Completed() {
		/* Do magic math stuff */
		return !Achievalble();
	}

private:
	STRING_ID mTargetPropertyName;
	STRING_ID mOwnerPropertyName;
};

