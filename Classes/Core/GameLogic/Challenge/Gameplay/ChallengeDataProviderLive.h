#pragma once

#include <Core/GameLogic/Challenge/Gameplay/IChallengeDataProvider.h>
#include <Core/Net/IRestConnector.h>
#include <Core/Net/IRestReceiver.h>
#include <Core/Net/Json/JsonProvider.h>


class ChallengeDataProviderLive : public IChallengeDataProvider, IRestReceiver {
public:
	ChallengeDataProviderLive();
	~ChallengeDataProviderLive();
	
	virtual void RequestChallengeId();
	virtual void RequestChallengeQuestions(int challengeId);
	virtual void SubmitChallengeAnswer(int challengeId, int answerId);

	virtual void RestReceived(const std::string& rest);

private:
	IRestConnector* m_restConnector;

	std::string m_requestChallengeIdRequestId;
	std::string m_requestChallengeQuestionsRequestId;
	std::string m_submitChallengeAnswerRequestId;

	virtual void ChallengeIdReceived(int challengeId);
    virtual void ChallengeTimerReceived(int challengeId);
	virtual void ChallengeQuestionsReceived(const std::queue<Question>& questions);

	std::queue<Question> JsonToQuestions(const json11::Json& json);
};
