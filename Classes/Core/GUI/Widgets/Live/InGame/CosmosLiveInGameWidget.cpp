#include <Core/GUI/Widgets/Live/InGame/CosmosLiveInGameWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>

#include <random>
#include <iostream>
#include <string>
#include <vector>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

const std::vector<std::string> CORRECT1AnswerOptions = {"CORRECT1!", "Right On!", "Amazing!", "Nailed It!", "Gnarly!", "Out Of This World!", "Certainly!", "Righteous!", "Okay!", "Impeccable!", "On The Mark!", "On Target!", "Excellent!", "Fantastic!", "Mission Complete!"};

const std::vector<std::string> INCORRECT11AnswerOptions = {"INCORRECT11!", "Ouch!", "Bummer!", "Yikes!", "Nice Try!", "Oops!", "Flaw Detected!", "Untrue!", "Dang!", "Cringe!", "EEERRRTTTT!", "<Glass Shattering>", "Houston, We Have A Problem!", "Mission Failed!", "Next Time!"};

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

CosmosLiveInGameWidget::CosmosLiveInGameWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : m_currentQuestionId(-1), m_flagQuestion(nullptr), m_questionFlagged(nullptr),
    m_currentUsername(nullptr), m_timerLabel(nullptr), m_answerStateLabel(nullptr), m_livesLabel(nullptr), m_livesSymbolsLabel(nullptr) {
        m_uiComponentFactory = uiComponentFactory;
        m_parentComponent = parentComponent;        
    }

void CosmosLiveInGameWidget::Init() {
    m_centerContainer = m_uiComponentFactory->createUIComponent("UIGroupArchetype", 600, 660, UIComponent::ANCHOR_CENTER);
    m_parentComponent->addChild(m_centerContainer);
}

void CosmosLiveInGameWidget::Release() {
    m_currentUsername->release();
    delete m_currentUsername;
    
    m_centerContainer->release();
    delete(m_centerContainer);
}

void CosmosLiveInGameWidget::RegisterAnswerSelectedReceiver(IAnswerSelectedReceiver* answerSelectedReceiver) {
    m_answerSelectedReceiver = answerSelectedReceiver;
}

void CosmosLiveInGameWidget::RegisterMainMenuPressedCallback(UITouchButton::onButtonStateChangedCallBack callBack) {
    m_mainMenuPressedCallback = callBack;
}

void CosmosLiveInGameWidget::DisplayCurrentUsername() {
    const std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    float usernameWidth = 12.5 * username.size();
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 60, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(80);
    m_parentComponent->addChild(m_currentUsername);
}

void CosmosLiveInGameWidget::DisplayLoading() {
    DisplayLoadingScheme();
}

void CosmosLiveInGameWidget::TakeDownLoading() {
    m_loadingLabel->setText("");
    m_loadingLabel->release();
}

void CosmosLiveInGameWidget::TakeDownFlag() {
    if (m_flagQuestion != nullptr) {
        m_flagQuestion->release();
    }
}

void CosmosLiveInGameWidget::TakeDownFlagged() {
    if (m_questionFlagged != nullptr) {
        m_questionFlagged->release();
    }
}

void CosmosLiveInGameWidget::DisplayLives() {
    const std::string lives = "Lives [         ]";
    float livesLength = 12.5 * lives.size();
    m_livesLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", livesLength, 60, UIComponent::ANCHOR_TOP_CENTER, lives);
    m_livesLabel->setDropShadowColor(dropShadowColor);
    m_livesLabel->setX(-325);
    m_livesLabel->setY(15);
    m_parentComponent->addChild(m_livesLabel);
}

void CosmosLiveInGameWidget::TakeDownLives() {
    if (m_livesLabel != nullptr) {
        m_livesLabel->release();
    }
}

void CosmosLiveInGameWidget::UpdateLivesSymbol(int livesRemaining) {
    const std::string lives = GetLivesSymbolString(livesRemaining);
    const glm::vec3 color = GetLivesSymbolColor(livesRemaining);
    
    m_livesSymbolsLabel->setTextString(lives);
    m_livesSymbolsLabel->setColor(color);
}

