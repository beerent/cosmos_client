#include "Core/GameLogic/AI/NPCAI/NCPAIFactory.h"
#include "Core/GameLogic/EntityControler/AICharacterControler.h"
#include <algorithm>
#include "Core/GameLogic/AI/AIGoal/IAIGoal.h"
CONST_STRING_DEF(NPCAIFactory, TYPE_NPCAI)
CONST_STRING_DEF(NPCAIFactory, BEHAVIORS)
CONST_STRING_DEF(NPCAIFactory, GOALS)

NPCAIFactory::NPCAIFactory(PropertyCollectionRepository& archetypeRepository) :
	mArchetypeRepository(archetypeRepository), 
	m_gaolFactory(archetypeRepository), 
	m_behaviorFactory(archetypeRepository) {
}

NPCAI* NPCAIFactory::create(const STRING_ID& name, AICharacterControler& aiController, ILevelView& levelView) {
	return create(*mArchetypeRepository.getPropertyCollection(name), aiController, levelView);
}

NPCAI* NPCAIFactory::create(const PropertyCollection& pArchetype, AICharacterControler& aiController, ILevelView& levelView) {
	PropertyCollection::Property const* property = NULL;
	NPCAI* npcAI = nullptr;
	if (pArchetype.findPropertyByName(PropertyCollection::TYPE, &property)) {
		StringManager::StringID typeName = property->m_data.StringIDValue;
		if (typeName == TYPE_NPCAI) {
			npcAI = new NPCAI(aiController, pArchetype);
		}
	}

	if (pArchetype.findPropertyByName(GOALS, &property)) {
		npcAI->m_goals.push_back(m_gaolFactory.Create(property->m_data.StringIDValue, levelView, *npcAI));
	}

	if (pArchetype.findPropertyByName(BEHAVIORS, &property)) {
		npcAI->m_behaviors.push_back(m_behaviorFactory.Create(property->m_data.StringIDValue, levelView, *npcAI));
	}

	return npcAI;
}
