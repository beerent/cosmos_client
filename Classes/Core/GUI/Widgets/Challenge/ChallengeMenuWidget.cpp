#include <Core/GUI/Widgets/Challenge/ChallengeMenuWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>
#include <IEngine.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

ChallengeMenuWidget::ChallengeMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : IUserProfileDisplayListener(uiComponentFactory, parentComponent),
      m_home(nullptr), m_leaderboard(nullptr), m_newGameButton(nullptr), m_currentUsername(nullptr), m_waitingForLeaderboard(false) {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
}

void ChallengeMenuWidget::Init() {
    m_parentComponent->setVisible(false);
    AddCurrentUserUsername();
	AddHomeButton();
	AddChallengeTitle();
	AddLeaderboardFrame();
	AddLeaderboardTitle();
	AddLeaderboardContents();
    m_parentComponent->setVisible(true);
}

void ChallengeMenuWidget::Release() {
    FinishWaitingForLeaderboard();

    m_home->release();
	delete m_home;
    
    m_currentUsername->release();
    delete m_currentUsername;

	m_leaderboardTitle->release();
	delete m_leaderboardTitle;

	m_challengeTitle->release();
	delete m_challengeTitle;

	for (UILabel* label : m_leaderboardEntries) {
		label->release();
		delete label;
	}

	for (UIUsernameLabel* usernameLabel : m_leaderboardClickListeners) {
		delete usernameLabel;
	}

    if (m_leaderboard != nullptr) {
        m_leaderboard->release();
        delete m_leaderboard;
    }
    
    if (m_newGameButton != nullptr) {
        m_newGameButton->release();
        delete m_newGameButton;
    }
}

void ChallengeMenuWidget::RegisterForChallengeMenuItemSelectedEvent(onChallengeMenuItemSelectedCallBack callback) {
	m_onChallengeMenuItemSelectedListeners.push_back(callback);
}

void ChallengeMenuWidget::UnregisterForChallengeMenuItemSelectedEvent(onChallengeMenuItemSelectedCallBack callback) {
	m_onChallengeMenuItemSelectedListeners.remove(callback);
}

void ChallengeMenuWidget::AddCurrentUserUsername() {
    const std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    float usernameWidth = 12.5 * username.size();
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 40, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(50);
    m_parentComponent->addChild(m_currentUsername);
}

void ChallengeMenuWidget::AddHomeButton() {
	UITouchButton::onButtonStateChangedCallBack callBack;

	m_home = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 50, 40, UIComponent::ANCHOR_TOP_LEFT, "home");
	m_home->setDropShadowColor(dropShadowColor);
	m_home->setX(50);

	callBack.bind(this, &ChallengeMenuWidget::OnHomePressed);
	m_home->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);

	m_parentComponent->addChild(m_home);
}

void ChallengeMenuWidget::AddChallengeTitle() {
	m_challengeTitle = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "Challenge Mode");
    m_challengeTitle->setDropShadowColor(dropShadowColor);
	m_parentComponent->addChild(m_challengeTitle);
}

void ChallengeMenuWidget::AddLeaderboardTitle() {
	m_leaderboardTitle = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "Challenge Leaderboard");
    m_leaderboardTitle->setDropShadowColor(dropShadowColor);
	m_leaderboard->addChild(m_leaderboardTitle);
}

void ChallengeMenuWidget::AddLeaderboardFrame() {
	m_leaderboard = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
	m_leaderboard->setAnchor(UIComponent::ANCHOR_CENTER);
	m_leaderboard->setWidth(600);
	m_leaderboard->setHeight(460);

	m_parentComponent->addChild(m_leaderboard);
}

void ChallengeMenuWidget::AddLeaderboardContents() {
	m_loadingLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "loading...");
    m_loadingLabel->setDropShadowColor(dropShadowColor);
	m_loadingLabel->setX(5.0f);
	m_loadingLabel->setY(190);

	m_leaderboard->addChild(m_loadingLabel);
}

