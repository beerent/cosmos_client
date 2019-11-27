#pragma once
#include "Core/GameLogic/AI/AIBehavior/IAIBehaviorListener.h"
#include <list>

class IAIBehavior {
public:
	virtual void StartBehavior() = 0;
	virtual void StopBehavior() = 0;
	virtual void OnTick() = 0;
};

using IAIBehaviorList = std::list<IAIBehavior*>;
