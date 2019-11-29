#pragma once

#include "Core/OpenSource/FastDelegate.h"
#include <list>
#include "Core/GUI/Components/UITouchButton.h"
#include "Core/GUI/Components/UILabel.h"
#include <Core/GUI/Components/UIComponentFactory.h>

#include <Core/GameLogic/Question/Question.h>
#include <Core/GameLogic/Question/Answer.h>
#include <Core/GameLogic/Question/IAnswerSelectedReceiver.h>

#include <Core/GameLogic/Question/Flag/QuestionFlagger.h>

class ChallengeModeWidget {
public:

	ChallengeModeWidget();

	void Init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent);
	void Release();

	void RegisterAnswerSelectedReceiver(IAnswerSelectedReceiver* answerSelectedReceiver);
	void RegisterMainMenuPressedCallback(UITouchButton::onButtonStateChangedCallBack callBack);

	void DisplayLoading();
	void TakeDownLoading();

	void UpdatePoints(int points);
	void DisplayPoints(int points);
	void TakeDownPoints();

	void DisplayQuestion(const Question& question);
	void TakeDownQuestion();
    
    void TakeDownFlag();
    void TakeDownFlagged();

	void GameOver();
	void TakeDownEntireChallenge();

private:
	int m_currentQuestionId;

	//answers for callback
	std::vector<Answer> m_answersForCallback;
	//UI Components
	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UIComponent *m_centerContainer;
	UILabel* m_loadingLabel;
	UILabel* m_pointsLabel;
	UILabel* m_challengeOverLabel;
	UILabel* m_challengeOverMainMenuButtonLabel;
	UILabel* m_questionLabelLineOne;
	UILabel* m_questionLabelLineTwo;
	std::vector<UILabel*> m_answers;

	UILabel* m_flagQuestion;
	UILabel* m_questionFlagged;

	UITouchButton::onButtonStateChangedCallBack m_mainMenuPressedCallback;
	
	// listeners
	IAnswerSelectedReceiver* m_answerSelectedReceiver;

	QuestionFlagger m_questionFlagger;


	void DisplayLoadingScheme();
	void DisplayQuestionScheme(const Question& question);
	void DisplayAnswerScheme(const Question& question);
	void Display2AnswerScheme(const std::vector<Answer>& answers);
	void Display4AnswerScheme(const std::vector<Answer>& answers);
	void DisplayAnswer(int index, const Answer& answer);

	void DisableAnswerButtons();
	void SetCorrectnessRevealingColors();

	void DisplayFlagQuestionButton();
	void DisplayQuestionFlagged();

	void DisplayMainMenuButton();

	UITouchButton::onButtonStateChangedCallBack CreateButtonCallback(int index);
	void OnAnswer0(UITouchButton::ButtonState state);
	void OnAnswer1(UITouchButton::ButtonState state);
	void OnAnswer2(UITouchButton::ButtonState state);
	void OnAnswer3(UITouchButton::ButtonState state);
	void OnAnswer(int index);

	void OnQuestionFlagged(UITouchButton::ButtonState state);
};
