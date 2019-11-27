#include "Core/GameLogic/AI/AIGoal/AIGoalFactory.h"
#include "Core/GameLogic/AI/AIGoal/LookAtTargetAIGoal.h"

CONST_STRING_DEF(AIGoalFactory, TYPE_LOOK_AT_TARGET_AI_GOAL)

AIGoalFactory::AIGoalFactory(PropertyCollectionRepository& archetypeRepository) : mArchetypeRepository(archetypeRepository){

}

AIGoal* AIGoalFactory::Create(STRING_ID name, ILevelView& levelView, const NPCPropertiesProvider& npcProperties) {
	return Create(*mArchetypeRepository.getPropertyCollection(name), levelView, npcProperties);
}

AIGoal* AIGoalFactory::Create(const PropertyCollection& pArchetype, ILevelView& levelView, const NPCPropertiesProvider& npcProperties) {
	PropertyCollection::Property const* property = NULL;
    AIGoal* goal = NULL;

	if (pArchetype.findPropertyByName(PropertyCollection::TYPE, &property)) {
		StringManager::StringID typeName = property->m_data.StringIDValue;
		if (typeName == TYPE_LOOK_AT_TARGET_AI_GOAL) {
			goal = new LookAtTargetAIGoal(npcProperties, levelView, pArchetype);
		}
	}
    
    return goal;
}
