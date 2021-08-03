#include <Core/GUI/UIState/States/Challenge/UIStateChallengeMode.h>
#include "Core/GUI/UIState/States/Challenge/UIStateChallengeMainMenu.h"
#include "Core/GUI/Components/UIComponentFactory.h"

#include "IEngine.h"
#include <math.h>

CONST_STRING_DEF(UIStateChallengeMode, UI_STATE_CHALLENGE_MODE)

void UIStateChallengeMode::OnEnterState() {
	m_challengeModeWidget = new ChallengeModeWidget();
	m_challengeModeWidget->RegisterAnswerSelectedReceiver(this);
	m_challengeModeWidget->Init(UIComponentFactory::getInstance(), IEngine::getEngine()->getUIRoot());

	UITouchButton::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UIStateChallengeMode::OnMainMenuPressed);
	m_challengeModeWidget->RegisterMainMenuPressedCallback(callBack);
    
    m_challengeModeWidget->DisplayCurrentUsername();

	m_challengeModeWidget->DisplayLoading();
	m_challengeModeWidget->DisplayPoints(m_challengeData.GetAmountCorrect() * 10);
    m_challengeModeWidget->DisplayLives();
    m_challengeModeWidget->DisplayLivesSymbol(3);
    m_challengeData.RegisterChallengeTimerReceiver(this);

	BaseStateDepricated::OnEnterState();
	
	RegisterQuestionsReadyReceiver();
	StartGame();
}

void UIStateChallengeMode::OnExitState() {
	m_challengeModeWidget->Release();
    delete m_challengeModeWidget;

    BaseStateDepricated::OnExitState();
}

void UIStateChallengeMode::RegisterQuestionsReadyReceiver() {
	m_challengeData.RegisterQuestionsReadyReceiver(this);
}

void UIStateChallengeMode::StartGame() {
    m_remainingLives = 3;
	m_challengeData.StartNewGame();
}

void UIStateChallengeMode::QuestionsReady() {
	const Question& question = m_challengeData.GetNextQuestion();
	m_challengeModeWidget->TakeDownLoading();
    DisplayQuestion(question);
    
    RegisterGameTimerTimers();
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
    m_challengeModeWidget->AnswerSelected(CORRECT);
    StartPostAnswerProcess();
}

void UIStateChallengeMode::StartPostAnswerProcess() {
    RegisterPostAnswerTimer();
    ResetGameTimer(3, TextColor::YELLOW_TEXT_COLOR);
}

void UIStateChallengeMode::AdvanceToNextQuestion() {
    m_challengeModeWidget->TakeDownAnswerState();
    m_challengeModeWidget->TakeDownQuestion();

    if (m_challengeData.ChallengeQuestionAvailable()) {
        const Question& question = m_challengeData.GetNextQuestion();
        DisplayQuestion(question);
    } else {
        DeregisterGameTimerTimers();

        m_challengeModeWidget->DisplayLoading();
        m_challengeData.ChallengeQuestionsRequested();
    }
}

void UIStateChallengeMode::DisplayQuestion(const Question& question) {
    ResetGameTimer();
    m_challengeModeWidget->DisplayQuestion(question);
}

void UIStateChallengeMode::ResetGameTimer() {
    ResetGameTimer(m_challengeModeTimerSeconds, TextColor::GREEN_TEXT_COLOR);
}

void UIStateChallengeMode::ResetGameTimer(int seconds, glm::vec3 color) {
    m_lastTimeCheck = Timer::SimpleTimer::GetCurrentTime();
    
    m_timerSecondsRemaining = seconds;
    m_challengeModeWidget->UpdateTimer(m_timerSecondsRemaining);
    m_challengeModeWidget->SetTimerColor(color);
}

void UIStateChallengeMode::HandleWrongAnswer() {
    if (m_remainingLives > 1) {
        HandleWrongAnswerLivesRemaining();
    } else {
        HandleGameOver();
    }
}

void UIStateChallengeMode::HandleWrongAnswerLivesRemaining() {
    m_remainingLives--;
    m_challengeModeWidget->UpdateLivesSymbol(m_remainingLives);
    m_challengeModeWidget->AnswerSelected(INCORRECT);
    StartPostAnswerProcess();
}

void UIStateChallengeMode::HandleGameOver() {
    UpdateSavedUserBestScore();
    m_challengeModeWidget->UpdateLivesSymbol(0);
    m_challengeModeWidget->GameOver();
    DeregisterGameTimerTimers();
}

