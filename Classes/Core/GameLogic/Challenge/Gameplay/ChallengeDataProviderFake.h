#pragma once

#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProvider.h>

class ChallengeDataProviderFake : public IChallengeDataProvider {
public:
	virtual void RequestChallengeId();
	virtual void RequestChallengeQuestions(int challengeId);

	virtual void SubmitChallengeAnswer(int challengeId, int answerId);

private:

	virtual void ChallengeIdReceived(int challengeId);
    virtual void ChallengeTimerReceived(int timerSeconds);
	virtual void ChallengeQuestionsReceived(const std::queue<Question>& questions);
};
