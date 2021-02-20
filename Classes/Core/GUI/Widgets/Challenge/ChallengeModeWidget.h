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

namespace TextColor {
    const glm::vec3 GREEN_TEXT_COLOR(0.0f / 255.0f, 255.0f / 255.0f, 128.0f / 255.0f);
    const glm::vec3 YELLOW_TEXT_COLOR(255.0f , 255.0f, 0.0f);
    const glm::vec3 RED_TEXT_COLOR(255.0f , 0.0f, 0.0f);
}

enum AnswerSelectedOption {
    CORRECT,
    INCORRECT,
    GAME_OVER
};

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
    
    std::string GetPointsString(int points) const;
    std::string GetTimerString(int seconds) const;
    
    void UpdateTimer(int points);
    void DisplayTimer(int points);
    void TakeDownTimer();
    void UpdateTimerColor(double secondsRemaining, double totalSecondsAllowed);
    void SetTimerColor(glm::vec3 color);

	void DisplayQuestion(const Question& question);
	void TakeDownQuestion();
    
    void TakeDownFlag();
    void TakeDownFlagged();
    
    void DisplayAnswerSelectedNotification(AnswerSelectedOption answerSelectedOption);
    void TakeDownAnswerState();

    void AnswerSelected(AnswerSelectedOption answerSelectedOption);
	void GameOver();
	void TakeDownEntireChallenge();
    
    void DisplayCurrentUsername();
    
    void DisplayLives();
    void TakeDownLives();
    
    void UpdateLivesSymbol(int livesRemaining);
    void DisplayLivesSymbol(int livesRemaining);
    void TakeDownlivesSymbol();
    std::string GetLivesSymbolString(int livesRemaining) const;
    glm::vec3 GetLivesSymbolColor(int livesRemaining) const;

private:
	int m_currentQuestionId;

	//answers for callback
	std::vector<Answer> m_answersForCallback;
	//UI Components
	UIComponentFactory* m_uiComponentFactory;
	UIComponent* m_parentComponent;

	UIComponent *m_centerContainer;
    UILabel* m_currentUsername;
	UILabel* m_loadingLabel;
	UILabel* m_pointsLabel;
    UILabel* m_timerLabel;
    UILabel* m_answerStateLabel;
	UILabel* m_challengeOverLabel;
	UILabel* m_challengeOverMainMenuButtonLabel;
	UILabel* m_questionLabelLineOne;
	UILabel* m_questionLabelLineTwo;
    UILabel* m_livesLabel;
    UILabel* m_livesSymbolsLabel;
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
	void ShowCorrectnessRevealingColors();

	void DisplayFlagQuestionButton();
	void DisplayQuestionFlagged();

	void DisplayMainMenuButton();
    
    std::string GetAnswerSelectedNotificationString(AnswerSelectedOption answerSelectedOption) const;
    std::string GetCorrectAnswerString() const;
    std::string GetIncorrectAnswerString() const;
    glm::vec3 GetAnswerSelectedNotificationColor(AnswerSelectedOption answerSelectedOption) const;

	UITouchButton::onButtonStateChangedCallBack CreateButtonCallback(int index);
	void OnAnswer0(UITouchButton::ButtonState state);
	void OnAnswer1(UITouchButton::ButtonState state);
	void OnAnswer2(UITouchButton::ButtonState state);
	void OnAnswer3(UITouchButton::ButtonState state);
	void OnAnswer(int index);
    void TearDownAnswers();

	void OnQuestionFlagged(UITouchButton::ButtonState state);
};
