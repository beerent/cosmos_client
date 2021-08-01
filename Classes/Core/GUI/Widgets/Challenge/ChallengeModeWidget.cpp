#include <Core/GUI/Widgets/Challenge/ChallengeModeWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>

#include <random>
#include <iostream>
#include <string>
#include <vector>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

const std::vector<std::string> correctAnswerOptions = {"Correct!", "Right On!", "Amazing!", "Nailed It!", "Out Of This World!", "Excellent!", "Fantastic!", "Splash Down!"};
const std::vector<std::string> incorrectAnswerOptions = {"Incorrect!", "Ouch!", "Bummer!", "Wrong!", "Nice Try!", "Oops!", "<Glass Shattering>", "Houston, We Have A Problem!", "Mission Failed!", "Next Time!"};

namespace {
	std::vector<std::string> SplitQuestionOnLength(const std::string& question) {
		std::vector<std::string> splitQuestion;
		const int max = 60;

		if (question.size() <= max) {
			splitQuestion.push_back(question);
			splitQuestion.push_back("");
			return splitQuestion;
		}

		int spaceIndex = 0;
		for (int i = max; i >= 0; i--) {
			if (question[i] == ' ') {
				spaceIndex = i;
				break;
			}
		}

		if (spaceIndex == 0) {
			splitQuestion.push_back(question);
			splitQuestion.push_back("");
			return splitQuestion;
		}

		splitQuestion.push_back(question.substr(0, spaceIndex));
		splitQuestion.push_back(question.substr(spaceIndex));

		return splitQuestion;
	}

    int GetRandomNumber(long max) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> dist6(0, (int)max-1); // distribution in range [1, 6]

        return dist6(rng);
    }
}

ChallengeModeWidget::ChallengeModeWidget() : m_currentQuestionId(-1), m_flagQuestion(nullptr), m_questionFlagged(nullptr),
    m_currentUsername(nullptr), m_timerLabel(nullptr), m_answerStateLabel(nullptr), m_livesLabel(nullptr), m_livesSymbolsLabel(nullptr) {}

void ChallengeModeWidget::Init(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;

	m_centerContainer = uiComponentFactory->createUIComponent("UIGroupArchetype", 600, 660, UIComponent::ANCHOR_CENTER);
	m_parentComponent->addChild(m_centerContainer);
}

void ChallengeModeWidget::Release() {
    m_currentUsername->release();
    delete m_currentUsername;
    
	m_centerContainer->release();
	delete(m_centerContainer);
}

void ChallengeModeWidget::RegisterAnswerSelectedReceiver(IAnswerSelectedReceiver* answerSelectedReceiver) {
	m_answerSelectedReceiver = answerSelectedReceiver;
}

void ChallengeModeWidget::RegisterMainMenuPressedCallback(UITouchButton::onButtonStateChangedCallBack callBack) {
	m_mainMenuPressedCallback = callBack;
}

void ChallengeModeWidget::DisplayCurrentUsername() {
    const std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    float usernameWidth = 12.5 * username.size();
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 60, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(80);
    m_parentComponent->addChild(m_currentUsername);
}

void ChallengeModeWidget::DisplayLoading() {
	DisplayLoadingScheme();
}

void ChallengeModeWidget::TakeDownLoading() {
    m_loadingLabel->setText("");
    m_loadingLabel->release();
}

void ChallengeModeWidget::TakeDownFlag() {
    if (m_flagQuestion != nullptr) {
        m_flagQuestion->release();
    }
}

void ChallengeModeWidget::TakeDownFlagged() {
    if (m_questionFlagged != nullptr) {
        m_questionFlagged->release();
    }
}

void ChallengeModeWidget::DisplayLives() {
    const std::string lives = "Lives [         ]";
    float livesLength = 12.5 * lives.size();
    m_livesLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", livesLength, 60, UIComponent::ANCHOR_TOP_CENTER, lives);
    m_livesLabel->setDropShadowColor(dropShadowColor);
    m_livesLabel->setX(-325);
    m_livesLabel->setY(15);
    m_parentComponent->addChild(m_livesLabel);
}

void ChallengeModeWidget::TakeDownLives() {
    if (m_livesLabel != nullptr) {
        m_livesLabel->release();
    }
}

void ChallengeModeWidget::UpdateLivesSymbol(int livesRemaining) {
    const std::string lives = GetLivesSymbolString(livesRemaining);
    const glm::vec3 color = GetLivesSymbolColor(livesRemaining);
    
    m_livesSymbolsLabel->setTextString(lives);
    m_livesSymbolsLabel->setColor(color);
}

