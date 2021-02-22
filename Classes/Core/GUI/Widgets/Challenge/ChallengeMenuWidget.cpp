#include <Core/GUI/Widgets/Challenge/ChallengeMenuWidget.h>
#include <Core/GUI/Components/UIComponentFactory.h>
#include <IEngine.h>

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

ChallengeMenuWidget::ChallengeMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : IUserProfileDisplayListener(uiComponentFactory, parentComponent),
      m_home(nullptr), m_leaderboard(nullptr), m_newGameButton(nullptr), m_currentUsername(nullptr), m_waitingForLeaderboard(false), m_loadingLabel(nullptr), m_maxPointsSize(0) {
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
    AddEmptyLeaderboard();
	AddLeaderboardLoading();
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
    m_currentUsername = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameWidth, 60, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_currentUsername->setDropShadowColor(dropShadowColor);
    m_currentUsername->setX(80);
    m_parentComponent->addChild(m_currentUsername);
}

void ChallengeMenuWidget::AddHomeButton() {
	UITouchButton::onButtonStateChangedCallBack callBack;

	m_home = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 150, 60, UIComponent::ANCHOR_TOP_LEFT, "        home        ");
	m_home->setDropShadowColor(dropShadowColor);
	m_home->setX(30);

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
    m_leaderboardTitle->setY(12.0);
	m_leaderboard->addChild(m_leaderboardTitle);
}

void ChallengeMenuWidget::AddLeaderboardFrame() {
	m_leaderboard = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
	m_leaderboard->setAnchor(UIComponent::ANCHOR_CENTER);
	m_leaderboard->setWidth(600);
	m_leaderboard->setHeight(460);

	m_parentComponent->addChild(m_leaderboard);
}

void ChallengeMenuWidget::AddLeaderboardLoading() {
	m_loadingLabel = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 0.0, 0.0, UIComponent::ANCHOR_CENTER, "loading...");
    m_loadingLabel->setDropShadowColor(dropShadowColor);

	m_leaderboard->addChild(m_loadingLabel);
}

void ChallengeMenuWidget::TakeDownLeaderboardLoading() {
    if (m_loadingLabel != nullptr) {
        std::string emptyString("");
        m_loadingLabel->setTextString(emptyString);
        m_loadingLabel->release();
    }
}

void ChallengeMenuWidget::AddEmptyLeaderboard() {
    const std::string text = "";
    float textWidth = 12.5 * text.size();
    
    const int basePadding = 29;
    const int rowPadding = 32;
    
    m_leaderboardEntry0 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry0->setDropShadowColor(dropShadowColor); m_leaderboardEntry0->setY(basePadding + ( rowPadding * (1 + 0))); m_leaderboard->addChild(m_leaderboardEntry0);
    
    m_leaderboardEntry1 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry1->setDropShadowColor(dropShadowColor); m_leaderboardEntry1->setY(basePadding + ( rowPadding * (1 + 1))); m_leaderboard->addChild(m_leaderboardEntry1);
    
    m_leaderboardEntry2 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry2->setDropShadowColor(dropShadowColor); m_leaderboardEntry2->setY(basePadding + ( rowPadding * (1 + 2))); m_leaderboard->addChild(m_leaderboardEntry2);
    
    m_leaderboardEntry3 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry3->setDropShadowColor(dropShadowColor); m_leaderboardEntry3->setY(basePadding + ( rowPadding * (1 + 3))); m_leaderboard->addChild(m_leaderboardEntry3);
    
    m_leaderboardEntry4 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry4->setDropShadowColor(dropShadowColor); m_leaderboardEntry4->setY(basePadding + ( rowPadding * (1 + 4))); m_leaderboard->addChild(m_leaderboardEntry4);
    
    m_leaderboardEntry5 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry5->setDropShadowColor(dropShadowColor); m_leaderboardEntry5->setY(basePadding + ( rowPadding * (1 + 5))); m_leaderboard->addChild(m_leaderboardEntry5);
    
    m_leaderboardEntry6 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry6->setDropShadowColor(dropShadowColor); m_leaderboardEntry6->setY(basePadding + ( rowPadding * (1 + 6))); m_leaderboard->addChild(m_leaderboardEntry6);
    
    m_leaderboardEntry7 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry7->setDropShadowColor(dropShadowColor); m_leaderboardEntry7->setY(basePadding + ( rowPadding * (1 + 7))); m_leaderboard->addChild(m_leaderboardEntry7);
    
    m_leaderboardEntry8 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry8->setDropShadowColor(dropShadowColor); m_leaderboardEntry8->setY(basePadding + ( rowPadding * (1 + 8))); m_leaderboard->addChild(m_leaderboardEntry8);
    
    m_leaderboardEntry9 = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", textWidth, 60, UIComponent::ANCHOR_TOP_CENTER, text);
    m_leaderboardEntry9->setDropShadowColor(dropShadowColor); m_leaderboardEntry9->setY(basePadding + ( rowPadding * (1 + 9))); m_leaderboard->addChild(m_leaderboardEntry9);
}

void ChallengeMenuWidget::SetLeaderboardContents(const ChallengeLeaderboard& leaderboard) {
    TakeDownLeaderboardLoading();

	for (int i = 0; i < leaderboard.Size(); i++) {
        const ChallengeLeaderboardEntry challengeLeaderboardEntry = leaderboard.GetEntryInPlace(i);
        SetLeaderboardEntry(challengeLeaderboardEntry, i);
	}
}

void ChallengeMenuWidget::SetLeaderboardEntry(const ChallengeLeaderboardEntry& challengeLeaderboardEntry, int position) {
    const std::string leaderboardString = ChallengeLeaderboardEntryToString(challengeLeaderboardEntry, position);
    SetLeaderboardString(leaderboardString, position);
}

void ChallengeMenuWidget::SetLeaderboardString(const std::string& leaderboardString, int position) {
    switch (position) {
        case 0: m_leaderboardEntry0->setTextString(leaderboardString); break;
        case 1: m_leaderboardEntry1->setTextString(leaderboardString); break;
        case 2: m_leaderboardEntry2->setTextString(leaderboardString); break;
        case 3: m_leaderboardEntry3->setTextString(leaderboardString); break;
        case 4: m_leaderboardEntry4->setTextString(leaderboardString); break;
        case 5: m_leaderboardEntry5->setTextString(leaderboardString); break;
        case 6: m_leaderboardEntry6->setTextString(leaderboardString); break;
        case 7: m_leaderboardEntry7->setTextString(leaderboardString); break;
        case 8: m_leaderboardEntry8->setTextString(leaderboardString); break;
        case 9: m_leaderboardEntry9->setTextString(leaderboardString); break;
        
    }
}

std::string ChallengeMenuWidget::ChallengeLeaderboardEntryToString(const ChallengeLeaderboardEntry& challengeLeaderboardEntry, int position) {
    std::string leaderboardString = "";
    std::string place = std::to_string(position + 1);
    while (place.length() < 5) {
        place += " ";
    }
    leaderboardString += place;
    
    std::string username = challengeLeaderboardEntry.GetUser().GetUsername();
    while (username.length() < 20) {
        username += " ";
    }
    leaderboardString += username;
    
    std::string points = std::to_string(challengeLeaderboardEntry.GetPoints());
    if (position == 0) {
        m_maxPointsSize = (int) points.length();
    } else {
        while (points.length() != m_maxPointsSize) {
            points = " " + points;
        }
    }
    
    while (points.length() < 4) {
        points  += " ";
    }
    leaderboardString += points;
    
    return leaderboardString;
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
