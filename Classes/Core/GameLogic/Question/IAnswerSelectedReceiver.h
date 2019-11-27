#pragma once

#include <Core/GameLogic/Question/Answer.h>

class IAnswerSelectedReceiver {
public:
	virtual void OnAnswerSelected(const Answer& answer) = 0;
};