void ChallengeModeWidget::DisplayLivesSymbol(int livesRemaining) {
    const std::string lives = GetLivesSymbolString(livesRemaining);
    
    float livesLength = 12.5 * lives.size();
    m_livesSymbolsLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", livesLength, 60, UIComponent::ANCHOR_TOP_CENTER, lives);
    m_livesSymbolsLabel->setDropShadowColor(dropShadowColor);
    m_livesSymbolsLabel->setX(-281);
    m_livesSymbolsLabel->setY(15);
    m_livesSymbolsLabel->setColor(TextColor::GREEN_TEXT_COLOR);
    m_parentComponent->addChild(m_livesSymbolsLabel);
}

void ChallengeModeWidget::TakeDownlivesSymbol() {
    if (m_livesSymbolsLabel != nullptr) {
        m_livesSymbolsLabel->release();
    }
}

std::string ChallengeModeWidget::GetLivesSymbolString(int livesRemaining) const {
    std::string lives = "";
    for (int i = 0; i < livesRemaining; i++) {
        lives += " X ";
    }
    
    for (int i = 0; i < 3 - livesRemaining; i++) {
        lives += "   ";
    }
    
    return lives;
}

glm::vec3 ChallengeModeWidget::GetLivesSymbolColor(int livesRemaining) const {
    switch (livesRemaining) {
        case 3:  return TextColor::GREEN_TEXT_COLOR;
        case 2:  return TextColor::YELLOW_TEXT_COLOR;
        default: return TextColor::RED_TEXT_COLOR;
    }
}

void ChallengeModeWidget::DisplayAnswerSelectedNotification(AnswerSelectedOption answerSelectedOption) {
    const std::string answerSelectedNotification = GetAnswerSelectedNotificationString(answerSelectedOption);
    const glm::vec3 answerSelectedNotificationColor = GetAnswerSelectedNotificationColor(answerSelectedOption);
    
    float pointsWidth = 12.5 * answerSelectedNotification.size();

    m_answerStateLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", pointsWidth, 40.0, UIComponent::ANCHOR_TOP_CENTER, answerSelectedNotification);
    m_answerStateLabel->setY(70);
    m_answerStateLabel->setColor(answerSelectedNotificationColor);
    m_answerStateLabel->setDropShadowColor(dropShadowColor);
    m_parentComponent->addChild(m_answerStateLabel);
}

std::string ChallengeModeWidget::GetAnswerSelectedNotificationString(AnswerSelectedOption answerSelectedOption) const {
    switch (answerSelectedOption) {
        case CORRECT:
            return GetCorrectAnswerString();
        case INCORRECT:
            return GetIncorrectAnswerString();
        default:
            return "GAME OVER!";
    }
}

std::string ChallengeModeWidget::GetCorrectAnswerString() const {
    int randomIndex = GetRandomNumber(correctAnswerOptions.size());
    
    return correctAnswerOptions[randomIndex];
}

std::string ChallengeModeWidget::GetIncorrectAnswerString() const {
    int randomIndex = GetRandomNumber(incorrectAnswerOptions.size());
    
    return incorrectAnswerOptions[randomIndex];
}


glm::vec3 ChallengeModeWidget::GetAnswerSelectedNotificationColor(AnswerSelectedOption answerSelectedOption) const {
    switch (answerSelectedOption) {
        case CORRECT:
            return TextColor::GREEN_TEXT_COLOR;
        case INCORRECT:
            return TextColor::YELLOW_TEXT_COLOR;
        default:
            return TextColor::RED_TEXT_COLOR;
    }
}

void ChallengeModeWidget::TakeDownAnswerState() {
    if (m_answerStateLabel != nullptr) {
        m_answerStateLabel->release();
    }
}

void ChallengeModeWidget::UpdatePoints(int points) {
    m_pointsLabel->setTextString(GetPointsString(points));
}

void ChallengeModeWidget::DisplayPoints(int points) {
    std::string pointsAsString = GetPointsString(points);
    float pointsWidth = 12.5 * pointsAsString.size();

	m_pointsLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", pointsWidth, 40.0, UIComponent::ANCHOR_TOP_CENTER, pointsAsString);
    m_pointsLabel->setY(15);
    m_pointsLabel->setDropShadowColor(dropShadowColor);
	m_parentComponent->addChild(m_pointsLabel);
}

std::string ChallengeModeWidget::GetPointsString(int points) const {
    return std::to_string(points) + " points";
}

void ChallengeModeWidget::TakeDownPoints() {
	m_pointsLabel->release();
}

