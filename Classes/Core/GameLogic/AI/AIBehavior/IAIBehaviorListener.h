#pragma once

class IAIBehaviorListener {
public:
	virtual void OnBehaviorCompleted() = 0;
	virtual void OnBehaviorInterupted() = 0;
	virtual void OnBehaviorBehaviorCanceled() = 0;
};