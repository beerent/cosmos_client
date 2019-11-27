#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "Interface.hpp"
#include "Core/OpenSource/Vector.hpp"
#include "Core/StringManager/StringManager.h"
#include "Core/Entity/Entities/Entity.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/GameLogic/AI/NPCAI/NPCAI.h"
#include "Core/GameLogic/AI/AIGoal/AIGoalFactory.h"
#include "Core/GameLogic/AI/AIBehavior/AIBehaviorFactory.h"
#include "Core/LevelFactory/Level.h"

class AICharacterControler;
class PropertyCollectionRepository;

class NPCAIFactory {
public:
	CONST_STRING_DEC(TYPE_NPCAI)
	CONST_STRING_DEC(BEHAVIORS)
	CONST_STRING_DEC(GOALS)

	NPCAIFactory(PropertyCollectionRepository& archetypeRepository);
	NPCAI* create(const STRING_ID& name, AICharacterControler& aiController, ILevelView& levelView);

private:
	AIGoalFactory m_gaolFactory;
	AIBehaviorFactory m_behaviorFactory;

	NPCAI* create(const PropertyCollection& pArchetype, AICharacterControler& aiController, ILevelView& levelView);
	PropertyCollectionRepository& mArchetypeRepository;
};