void ChallengeModeWidget::UpdateTimer(int seconds) {
    if (seconds < 0) {
        seconds = 0;
    }
    
    if (m_timerLabel == nullptr) {
        DisplayTimer(seconds);
    } else {
        m_timerLabel->setTextString(GetTimerString(seconds));
    }
}

void ChallengeModeWidget::DisplayTimer(int seconds) {
    std::string timerString = GetTimerString(seconds);
    float timerWidth = 12.5 * timerString.size();

    m_timerLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", timerWidth, 40.0, UIComponent::ANCHOR_TOP_CENTER, timerString);
    m_timerLabel->setX(300);
    m_timerLabel->setY(15);
    m_timerLabel->setDropShadowColor(dropShadowColor);
    m_parentComponent->addChild(m_timerLabel);
}

std::string ChallengeModeWidget::GetTimerString(int seconds) const {
    std::string secondString = "seconds";
    if (seconds == 1) {
        secondString = "second";
    }
    
    return std::to_string(seconds) + " " + secondString;
}

void ChallengeModeWidget::UpdateTimerColor(double secondsRemaining, double totalSecondsAllowed) {
    double secondsRemainingPrecentage = secondsRemaining / totalSecondsAllowed;
    if (secondsRemainingPrecentage >= .60) {
        SetTimerColor(TextColor::GREEN_TEXT_COLOR);
    } else {
        SetTimerColor(TextColor::YELLOW_TEXT_COLOR);
    }
}

void ChallengeModeWidget::SetTimerColor(glm::vec3 color) {
    m_timerLabel->setColor(color);
}

void ChallengeModeWidget::TakeDownTimer() {
    m_timerLabel->release();
}

void ChallengeModeWidget::DisplayQuestion(const Question& question) {
	m_currentQuestionId = question.GetQuestionId();
	DisplayQuestionScheme(question);
	DisplayAnswerScheme(question);
}

void ChallengeModeWidget::TakeDownQuestion() {
	m_questionLabelLineOne->release();
	m_questionLabelLineTwo->release();

	for (auto& answerLabel : m_answers) {
		answerLabel->release();
	}
}

void ChallengeModeWidget::DisplayLoadingScheme() {
	m_loadingLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 0.0, 0.0, UIComponent::ANCHOR_CENTER, "Loading...");
    m_loadingLabel->setDropShadowColor(dropShadowColor);
	m_centerContainer->addChild(m_loadingLabel);
}

void ChallengeModeWidget::DisplayQuestionScheme(const Question& question) {
	std::vector<std::string> splitQuestion = SplitQuestionOnLength(question.GetQuestion());

	m_questionLabelLineOne = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, splitQuestion[0]);
    m_questionLabelLineOne->setDropShadowColor(dropShadowColor);
	m_questionLabelLineOne->setY(50);
	m_centerContainer->addChild(m_questionLabelLineOne);

	m_questionLabelLineTwo = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, splitQuestion[1]);
    m_questionLabelLineTwo->setDropShadowColor(dropShadowColor);
	m_questionLabelLineTwo->setY(80);
	m_centerContainer->addChild(m_questionLabelLineTwo);
}

void ChallengeModeWidget::DisplayAnswerScheme(const Question& question) {
	m_answersForCallback = question.GetRandomizedAnswers();

	if (m_answersForCallback.size() == 2) {
		Display2AnswerScheme(m_answersForCallback);
	} else {
		Display4AnswerScheme(m_answersForCallback);
	}
}

void ChallengeModeWidget::DisplayMainMenuButton() {
	m_challengeOverMainMenuButtonLabel = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", 585.0, 90.0, UIComponent::ANCHOR_BOTTOM_CENTER, "Continue");
    m_challengeOverMainMenuButtonLabel->setDropShadowColor(dropShadowColor);

	m_challengeOverMainMenuButtonLabel->registerForButtonEvent(UITouchButton::DEPRESSED, m_mainMenuPressedCallback);

	m_centerContainer->addChild(m_challengeOverMainMenuButtonLabel);
}

void ChallengeModeWidget::TakeDownEntireChallenge() {
	//memory leak, need to fix this
	//while (m_answers.size() > 0) {
	//	auto it = m_answers.begin();
	//	delete (*it);
	//	m_answers.erase(it);
	//}

	TakeDownPoints();
    TakeDownTimer();
	TakeDownQuestion();
    TakeDownFlag();
    TakeDownFlagged();
    TakeDownAnswerState();
    TakeDownLives();
    TakeDownlivesSymbol();
	m_challengeOverMainMenuButtonLabel->release();
}

