#pragma once

#include <queue>

#include <Core/GameLogic/Question/IQuestionsReadyReceiver.h>
#include <Core/GameLogic/Question/Question.h>
#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProvider.h>
#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProviderReceiver.h>

class IChallengeTimerReceiver {
public:
    virtual void OnChallengeTimerReceived(int timerSeconds) = 0;
};

class ChallengeData : public IChallengeDataProviderReceiver {
public:
	ChallengeData();
	void StartNewGame();
    void GameOver() const;
	void RegisterQuestionsReadyReceiver(IQuestionsReadyReceiver* questionReadyReceiver);
    void RegisterChallengeTimerReceiver(IChallengeTimerReceiver* challengeTimerReceiver);

	virtual void ChallengeIdReceived(int gameId);
    virtual void ChallengeTimerReceived(int gameId);
	virtual void ChallengeQuestionsReceived(std::queue<Question> questions);

	bool ChallengeQuestionAvailable() const;
	void ChallengeQuestionsRequested();
	Question GetNextQuestion();
	int GetAmountCorrect() const;

	void RecordAnswer(const Answer& answer);
    
private:
	IChallengeDataProvider* m_challengeDataProvider;
    IChallengeTimerReceiver* m_challengeTimerReciever;
	IQuestionsReadyReceiver* m_questionsReadyReceiver;

	bool m_questionsReadyReceiverWaiting;

	int m_challengeId;
	std::queue<Question> m_questionQueue;
	int m_amountCorrect;
    
    bool ShouldRequestReview() const;
    bool RequestReview() const;
};
