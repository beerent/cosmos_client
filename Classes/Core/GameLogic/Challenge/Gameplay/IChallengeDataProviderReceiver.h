#pragma once

#include <queue>
#include <Core/GameLogic/Question/Question.h>

class IChallengeDataProviderReceiver {
public:
	virtual void ChallengeIdReceived(int challengeId) = 0;
    virtual void ChallengeTimerReceived(int timerSeconds) = 0;
	virtual void ChallengeQuestionsReceived(std::queue<Question> questions) = 0;

};