void ChallengeMenuWidget::SetLeaderboardContents(const ChallengeLeaderboard& leaderboard) {
	if (m_loadingLabel != nullptr) {
		m_loadingLabel->release();
	}

	const int LEADERBOARD_ENTRY_PADDING = 25; //this is used to get the entries under the leaderboard title.

	UILabel* label = nullptr;
	UIUsernameLabel* usernameLabel = nullptr;
	for (int i = 0; i < leaderboard.Size(); i++) {
		double calculatedY = 30 * (i + 1);

		label = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0/6, 90.0, UIComponent::ANCHOR_TOP_CENTER, std::to_string(i + 1));
        label->setDropShadowColor(dropShadowColor);
		label->setY(LEADERBOARD_ENTRY_PADDING + calculatedY);
		label->setX(-160);
        if (IsWaitingForLeaderboard() == false) {
            return;
        }
		m_leaderboard->addChild(label);

		label = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 200.0, 40.0, UIComponent::ANCHOR_TOP_CENTER, leaderboard.GetEntryInPlace(i).GetUser().GetUsername());
        label->setDropShadowColor(dropShadowColor);
		label->setY(LEADERBOARD_ENTRY_PADDING + 24 + calculatedY);

		usernameLabel = new UIUsernameLabel(leaderboard.GetEntryInPlace(i).GetUser(), label);
		//usernameLabel->RegisterUserProfileDisplayListener(this);
		m_leaderboardEntries.push_back(label);
		//m_leaderboardClickListeners.push_back(usernameLabel);

        if (IsWaitingForLeaderboard() == false) {
            return;
        }
		m_leaderboard->addChild(label);

		label = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0 / 6, 90.0, UIComponent::ANCHOR_TOP_CENTER, std::to_string(leaderboard.GetEntryInPlace(i).GetPoints()));
        label->setDropShadowColor(dropShadowColor);
		label->setY(LEADERBOARD_ENTRY_PADDING + calculatedY);
		label->setX(160);

        if (IsWaitingForLeaderboard() == false) {
            return;
        }
		m_leaderboard->addChild(label);
	}
}

void ChallengeMenuWidget::SetLeaderboardContentsLoadFailed() {
	m_loadingLabel->release();

	m_loadingLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 585.0, 90.0, UIComponent::ANCHOR_TOP_CENTER, "leaderboard currently unavailable :(");
    m_loadingLabel->setDropShadowColor(dropShadowColor);
	m_loadingLabel->setX(5.0f);
	m_loadingLabel->setY(190);

	m_leaderboard->addChild(m_loadingLabel);
}

void ChallengeMenuWidget::AddNewGameButton() {
	UITouchButton::onButtonStateChangedCallBack callBack;

	m_newGameButton = UIComponentFactory::getInstance()->createUILabel("KYCQuestionButtonArchetype", 585.0, 90.0, UIComponent::ANCHOR_BOTTOM_CENTER	, "New Game");
    m_newGameButton->setDropShadowColor(dropShadowColor);
	m_newGameButton->setY(40);

	if (IEngine::getEngine()->GetUserProvider()->IsLoggedIn() == false) {
		m_newGameButton->DisableButton();
	}

	callBack.bind(this, &ChallengeMenuWidget::OnNewGamePressed);
	m_newGameButton->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
	m_parentComponent->addChild(m_newGameButton);
}

void ChallengeMenuWidget::OnHomePressed(UITouchButton::ButtonState state) {
	std::list<onChallengeMenuItemSelectedCallBack>::iterator it = m_onChallengeMenuItemSelectedListeners.begin();
	while (it != m_onChallengeMenuItemSelectedListeners.end()) {
		(*it)(LOAD_MAIN_MENU);
		it++;
	}
}

void ChallengeMenuWidget::OnNewGamePressed(UITouchButton::ButtonState state) {
	std::list<onChallengeMenuItemSelectedCallBack>::iterator it = m_onChallengeMenuItemSelectedListeners.begin();
	while (it != m_onChallengeMenuItemSelectedListeners.end()) {
		(*it)(LOAD_CHALLENGE_LEVEL);
		it++;
	}
}
