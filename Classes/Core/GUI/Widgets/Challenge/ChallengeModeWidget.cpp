#include <Core/GUI/Widgets/Challenge/ChallengeModeWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>

#include <string>

const glm::vec3 GREEN_TEXT_COLOR(0.0f, 225.0f, 0.0f);
const glm::vec3 RED_TEXT_COLOR(255.0f , 0.0f, 0.0f);
const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

namespace {
	std::vector<std::string> SplitQuestionOnLength(const std::string& question) {
		std::vector<std::string> splitQuestion;
		const int max = 90;

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
}

ChallengeModeWidget::ChallengeModeWidget() : m_currentQuestionId(-1), m_flagQuestion(nullptr), m_questionFlagged(nullptr), m_currentUsername(nullptr) {}

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
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 40, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(50);
    m_parentComponent->addChild(m_currentUsername);
}

void ChallengeModeWidget::DisplayLoading() {
	DisplayLoadingScheme();
}

void ChallengeModeWidget::TakeDownLoading() {
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

void ChallengeModeWidget::UpdatePoints(int points) {
	TakeDownPoints();
	DisplayPoints(points);
}

void ChallengeModeWidget::DisplayPoints(int points) {
	std::string pointsAsString = std::to_string(points) + " points";

	m_pointsLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_LEFT, pointsAsString);
    m_pointsLabel->setDropShadowColor(dropShadowColor);
	m_parentComponent->addChild(m_pointsLabel);
}

void ChallengeModeWidget::TakeDownPoints() {
	m_pointsLabel->release();
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
	m_loadingLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_CENTER, "Loading...");
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

	DisplayFlagQuestionButton();
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
	TakeDownQuestion();
    TakeDownFlag();
    TakeDownFlagged();
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

void ChallengeModeWidget::GameOver() {
	DisableAnswerButtons();
	SetCorrectnessRevealingColors();
	DisplayMainMenuButton();
}

void ChallengeModeWidget::DisableAnswerButtons() {
	for (auto& answer : m_answers) {
		answer->DisableButton();
	}

}

void ChallengeModeWidget::SetCorrectnessRevealingColors() {
	for (int i = 0; i < m_answersForCallback.size(); i++) {
		Answer& answer = m_answersForCallback[i];

		if (answer.IsCorrect()) {
			m_answers[i]->setColor(GREEN_TEXT_COLOR);
		} else {
			m_answers[i]->setColor(RED_TEXT_COLOR);
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
	//this is the worst place to handle this but the button onclick is iterating over listeners.
	//if you delete the button before it finishes iterating over listeners an exception is thrown.
	while (m_answers.size() > m_answersForCallback.size()) {
		auto it = m_answers.begin();
		delete (*it);
		m_answers.erase(it);
	}

	m_answerSelectedReceiver->OnAnswerSelected(m_answersForCallback[index]);
}