void ChallengeModeWidget::Display2AnswerScheme(const std::vector<Answer>& answers) {
	DisplayAnswer(0, answers[0]);
	DisplayAnswer(1, answers[1]);
}

void ChallengeModeWidget::Display4AnswerScheme(const std::vector<Answer>& answers) {
	DisplayAnswer(0, answers[0]);
	DisplayAnswer(1, answers[1]);
	DisplayAnswer(2, answers[2]);
	DisplayAnswer(3, answers[3]);
}

void ChallengeModeWidget::DisplayFlagQuestionButton() {
	UITouchButton::onButtonStateChangedCallBack callBack;

	m_flagQuestion = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 180.0, 20.0, UIComponent::ANCHOR_RIGHT, "Flag");
	m_flagQuestion->setDropShadowColor(dropShadowColor);

	callBack.bind(this, &ChallengeModeWidget::OnQuestionFlagged);
	m_flagQuestion->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

	m_parentComponent->addChild(m_flagQuestion);
}

void ChallengeModeWidget::OnQuestionFlagged(UITouchButton::ButtonState state) {
	m_questionFlagger.FlagQuestion(m_currentQuestionId);

	m_flagQuestion->release();
	DisplayQuestionFlagged();
}

void ChallengeModeWidget::DisplayQuestionFlagged() {
	m_questionFlagged = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 225.0, 20.0, UIComponent::ANCHOR_RIGHT, "Flagged!");
	m_questionFlagged->setDropShadowColor(dropShadowColor);
	m_parentComponent->addChild(m_questionFlagged);
}

void ChallengeModeWidget::AnswerSelected(AnswerSelectedOption answerSelectedOption) {
    DisableAnswerButtons();
    TearDownAnswers();
    ShowCorrectnessRevealingColors();
    DisplayAnswerSelectedNotification(answerSelectedOption);
}

void ChallengeModeWidget::GameOver() {
    AnswerSelected(GAME_OVER);
	DisplayMainMenuButton();
}

void ChallengeModeWidget::DisableAnswerButtons() {
	for (auto& answer : m_answers) {
		answer->DisableButton();
	}

}

void ChallengeModeWidget::ShowCorrectnessRevealingColors() {
	for (int i = 0; i < m_answersForCallback.size(); i++) {
		Answer& answer = m_answersForCallback[i];

		if (answer.IsCorrect()) {
			m_answers[i]->setColor(TextColor::GREEN_TEXT_COLOR);
		} else {
			m_answers[i]->setColor(TextColor::RED_TEXT_COLOR);
		}
	}
}

void ChallengeModeWidget::DisplayAnswer(int index, const Answer& answer) {
	UILabel* answerLabel = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, answer.GetAnswer());
    answerLabel->setDropShadowColor(dropShadowColor);
	UITouchButton::onButtonStateChangedCallBack callBack = CreateButtonCallback(index);

	answerLabel->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

	float labelYPosition = 100.0;
	const float labelSpacing = 90.0;

	answerLabel->setY(labelYPosition + labelSpacing * (index + 1));

	m_answers.push_back(answerLabel);
	m_centerContainer->addChild(answerLabel);
}

UITouchButton::onButtonStateChangedCallBack ChallengeModeWidget::CreateButtonCallback(int index) {
	UITouchButton::onButtonStateChangedCallBack callBack;
	switch (index) {
	case 0:
		callBack.bind(this, &ChallengeModeWidget::OnAnswer0);
		break;
	case 1:
		callBack.bind(this, &ChallengeModeWidget::OnAnswer1);
		break;
	case 2:
		callBack.bind(this, &ChallengeModeWidget::OnAnswer2);
		break;
	case 3:
		callBack.bind(this, &ChallengeModeWidget::OnAnswer3);
		break;
	}

	return callBack;
}

void ChallengeModeWidget::OnAnswer0(UITouchButton::ButtonState state) {
	OnAnswer(0);
}

void ChallengeModeWidget::OnAnswer1(UITouchButton::ButtonState state) {
	OnAnswer(1);
}

void ChallengeModeWidget::OnAnswer2(UITouchButton::ButtonState state) {
	OnAnswer(2);
}

void ChallengeModeWidget::OnAnswer3(UITouchButton::ButtonState state) {
	OnAnswer(3);
}

void ChallengeModeWidget::OnAnswer(int index) {
	m_answerSelectedReceiver->OnAnswerSelected(m_answersForCallback[index]);
}

void ChallengeModeWidget::TearDownAnswers() {
    while (m_answers.size() > m_answersForCallback.size()) {
        auto it = m_answers.begin();
        delete (*it);
        m_answers.erase(it);
    }
}