void CosmosLiveInGameWidget::DisplayLivesSymbol(int livesRemaining) {
    const std::string lives = GetLivesSymbolString(livesRemaining);
    
    float livesLength = 12.5 * lives.size();
    m_livesSymbolsLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", livesLength, 60, UIComponent::ANCHOR_TOP_CENTER, lives);
    m_livesSymbolsLabel->setDropShadowColor(dropShadowColor);
    m_livesSymbolsLabel->setX(-281);
    m_livesSymbolsLabel->setY(15);
    m_livesSymbolsLabel->setColor(TextColor::GREEN_TEXT_COLOR1);
    m_parentComponent->addChild(m_livesSymbolsLabel);
}

void CosmosLiveInGameWidget::TakeDownlivesSymbol() {
    if (m_livesSymbolsLabel != nullptr) {
        m_livesSymbolsLabel->release();
    }
}

std::string CosmosLiveInGameWidget::GetLivesSymbolString(int livesRemaining) const {
    std::string lives = "";
    for (int i = 0; i < livesRemaining; i++) {
        lives += " X ";
    }
    
    for (int i = 0; i < 3 - livesRemaining; i++) {
        lives += "   ";
    }
    
    return lives;
}

glm::vec3 CosmosLiveInGameWidget::GetLivesSymbolColor(int livesRemaining) const {
    switch (livesRemaining) {
        case 3:  return TextColor::GREEN_TEXT_COLOR1;
        case 2:  return TextColor::YELLOW_TEXT_COLOR1;
        default: return TextColor::RED_TEXT_COLOR1;
    }
}

void CosmosLiveInGameWidget::DisplayAnswerSelectedNotification(AnswerSelectedOption1 AnswerSelectedOption1) {
    const std::string answerSelectedNotification = GetAnswerSelectedNotificationString(AnswerSelectedOption1);
    const glm::vec3 answerSelectedNotificationColor = GetAnswerSelectedNotificationColor(AnswerSelectedOption1);
    
    float pointsWidth = 12.5 * answerSelectedNotification.size();

    m_answerStateLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", pointsWidth, 40.0, UIComponent::ANCHOR_TOP_CENTER, answerSelectedNotification);
    m_answerStateLabel->setY(70);
    m_answerStateLabel->setColor(answerSelectedNotificationColor);
    m_answerStateLabel->setDropShadowColor(dropShadowColor);
    m_parentComponent->addChild(m_answerStateLabel);
}

std::string CosmosLiveInGameWidget::GetAnswerSelectedNotificationString(AnswerSelectedOption1 AnswerSelectedOption1) const {
    switch (AnswerSelectedOption1) {
        case CORRECT1:
            return GetCorrectAnswerString();
        case INCORRECT1:
            return GetIncorrectAnswerString();
        default:
            return "GAME OVER!";
    }
}

std::string CosmosLiveInGameWidget::GetCorrectAnswerString() const {
    int randomIndex = GetRandomNumber(CORRECT1AnswerOptions.size());
    
    return CORRECT1AnswerOptions[randomIndex];
}

std::string CosmosLiveInGameWidget::GetIncorrectAnswerString() const {
    int randomIndex = GetRandomNumber(INCORRECT11AnswerOptions.size());
    
    return INCORRECT11AnswerOptions[randomIndex];
}


glm::vec3 CosmosLiveInGameWidget::GetAnswerSelectedNotificationColor(AnswerSelectedOption1 AnswerSelectedOption1) const {
    switch (AnswerSelectedOption1) {
        case CORRECT1:
            return TextColor::GREEN_TEXT_COLOR1;
        case INCORRECT1:
            return TextColor::YELLOW_TEXT_COLOR1;
        default:
            return TextColor::RED_TEXT_COLOR1;
    }
}

void CosmosLiveInGameWidget::TakeDownAnswerState() {
    if (m_answerStateLabel != nullptr) {
        m_answerStateLabel->release();
    }
}

void CosmosLiveInGameWidget::UpdatePoints(int points) {
    m_pointsLabel->setTextString(GetPointsString(points));
}

void CosmosLiveInGameWidget::DisplayPoints(int points) {
    std::string pointsAsString = GetPointsString(points);
    float pointsWidth = 12.5 * pointsAsString.size();

    m_pointsLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", pointsWidth, 40.0, UIComponent::ANCHOR_TOP_CENTER, pointsAsString);
    m_pointsLabel->setY(15);
    m_pointsLabel->setDropShadowColor(dropShadowColor);
    m_parentComponent->addChild(m_pointsLabel);
}

