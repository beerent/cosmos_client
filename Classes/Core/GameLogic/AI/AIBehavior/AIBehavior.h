#pragma once 
#include "Core/GameLogic/AI/AIBehavior/IAIBehavior.h"
#include "Core/GameLogic/AI/NPCProperties.h"
#include "Core/LevelFactory/ILevelView.h"
#include <assert.h>

class AIBehavior : public IAIBehavior {
public:
	AIBehavior(const NPCPropertiesProvider& npcProperties, ILevelView& levelView) : 
		mNPCProperties(npcProperties), mLevelView(levelView) {
	}

	virtual void RegisterListener(IAIBehaviorListener& listener) {
		assert(mListener == nullptr && "Listener already set");
		mListener = &listener;
	}

	virtual void UnRegisterListener(IAIBehaviorListener& listener) {
		assert(mListener == &listener && "Was not registered!");
		mListener = nullptr;
	}

protected:
	Entity* getLevelEntityByName(STRING_ID name) {
		return mLevelView.getEntityByName(name, false);
	}

	STRING_ID GetNPCProperty(STRING_ID id) const {
		return mNPCProperties.GetNPCProperties().getProperty(id);
	}

private:
	IAIBehaviorListener* mListener;
	const NPCPropertiesProvider& mNPCProperties;
	ILevelView& mLevelView;
};
