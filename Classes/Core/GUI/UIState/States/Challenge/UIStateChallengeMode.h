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
#include <Core/Util/SimpleTimer.h>


class UIStateChallengeMode : public BaseStateDepricated, IQuestionsReadyReceiver, IAnswerSelectedReceiver, Timer::SimpleTimerListener, IChallengeTimerReceiver {

public:
	UIStateChallengeMode(IStateChanageListenerDepricated* stateChangeListener) : BaseStateDepricated(stateChangeListener), m_timerSecondsRemaining(0), m_challengeModeTimerSeconds(0), m_timer(this) {};
	CONST_STRING_DEC(UI_STATE_CHALLENGE_MODE)

	virtual void OnEnterState();
	virtual void OnExitState();
	virtual STRING_ID GetStateID() { return UI_STATE_CHALLENGE_MODE; }

	virtual void QuestionsReady();
	virtual void OnAnswerSelected(const Answer& answer);
    virtual void OnTimerEvent(Timer::TimerType type);
    virtual void OnChallengeTimerReceived(int timerSeconds);
	void OnMainMenuPressed(UITouchButton::ButtonState state);

private:
	//widget contains the UI components for the Challenge Mode UI State
	ChallengeModeWidget* m_challengeModeWidget;

	//interface handles all business around the actual questions. i.e. getting questions, is question right, register user answer, etc.
	ChallengeData m_challengeData;
    
    Timer::SimpleTimer m_timer;
    int m_challengeModeTimerSeconds;
    int m_timerSecondsRemaining;
    std::chrono::steady_clock::time_point m_lastTimeCheck;

	void RegisterQuestionsReadyReceiver();
	void StartGame();

	void HandleAnswerByCorrectness(bool isCorrect);
	void HandleCorrectAnswer();
	void HandleWrongAnswer();
    
    void DisplayQuestion(const Question& question);
    void ResetGameTimer();
    
    void UpdateTimer();
    bool ElapsedTimeIsGreaterThanOneSecond(long long elapsedTime) const;
    long long GetTimeDifferenceInMilliseconds(std::chrono::steady_clock::time_point a, std::chrono::steady_clock::time_point b) const;
    void UpdateRemainingSeconds(long long elapsedTime);
    
    void RegisterTimers();
    void DeregisterTimers();
    
    bool TimerIsExpired() const;
    

};
