#pragma once
#include "IAIGoal.h"
#include <assert.h>
#include "Core/GameLogic/AI/NPCProperties.h"
#include "Core/LevelFactory/ILevelView.h"

class AIGoal : public IAIGoal {
public:
	AIGoal(const NPCPropertiesProvider& npcProperties, const ILevelView& levelView) :
		mListener(nullptr), mNPCProperties(npcProperties), mLevelView(levelView) {
	}

	virtual void RegisterListener(IAIGoalListener& goalListener) {
		assert(mListener == nullptr && "Listener already set");
		mListener = &goalListener;
	}

	virtual void UnRegisterListener(IAIGoalListener& goalListener) {
		assert(mListener == &goalListener && "Was not registered!");
		mListener = nullptr;
	}

protected:
	STRING_ID GetNPCProperty(STRING_ID id) const {
		return mNPCProperties.GetNPCProperties().getProperty(id);
	}

	const Entity* getLevelEntityByName(STRING_ID name) {
		return mLevelView.getEntityByName(name, false);
	}

private:
	IAIGoalListener* mListener;
	const NPCPropertiesProvider& mNPCProperties;
	const ILevelView& mLevelView;
};
