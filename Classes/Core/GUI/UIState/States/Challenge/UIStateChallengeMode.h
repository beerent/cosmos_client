#pragma once

#include <stdio.h>
#include "Core/StringManager/StringManager.h"
#include "Core/Generic/StateMachine/BaseState.h"
#include "Core/GUI/Components/UILabel.h"

#include <Core/GUI/Widgets/Challenge/ChallengeModeWidget.h>
#include <Core/GameLogic/Question/IQuestionsReadyReceiver.h>
#include <Core/GameLogic/Question/IAnswerSelectedReceiver.h>
#include <Core/GameLogic/Question/Question.h>
#include <Core/GameLogic/Question/Answer.h>
#include <Core/GameLogic/Challenge/Gameplay/ChallengeData.h>


class UIStateChallengeMode : public BaseStateDepricated, IQuestionsReadyReceiver, IAnswerSelectedReceiver {

public:
	UIStateChallengeMode(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener) {};
	CONST_STRING_DEC(UI_STATE_CHALLENGE_MODE)

	virtual void OnEnterState();
	virtual void OnExitState();
	virtual STRING_ID GetStateID() { return UI_STATE_CHALLENGE_MODE; }

	virtual void QuestionsReady();
	virtual void OnAnswerSelected(const Answer& answer);
	void OnMainMenuPressed(UITouchButton::ButtonState state);

private:
	//widget contains the UI components for the Challenge Mode UI State
	ChallengeModeWidget* m_challengeModeWidget;

	//interface handles all business around the actual questions. i.e. getting questions, is question right, register user answer, etc.
	ChallengeData m_challengeData;

	void RegisterQuestionsReadyReceiver();
	void StartGame();

	void HandleAnswerByCorrectness(bool isCorrect);
	void HandleCorrectAnswer();
	void HandleWrongAnswer();

};