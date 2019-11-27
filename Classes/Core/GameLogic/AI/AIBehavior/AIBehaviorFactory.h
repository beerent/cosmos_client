#pragma once
#include "IAIBehavior.h"
#include <assert.h>
#include "Core/GameLogic/AI/NPCProperties.h"
#include "Core/LevelFactory/ILevelView.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/GameLogic/AI/AIBehavior/AIBehavior.h"
#include "Core/GameLogic/AI/NPCProperties.h"

class AIBehaviorFactory {
public:
	CONST_STRING_DEC(TYPE_LOOK_AT_TARGET_AI_BEHAVIOR)
	CONST_STRING_DEC(SCRIPTED_AI_BEHAVIOR)

	AIBehaviorFactory(PropertyCollectionRepository& archetypeRepository);
	IAIBehavior* Create(STRING_ID name, ILevelView& levelView, const NPCPropertiesProvider& npcProperties);

private:
	IAIBehavior* Create(const PropertyCollection& pArchetype, ILevelView& levelView, const NPCPropertiesProvider& npcProperties);
	PropertyCollectionRepository& mArchetypeRepository;
};