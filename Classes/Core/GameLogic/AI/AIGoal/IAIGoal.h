#pragma once
#include <list>

class IAIGoalListener {
public:
	virtual void OnGoalCompleted() = 0;
};

class IAIGoal {
public:
	virtual void RegisterListener(IAIGoalListener& goalListener) = 0;
	virtual void UnRegisterListener(IAIGoalListener& goalListener) = 0;
	virtual bool Achievalble() = 0;
	virtual bool Completed() = 0;
};

using IAIGoalList = std::list<IAIGoal*>;
