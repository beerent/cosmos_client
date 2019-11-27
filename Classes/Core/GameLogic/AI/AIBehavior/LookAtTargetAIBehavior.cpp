#include "core/GameLogic/AI/AIBehavior/LookAtTargetAIBehavior.h"

CONST_STRING_DEF(LookAtTargetAIBehavior, TARGET_PROPERTY_NAME)
CONST_STRING_DEF(LookAtTargetAIBehavior, OWNER_PROPERTY_NAME)

bool LookAtTargetAIBehavior::shouldTurnLeft(const vec2& ownerPosition, const vec2& targetPosition, const vec2& lookAtPosition) {
	double pos = (lookAtPosition.x - ownerPosition.x) * (targetPosition.y - ownerPosition.y) - (lookAtPosition.y - ownerPosition.y) * (targetPosition.x - ownerPosition.x);
	return pos >= 0;
}
