#pragma once
#include "IAIGoal.h"
#include <assert.h>
#include "Core/GameLogic/AI/NPCProperties.h"
#include "Core/LevelFactory/ILevelView.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/GameLogic/AI/AIGoal/AIGoal.h"
#include "Core/GameLogic/AI/NPCProperties.h"

class AIGoalFactory {
public:
	CONST_STRING_DEC(TYPE_LOOK_AT_TARGET_AI_GOAL)		
	AIGoalFactory(PropertyCollectionRepository& archetypeRepository);
	AIGoal* Create(STRING_ID name, ILevelView& levelView, const NPCPropertiesProvider& npcProperties);

private:	
	AIGoal* Create(const PropertyCollection& pArchetype, ILevelView& levelView, const NPCPropertiesProvider& npcProperties);
	PropertyCollectionRepository& mArchetypeRepository;
};