void UIStateChallengeMode::UpdateSavedUserBestScore() {
    int currentScore = m_challengeData.GetAmountCorrect();
    
    const std::string currentBestString = IEngine::getEngine()->GetDeviceUtil()->ReadFromDeviceStorage("high_score");
    if (currentBestString.empty()) {
        IEngine::getEngine()->GetDeviceUtil()->WriteToDeviceStorage("high_score", std::to_string(currentScore));
        return;
    }
    
    int bestScore = std::atoi(currentBestString.c_str());
    
    if (currentScore > bestScore) {
        IEngine::getEngine()->GetDeviceUtil()->WriteToDeviceStorage("high_score", std::to_string(currentScore));
    }
}

void UIStateChallengeMode::OnMainMenuPressed(UITouchButton::ButtonState state) {
    m_challengeData.GameOver();
	m_challengeModeWidget->TakeDownEntireChallenge();
	ChangeState(UIStateChallengeMainMenu::UI_STATE_CHALLENGE_MAIN_MENU);
}

void UIStateChallengeMode::OnTimerEvent(Timer::TimerType type) {
    switch (type) {
        case Timer::TimerType::CHALLENGE_QUESTION_TIMER_100_MS:
            if (TimerIsExpired()) {
                m_challengeModeWidget->SetTimerColor(TextColor::RED_TEXT_COLOR);
                HandleWrongAnswer();
            }
            break;
            
        case Timer::TimerType::CHALLENGE_QUESTION_TIMER_1000_MS:
                UpdateTimer();
            break;
            
        case Timer::TimerType::CHALLENGE_POST_ANSWER_TIMER:
            DeregisterPostAnswerTimer();
            AdvanceToNextQuestion();

        default:
            break;
    }
}

void UIStateChallengeMode::UpdateTimer() {
    std::chrono::steady_clock::time_point currentTime = Timer::SimpleTimer::GetCurrentTime();
    std::chrono::steady_clock::time_point lastTimeCheck = m_lastTimeCheck;
    auto elapsedTimeSeconds = GetTimeDifferenceInMilliseconds(lastTimeCheck, currentTime) / 1000;
    
    if (ElapsedTimeIsGreaterThanOneSecond(elapsedTimeSeconds)) {
        UpdateRemainingSeconds(elapsedTimeSeconds);
        m_challengeModeWidget->UpdateTimer(m_timerSecondsRemaining);
        m_challengeModeWidget->UpdateTimerColor(m_timerSecondsRemaining, m_challengeModeTimerSeconds);
        m_lastTimeCheck = currentTime;
    }
}

bool UIStateChallengeMode::ElapsedTimeIsGreaterThanOneSecond(long long elapsedTimeSeconds) const {
    return elapsedTimeSeconds >= 1;
}

long long UIStateChallengeMode::GetTimeDifferenceInMilliseconds(std::chrono::steady_clock::time_point a, std::chrono::steady_clock::time_point b) const {
    return std::chrono::duration_cast<std::chrono::milliseconds>(b - a).count();
}

void UIStateChallengeMode::UpdateRemainingSeconds(long long elapsedTimeSeconds) {
    int secondsToRemove = floor(elapsedTimeSeconds);
    m_timerSecondsRemaining -= secondsToRemove;
}

bool UIStateChallengeMode::TimerIsExpired() const {
    return m_timerSecondsRemaining < 0;
}

void UIStateChallengeMode::OnChallengeTimerReceived(int timerSeconds) {
    m_challengeModeTimerSeconds = timerSeconds;
    m_timerSecondsRemaining = m_challengeModeTimerSeconds;
}

void UIStateChallengeMode::RegisterGameTimerTimers() {
    m_timer.RegisterTimer(Timer::TimerType::CHALLENGE_QUESTION_TIMER_100_MS);
    m_timer.RegisterTimer(Timer::TimerType::CHALLENGE_QUESTION_TIMER_1000_MS);
}

void UIStateChallengeMode::DeregisterGameTimerTimers() {
    m_timer.DeregisterTimer(Timer::TimerType::CHALLENGE_QUESTION_TIMER_100_MS);
    m_timer.DeregisterTimer(Timer::TimerType::CHALLENGE_QUESTION_TIMER_1000_MS);
}

void UIStateChallengeMode::RegisterPostAnswerTimer() {
    m_timer.RegisterTimer(Timer::TimerType::CHALLENGE_POST_ANSWER_TIMER);
}

void UIStateChallengeMode::DeregisterPostAnswerTimer() {
    m_timer.DeregisterTimer(Timer::TimerType::CHALLENGE_POST_ANSWER_TIMER);
}
