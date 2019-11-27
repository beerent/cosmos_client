#include "Core/GameLogic/AI/NPCAI/NPCAI.h"

NPCAI::NPCAI(AICharacterControler& aiController, 
	const PropertyCollection& propertyCollection) :
		mAIController(aiController), m_activeGoal(nullptr),
		m_activeBehavior(nullptr) {

	IEngine::getEngine()->registerForDeltaTimeEvents(this);

}

NPCAI::~NPCAI() {
	IEngine::getEngine()->unregisterForDeltaTimeEvents(this);
}

