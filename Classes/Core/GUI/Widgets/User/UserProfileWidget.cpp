#include <Core/GUI/Widgets/User/UserProfileWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>
#include <IEngine.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const float LABEL_HEIGHT = 90.0;
const float LABEL_WIDTH = 585.0;

UserProfileWidget::UserProfileWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : m_user("", "", "", UserAccessLevel::INVALID), m_challengeCount(nullptr), m_challengeHighScore(nullptr), m_challengeLeaderboardPosition(nullptr) {
    
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
}

void UserProfileWidget::Init(const User& user, IProfileCloser* profileCloser) {
	m_user = user;
	m_profileCloser = profileCloser;

	AddProfileWindow();
	AddExitButton();
	AddProfileFrame();
	AddUsernameLabel();
}

void UserProfileWidget::Release() {
    if (m_challengeCount != nullptr) {
        m_challengeCount->release();
        delete m_challengeCount;
    }
    
    if (m_challengeHighScore != nullptr) {
        m_challengeHighScore->release();
        delete m_challengeHighScore;
    }
    
    if (m_challengeLeaderboardPosition != nullptr) {
        m_challengeLeaderboardPosition->release();
        delete m_challengeLeaderboardPosition;
    }
}

UIComponent* UserProfileWidget::GetWindow() {
	return m_profileWindow;
}

void UserProfileWidget::DisplayLoading() {
    AddLoading();
}

void UserProfileWidget::TakeDownLoading() {
    m_loading->release();
}

void UserProfileWidget::DisplayUserProfile(const UserProfile& userProfile) {
    AddChallengeLeaderboardPosition(userProfile.GetChallengeProfileData().GetLeaderboardPosition());
    AddChallengeCount(userProfile.GetChallengeProfileData().GetCount());
    AddChallengeHighScore(userProfile.GetChallengeProfileData().GetHighScore());
}

void UserProfileWidget::AddChallengeLeaderboardPosition(int position) {
    std::string challengeLeaderboardPosition = "leaderboard position: " + std::to_string(position);
    m_challengeLeaderboardPosition = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 0, 0, UIComponent::ANCHOR_TOP_LEFT, challengeLeaderboardPosition);
    m_challengeLeaderboardPosition->setDropShadowColor(dropShadowColor);
    m_challengeLeaderboardPosition->setX(224);
    m_challengeLeaderboardPosition->setY(100);
    m_profileFrame->addChild(m_challengeLeaderboardPosition);
}

void UserProfileWidget::AddChallengeCount(int count) {
    std::string challengeCountAsString = "attempts: " + std::to_string(count);
    m_challengeCount = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 0, 0, UIComponent::ANCHOR_TOP_LEFT, challengeCountAsString);
    m_challengeCount->setDropShadowColor(dropShadowColor);
    m_challengeCount->setX(165);
    m_challengeCount->setY(130);
    m_profileFrame->addChild(m_challengeCount);
}

void UserProfileWidget::AddChallengeHighScore(int score) {
    std::string challengeHighScore = "high score: " + std::to_string(score);
    m_challengeHighScore = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 0, 0, UIComponent::ANCHOR_TOP_LEFT, challengeHighScore);
    m_challengeHighScore->setDropShadowColor(dropShadowColor);
    m_challengeHighScore->setX(178);
    m_challengeHighScore->setY(160);
    m_profileFrame->addChild(m_challengeHighScore);
}

void UserProfileWidget::AddProfileWindow() {
	m_profileWindow = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("UIGroupArchetype"));
	m_profileWindow->setWidth(m_parentComponent->getWidth());
	m_profileWindow->setHeight(m_parentComponent->getHeight());
}

void UserProfileWidget::AddProfileFrame() {
	m_profileFrame = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
	m_profileFrame->setAnchor(UIComponent::ANCHOR_CENTER);
	m_profileFrame->setWidth(900);
	m_profileFrame->setHeight(460);

	m_profileWindow->addChild(m_profileFrame);
}

void UserProfileWidget::AddUsernameLabel() {
	m_usernameLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, m_user.GetUsername());
	m_usernameLabel->setDropShadowColor(dropShadowColor);
	m_usernameLabel->setY(6);

	m_profileFrame->addChild(m_usernameLabel);
}

void UserProfileWidget::AddExitButton() {
	m_exitButton = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_LEFT, "X");
	m_exitButton->setDropShadowColor(dropShadowColor);
	m_exitButton->setX(55);
	m_exitButton->setY(5);

	UITouchButton::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UserProfileWidget::OnExitPressed);
	m_exitButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
	
	m_profileWindow->addChild(m_exitButton);
}

void UserProfileWidget::AddLogoutButton() {
	m_logoutButton = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_RIGHT, "Logout");
	m_logoutButton->setDropShadowColor(dropShadowColor);
	m_logoutButton->setX(50);

	UITouchButton::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &UserProfileWidget::OnLogoutPressed);
	m_logoutButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

	m_profileWindow->addChild(m_logoutButton);
}

void UserProfileWidget::AddLoading() {
    m_loading = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_CENTER, "loading...");
    m_loading->setDropShadowColor(dropShadowColor);
    
    m_profileFrame->addChild(m_loading);
}

void UserProfileWidget::OnExitPressed(UITouchButton::ButtonState state) {
		m_profileCloser->CloseUserProfile();
}

void UserProfileWidget::OnLogoutPressed(UITouchButton::ButtonState state) {
	m_profileCloser->CloseUserProfile();
}