std::string CosmosLiveInGameWidget::GetPointsString(int points) const {
    return std::to_string(points) + " points";
}

void CosmosLiveInGameWidget::TakeDownPoints() {
    m_pointsLabel->release();
}

void CosmosLiveInGameWidget::UpdateTimer(int seconds) {
    if (seconds < 0) {
        seconds = 0;
    }
    
    if (m_timerLabel == nullptr) {
        DisplayTimer(seconds);
    } else {
        m_timerLabel->setTextString(GetTimerString(seconds));
    }
}

void CosmosLiveInGameWidget::DisplayTimer(int seconds) {
    std::string timerString = GetTimerString(seconds);
    float timerWidth = 12.5 * timerString.size();

    m_timerLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", timerWidth, 40.0, UIComponent::ANCHOR_TOP_CENTER, timerString);
    m_timerLabel->setX(300);
    m_timerLabel->setY(15);
    m_timerLabel->setDropShadowColor(dropShadowColor);
    m_parentComponent->addChild(m_timerLabel);
}

std::string CosmosLiveInGameWidget::GetTimerString(int seconds) const {
    std::string secondString = "seconds";
    if (seconds == 1) {
        secondString = "second";
    }
    
    return std::to_string(seconds) + " " + secondString;
}

void CosmosLiveInGameWidget::UpdateTimerColor(double secondsRemaining, double totalSecondsAllowed) {
    double secondsRemainingPrecentage = secondsRemaining / totalSecondsAllowed;
    if (secondsRemainingPrecentage >= .60) {
        SetTimerColor(TextColor::GREEN_TEXT_COLOR1);
    } else {
        SetTimerColor(TextColor::YELLOW_TEXT_COLOR1);
    }
}

void CosmosLiveInGameWidget::SetTimerColor(glm::vec3 color) {
    m_timerLabel->setColor(color);
}

void CosmosLiveInGameWidget::TakeDownTimer() {
    m_timerLabel->release();
}

void CosmosLiveInGameWidget::DisplayQuestion(const Question& question) {
    m_currentQuestionId = question.GetQuestionId();
    DisplayQuestionScheme(question);
    DisplayAnswerScheme(question);
}

void CosmosLiveInGameWidget::TakeDownQuestion() {
    m_questionLabelLineOne->release();
    m_questionLabelLineTwo->release();

    for (auto& answerLabel : m_answers) {
        answerLabel->release();
    }
}

void CosmosLiveInGameWidget::DisplayLoadingScheme() {
    m_loadingLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 0.0, 0.0, UIComponent::ANCHOR_CENTER, "Loading...");
    m_loadingLabel->setDropShadowColor(dropShadowColor);
    m_centerContainer->addChild(m_loadingLabel);
}

