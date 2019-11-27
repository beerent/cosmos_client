#include <Core/GameLogic/Challenge/Gameplay/ChallengeData.h>
#include <Core/GameLogic/Challenge/Gameplay/ChallengeDataProviderFake.h>
#include <Core/GameLogic/Challenge/Gameplay/ChallengeDataProviderLive.h>

ChallengeData::ChallengeData() : m_questionsReadyReceiverWaiting(false), m_amountCorrect(0) {
	m_challengeDataProvider = new ChallengeDataProviderLive();
	m_challengeDataProvider->RegisterChallengeDataReceiver(this);
}

void ChallengeData::RegisterQuestionsReadyReceiver(IQuestionsReadyReceiver* questionsReadyReceiver) {
	m_questionsReadyReceiver = questionsReadyReceiver;
}

void ChallengeData::StartNewGame() {
	m_questionsReadyReceiverWaiting = true;
	m_challengeDataProvider->RequestChallengeId();
}

void ChallengeData::ChallengeIdReceived(int challengeId) {
	m_challengeId = challengeId;
	m_challengeDataProvider->RequestChallengeQuestions(challengeId);
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