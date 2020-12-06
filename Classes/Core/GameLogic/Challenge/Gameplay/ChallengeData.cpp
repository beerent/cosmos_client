#include <Core/GameLogic/Challenge/Gameplay/ChallengeData.h>
#include <Core/GameLogic/Challenge/Gameplay/ChallengeDataProviderFake.h>
#include <Core/GameLogic/Challenge/Gameplay/ChallengeDataProviderLive.h>
#include "IEngine.h"

static const int MIN_CORRECT_QUESTIONS_TO_REQUEST_REVIEW(4);

ChallengeData::ChallengeData() : m_questionsReadyReceiverWaiting(false), m_amountCorrect(0) {
    //m_challengeDataProvider = new ChallengeDataProviderFake();
    m_challengeDataProvider = new ChallengeDataProviderLive();
	m_challengeDataProvider->RegisterChallengeDataReceiver(this);
}

void ChallengeData::RegisterQuestionsReadyReceiver(IQuestionsReadyReceiver* questionsReadyReceiver) {
	m_questionsReadyReceiver = questionsReadyReceiver;
}

void ChallengeData::RegisterChallengeTimerReceiver(IChallengeTimerReceiver* challengeTimerReciever) {
    m_challengeTimerReciever = challengeTimerReciever;
}

void ChallengeData::StartNewGame() {
	m_questionsReadyReceiverWaiting = true;
	m_challengeDataProvider->RequestChallengeId();
}

void ChallengeData::GameOver() const {
    if (ShouldRequestReview()) {
        RequestReview();
    }
}

bool ChallengeData::ShouldRequestReview() const {
    return GetAmountCorrect() >= MIN_CORRECT_QUESTIONS_TO_REQUEST_REVIEW;
}

bool ChallengeData::RequestReview() const {
    IEngine::getEngine()->GetRateAppController()->RateAppRequest();
}

void ChallengeData::ChallengeIdReceived(int challengeId) {
	m_challengeId = challengeId;
	m_challengeDataProvider->RequestChallengeQuestions(challengeId);
}

void ChallengeData::ChallengeTimerReceived(int timerSeconds) {
    m_challengeTimerReciever->OnChallengeTimerReceived(timerSeconds);
}

void ChallengeData::ChallengeQuestionsReceived(std::queue<Question> questions) {
	m_questionQueue = questions;
	if (m_questionsReadyReceiverWaiting) {
		m_questionsReadyReceiver->QuestionsReady();
		m_questionsReadyReceiverWaiting = false;
	}
}

bool ChallengeData::ChallengeQuestionAvailable() const {
	return m_questionQueue.size() > 0;
}

void ChallengeData::ChallengeQuestionsRequested() {
	m_questionsReadyReceiverWaiting = true;
	m_challengeDataProvider->RequestChallengeQuestions(m_challengeId);
}

Question ChallengeData::GetNextQuestion() {
	Question question = m_questionQueue.front();
	m_questionQueue.pop();

	return question;
}

void ChallengeData::RecordAnswer(const Answer& answer) {
	if (answer.IsCorrect()) {
		m_amountCorrect++;
	}

	m_challengeDataProvider->SubmitChallengeAnswer(m_challengeId, answer.GetId());
}

int ChallengeData::GetAmountCorrect() const {
	return m_amountCorrect;
}