void CosmosLiveInGameWidget::DisplayQuestionScheme(const Question& question) {
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

void CosmosLiveInGameWidget::DisplayAnswerScheme(const Question& question) {
    m_answersForCallback = question.GetRandomizedAnswers();

    if (m_answersForCallback.size() == 2) {
        Display2AnswerScheme(m_answersForCallback);
    } else {
        Display4AnswerScheme(m_answersForCallback);
    }
}

void CosmosLiveInGameWidget::DisplayMainMenuButton() {
    m_challengeOverMainMenuButtonLabel = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", 585.0, 90.0, UIComponent::ANCHOR_BOTTOM_CENTER, "Continue");
    m_challengeOverMainMenuButtonLabel->setDropShadowColor(dropShadowColor);

    m_challengeOverMainMenuButtonLabel->registerForButtonEvent(UITouchButton::DEPRESSED, m_mainMenuPressedCallback);

    m_centerContainer->addChild(m_challengeOverMainMenuButtonLabel);
}

void CosmosLiveInGameWidget::TakeDownEntireChallenge() {
    //memory leak, need to fix this
    //while (m_answers.size() > 0) {
    //    auto it = m_answers.begin();
    //    delete (*it);
    //    m_answers.erase(it);
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

void CosmosLiveInGameWidget::Display2AnswerScheme(const std::vector<Answer>& answers) {
    DisplayAnswer(0, answers[0]);
    DisplayAnswer(1, answers[1]);
}

void CosmosLiveInGameWidget::Display4AnswerScheme(const std::vector<Answer>& answers) {
    DisplayAnswer(0, answers[0]);
    DisplayAnswer(1, answers[1]);
    DisplayAnswer(2, answers[2]);
    DisplayAnswer(3, answers[3]);
}

void CosmosLiveInGameWidget::DisplayFlagQuestionButton() {
    UITouchButton::onButtonStateChangedCallBack callBack;

    m_flagQuestion = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 180.0, 20.0, UIComponent::ANCHOR_RIGHT, "Flag");
    m_flagQuestion->setDropShadowColor(dropShadowColor);

    callBack.bind(this, &CosmosLiveInGameWidget::OnQuestionFlagged);
    m_flagQuestion->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

    m_parentComponent->addChild(m_flagQuestion);
}

void CosmosLiveInGameWidget::OnQuestionFlagged(UITouchButton::ButtonState state) {
    m_questionFlagger.FlagQuestion(m_currentQuestionId);

    m_flagQuestion->release();
    DisplayQuestionFlagged();
}

void CosmosLiveInGameWidget::DisplayQuestionFlagged() {
    m_questionFlagged = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 225.0, 20.0, UIComponent::ANCHOR_RIGHT, "Flagged!");
    m_questionFlagged->setDropShadowColor(dropShadowColor);
    m_parentComponent->addChild(m_questionFlagged);
}

void CosmosLiveInGameWidget::AnswerSelected(AnswerSelectedOption1 AnswerSelectedOption1) {
    DisableAnswerButtons();
    TearDownAnswers();
    ShowCorrectnessRevealingColors();
    DisplayAnswerSelectedNotification(AnswerSelectedOption1);
}

void CosmosLiveInGameWidget::GameOver() {
    AnswerSelected(GAME_OVER1);
    DisplayMainMenuButton();
}

void CosmosLiveInGameWidget::DisableAnswerButtons() {
    for (auto& answer : m_answers) {
        answer->DisableButton();
    }

}

void CosmosLiveInGameWidget::ShowCorrectnessRevealingColors() {
    for (int i = 0; i < m_answersForCallback.size(); i++) {
        Answer& answer = m_answersForCallback[i];

        if (answer.IsCorrect()) {
            m_answers[i]->setColor(TextColor::GREEN_TEXT_COLOR1);
        } else {
            m_answers[i]->setColor(TextColor::RED_TEXT_COLOR1);
        }
    }
}

void CosmosLiveInGameWidget::DisplayAnswer(int index, const Answer& answer) {
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

UITouchButton::onButtonStateChangedCallBack CosmosLiveInGameWidget::CreateButtonCallback(int index) {
    UITouchButton::onButtonStateChangedCallBack callBack;
    switch (index) {
    case 0:
        callBack.bind(this, &CosmosLiveInGameWidget::OnAnswer0);
        break;
    case 1:
        callBack.bind(this, &CosmosLiveInGameWidget::OnAnswer1);
        break;
    case 2:
        callBack.bind(this, &CosmosLiveInGameWidget::OnAnswer2);
        break;
    case 3:
        callBack.bind(this, &CosmosLiveInGameWidget::OnAnswer3);
        break;
    }

    return callBack;
}

void CosmosLiveInGameWidget::OnAnswer0(UITouchButton::ButtonState state) {
    OnAnswer(0);
}

void CosmosLiveInGameWidget::OnAnswer1(UITouchButton::ButtonState state) {
    OnAnswer(1);
}

void CosmosLiveInGameWidget::OnAnswer2(UITouchButton::ButtonState state) {
    OnAnswer(2);
}

void CosmosLiveInGameWidget::OnAnswer3(UITouchButton::ButtonState state) {
    OnAnswer(3);
}

void CosmosLiveInGameWidget::OnAnswer(int index) {
    m_answerSelectedReceiver->OnAnswerSelected(m_answersForCallback[index]);
}

void CosmosLiveInGameWidget::TearDownAnswers() {
    while (m_answers.size() > m_answersForCallback.size()) {
        auto it = m_answers.begin();
        delete (*it);
        m_answers.erase(it);
    }
}
