#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include "Core/GUI/Components/UIComponentFactory.h"

#include "IEngine.h"

CONST_STRING_DEF(UIStateChallengeMode, UI_STATE_CHALLENGE_MODE)

void UIStateChallengeMode::OnEnterState() {
	m_challengeModeWidget = new ChallengeModeWidget();
	m_challengeModeWidget->RegisterAnswerSelectedReceiver(this);
	m_challengeModeWidget->Init(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());

	UITouchButton::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UIStateChallengeMode::OnMainMenuPressed);
	m_challengeModeWidget->RegisterMainMenuPressedCallback(callBack);

	m_challengeModeWidget->DisplayLoading();
	m_challengeModeWidget->DisplayPoints(m_challengeData.GetAmountCorrect() * 10);

	BaseStateDepricated::OnEnterState();
	
	RegisterQuestionsReadyReceiver();
	StartGame();
}

void UIStateChallengeMode::OnExitState() {
	m_challengeModeWidget->Release();
	BaseStateDepricated::OnExitState();
}

void UIStateChallengeMode::RegisterQuestionsReadyReceiver() {
	m_challengeData.RegisterQuestionsReadyReceiver(this);
}

void UIStateChallengeMode::StartGame() {
	m_challengeData.StartNewGame();
}

void UIStateChallengeMode::QuestionsReady() {
	const Question& question = m_challengeData.GetNextQuestion();
	m_challengeModeWidget->TakeDownLoading();
	m_challengeModeWidget->DisplayQuestion(question);
}

void UIStateChallengeMode::OnAnswerSelected(const Answer& answer) {
    m_challengeModeWidget->TakeDownFlag();
    m_challengeModeWidget->TakeDownFlagged();
	m_challengeData.RecordAnswer(answer);
	m_challengeModeWidget->UpdatePoints(m_challengeData.GetAmountCorrect() * 10);
	HandleAnswerByCorrectness(answer.IsCorrect());
}

void UIStateChallengeMode::HandleAnswerByCorrectness(bool isCorrect) {
	if (isCorrect) {
		HandleCorrectAnswer();
	} else {
		HandleWrongAnswer();
	}
}

void UIStateChallengeMode::HandleCorrectAnswer() {
	m_challengeModeWidget->TakeDownQuestion();

	if (m_challengeData.ChallengeQuestionAvailable()) {
		const Question& question = m_challengeData.GetNextQuestion();
		m_challengeModeWidget->DisplayQuestion(question);
	} else {
		m_challengeModeWidget->DisplayLoading();
		m_challengeData.ChallengeQuestionsRequested();
	}
}

void UIStateChallengeMode::HandleWrongAnswer() {
	m_challengeModeWidget->GameOver();
}

void UIStateChallengeMode::OnMainMenuPressed(UITouchButton::ButtonState state) {
	m_challengeModeWidget->TakeDownEntireChallenge();
	ChangeState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
}
