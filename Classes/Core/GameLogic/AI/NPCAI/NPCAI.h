#pragma  once
#include "Core/StringManager/StringManager.h"
#include "Core/Properties/PropertyCollection.h"
#include "Core/GameLogic/EntityControler/AICharacterControler.h"
#include "Core/GameLogic/AI/AIGoal/IAIGoal.h"
#include "Core/GameLogic/AI/AIBehavior/IAIBehavior.h"
#include "Core/GameLogic/AI/NPCProperties.h"
#include "Core/Events/EventDispatcher.h"
#include "Core/Events/GameEvents.h"

class NPCAI : public NPCPropertiesProvider, public IEventHandler {
public:
	friend class NPCAIFactory;

	NPCAI(AICharacterControler& aiController, 
		  const PropertyCollection& propertyCollection);

	virtual ~NPCAI();
	
	const NPCProperties& GetNPCProperties() const {
		return m_NPCproperties;
	}

	void setProperty(STRING_ID propertyName, STRING_ID propertyValue) {
		m_NPCproperties.addProperty(propertyName, propertyValue);
	}

	void OnHandleEvent(const Event& event) {
		if (event.GetEventType() == GameEvents::GAME_DELTA_TIME_EVENT) {
			OnTick();
		}
	}

private:
	IAIGoalList m_goals;
	IAIBehaviorList m_behaviors;
	AICharacterControler& mAIController;
	NPCProperties m_NPCproperties;
	IAIGoal* m_activeGoal;
	IAIBehavior* m_activeBehavior;

	void OnTick() {
		SelectGoal();
		SelectBehavior();
		PerformBehavior();
	}

	void SelectGoal() {
		if (m_activeGoal == nullptr && m_goals.size() > 0) {
			m_activeGoal = m_goals.front();
		}
	}

	void SelectBehavior() {
		if (m_activeGoal != nullptr) {
			if (m_activeGoal->Achievalble()){
				if (m_activeBehavior == nullptr && m_behaviors.size() > 0) {
					m_activeBehavior = m_behaviors.front();
					m_activeBehavior->StartBehavior();
				}
			}
			else if (m_activeGoal->Completed() && m_activeBehavior != nullptr) {
				m_activeBehavior->StopBehavior();
				m_activeBehavior = nullptr;
			}
		}		
	}

	void PerformBehavior() {
		if (m_activeBehavior != nullptr) {
			m_activeBehavior->OnTick();
		}
	}
};
