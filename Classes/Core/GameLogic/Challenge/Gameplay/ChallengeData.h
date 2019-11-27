#pragma once

#include <queue>

#include <Core/GameLogic/Question/IQuestionsReadyReceiver.h>
#include <Core/GameLogic/Question/Question.h>
#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProvider.h>
#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProviderReceiver.h>

class ChallengeData : public IChallengeDataProviderReceiver {
public:
	ChallengeData();
	void StartNewGame();
	void RegisterQuestionsReadyReceiver(IQuestionsReadyReceiver* questionReadyReceiver);

	virtual void ChallengeIdReceived(int gameId);
	virtual void ChallengeQuestionsReceived(std::queue<Question> questions);

	bool ChallengeQuestionAvailable() const;
	void ChallengeQuestionsRequested();
	Question GetNextQuestion();
	int GetAmountCorrect() const;

	void RecordAnswer(const Answer& answer);
private:
	IChallengeDataProvider* m_challengeDataProvider;
	IQuestionsReadyReceiver* m_questionsReadyReceiver;

	bool m_questionsReadyReceiverWaiting;

	int m_challengeId;
	std::queue<Question> m_questionQueue;
	int m_amountCorrect;
};