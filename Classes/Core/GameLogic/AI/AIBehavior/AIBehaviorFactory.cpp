#include "Core/GameLogic/AI/AIBehavior/AIBehaviorFactory.h"
#include "Core/GameLogic/AI/AIBehavior/LookAtTargetAIBehavior.h"
#include "Core/GameLogic/AI/AIBehavior/ScriptedAIBehavior.h"

CONST_STRING_DEF(AIBehaviorFactory, TYPE_LOOK_AT_TARGET_AI_BEHAVIOR)
CONST_STRING_DEF(AIBehaviorFactory, SCRIPTED_AI_BEHAVIOR)

AIBehaviorFactory::AIBehaviorFactory(PropertyCollectionRepository& archetypeRepository) : mArchetypeRepository(archetypeRepository) {
}

IAIBehavior* AIBehaviorFactory::Create(STRING_ID name, ILevelView& levelView, const NPCPropertiesProvider& npcProperties) {
	return Create(*mArchetypeRepository.getPropertyCollection(name), levelView, npcProperties);
}

IAIBehavior* AIBehaviorFactory::Create(const PropertyCollection& pArchetype, ILevelView& levelView, const NPCPropertiesProvider& npcProperties) {
	PropertyCollection::Property const* property = NULL;
    IAIBehavior* behavior = NULL;
    
	if (pArchetype.findPropertyByName(PropertyCollection::TYPE, &property)) {
		StringManager::StringID typeName = property->m_data.StringIDValue;
		if (typeName == TYPE_LOOK_AT_TARGET_AI_BEHAVIOR) {
			behavior = new LookAtTargetAIBehavior(npcProperties, levelView, pArchetype);
		} else if (typeName == SCRIPTED_AI_BEHAVIOR) {
			behavior = new ScriptedAIBehavior(npcProperties, levelView, pArchetype);
		}
	}
    
    return behavior;
}
