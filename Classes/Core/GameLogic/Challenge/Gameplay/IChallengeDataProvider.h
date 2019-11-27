#pragma once

#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProviderReceiver.h>

class IChallengeDataProvider {
public:
	virtual void RequestChallengeId() = 0;
	virtual void RequestChallengeQuestions(int challengeId) = 0;
	virtual void SubmitChallengeAnswer(int challengeId, int answerId) = 0;

	void RegisterChallengeDataReceiver(IChallengeDataProviderReceiver* challengeDataProviderReceiver) {
		m_challengeDataProviderReceiver = challengeDataProviderReceiver;
	}
	
protected:
	IChallengeDataProviderReceiver* m_challengeDataProviderReceiver;

private:
	virtual void ChallengeIdReceived(int challengeId) = 0;
	virtual void ChallengeQuestionsReceived(const std::queue<Question>& questions) = 0;

};