#include "Core/GUI/Widgets/MainMenuWidget.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/Debug/DebugLogic.h"
#include <Core/User/UserProvider.h>
#include <cmath>

float labelYPosition = 40.0;
float labelXPosition = 5.0f;
const float LABEL_HEIGHT = 90.0;
const float LABEL_SPACING = 10.0 + LABEL_HEIGHT;
const float LABEL_WIDTH = 585.0;

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);
const glm::vec3 YELLOW_TEXT_COLOR(255.0f, 255.0f, 0.0f);


MainMenuWidget::MainMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : m_menu(nullptr), m_title(nullptr), m_username(nullptr), m_usernamePrefix(nullptr),
      m_appVersion(nullptr), m_usernamePressedCallback(nullptr), m_challengeMode(nullptr), m_cosmosLive(nullptr), m_message(nullptr), m_appVersionPressCount(0), m_usingRainbowMessageColor(false) {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
}

MainMenuWidget::~MainMenuWidget() {}

void MainMenuWidget::init() {
	m_menu = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_menu->setAnchor(UIComponent::ANCHOR_CENTER);
    m_menu->setWidth(600);
    m_menu->setHeight(460);
    
	m_parentComponent->addChild(m_menu);

	m_title = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Know Your Cosmos");
    m_title->setDropShadowColor(dropShadowColor);
    m_title->setY(12.0);
    m_menu->addChild(m_title);
    
    DisplayChallengeModeButton();
    DisplayCosmosLiveButton();

    DisplayUsername();
    DisplayAppVersion();
}

void MainMenuWidget::DisplayChallengeModeButton() {
    m_challengeMode = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_LEFT, "Cosmos Quiz");
    m_challengeMode->setDropShadowColor(dropShadowColor);
    m_challengeMode->setX(labelXPosition);
    m_challengeMode->setY(labelYPosition + LABEL_SPACING);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &MainMenuWidget::OnLoadChallengeMenu);
    m_challengeMode->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_menu->addChild(m_challengeMode);
}

void MainMenuWidget::DisplayCosmosLiveButton() {
    m_cosmosLive = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_LEFT, "Cosmic Chat");
    m_cosmosLive->setDropShadowColor(dropShadowColor);
    m_cosmosLive->setX(labelXPosition);
    m_cosmosLive->setY(labelYPosition + LABEL_SPACING * 2);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &MainMenuWidget::OnLoadCosmosLiveLobby);
    m_cosmosLive->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_menu->addChild(m_cosmosLive);
}

void MainMenuWidget::DisplayAppVersion() {
    const std::string appVersion = IEngine::getEngine()->GetDeviceUtil()->GetBuildVersion();
    float offset = appVersion.size() * 12.5;
    m_appVersion = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 100, 100, UIComponent::ANCHOR_BOTTOM_RIGHT, appVersion);
    m_appVersion->setDropShadowColor(dropShadowColor);
    m_appVersion->setX(45);
    m_appVersion->setY(-40);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &MainMenuWidget::OnAppVersionPressed);
    m_appVersion->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_parentComponent->addChild(m_appVersion);
}

void MainMenuWidget::TakeDownAppVersion() {
    m_appVersion->release();
    delete m_appVersion;
    m_appVersion = nullptr;
}

void MainMenuWidget::OnAppVersionPressed(UITouchButton::ButtonState state) {
    UpdateAppTitleTextColor();
    m_appVersionPressCount++;
}

int MainMenuWidget::GetAppVersionPressCount() const {
    return m_appVersionPressCount;
}

void MainMenuWidget::ActivateRainbowMessageColor() {
    m_usingRainbowMessageColor = true;
}

void MainMenuWidget::UpdateAppTitleTextColor() {
    UpdateLabelColor(m_appVersionPressCount, m_title);
}

void MainMenuWidget::UpdateLabelColor(int colorOffset, UILabel* labelComponent) {
    glm::vec3 color;
    
    int switchKey = abs(colorOffset) % 7;
    switch (switchKey) {
        case 0:
            color = glm::vec3(0, 1, 1);
            break;
            
        case 1:
            color = glm::vec3(1, 0, 0);
            break;
            
        case 2:
            color = glm::vec3(0, 1, 0);
            break;
            
        case 3:
            color = glm::vec3(0, 0, 1);
            break;
            
        case 4:
            color = glm::vec3(1, 1, 0);
            break;
            
        case 5:
            color = glm::vec3(1, 0, 1);
            break;
            
        case 6:
            color = glm::vec3(1, 1, 1);
            break;

    }
    
    labelComponent->setColor(color);
}

void MainMenuWidget::OnLoadChallengeMenu(UITouchButton::ButtonState state) {
    std::list<onMenuItemSelectedCallBack>::iterator it = m_onMenuItemSelectedListeners.begin();
    while(it != m_onMenuItemSelectedListeners.end()) {
        (*it)(LOAD_CHALLENGE_MENU_LEVEL);
        it++;
    }
}

void MainMenuWidget::OnLoadCosmosLiveLobby(UITouchButton::ButtonState state) {
    std::list<onMenuItemSelectedCallBack>::iterator it = m_onMenuItemSelectedListeners.begin();
    while(it != m_onMenuItemSelectedListeners.end()) {
        (*it)(LOAD_COSMOS_LIVE_LEVEL);
        it++;
    }
}

void MainMenuWidget::registerUsernamePressedCallback(UITouchButton::onButtonStateChangedCallBack usernamePressedCallback) {
    m_usernamePressedCallback = usernamePressedCallback;
}

void MainMenuWidget::OnUsernamePressed(UITouchButton::ButtonState state) {
    if (m_usernamePressedCallback != nullptr) {
        m_usernamePressedCallback(state);
    }
}

void MainMenuWidget::registerForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack) {
    m_onMenuItemSelectedListeners.push_back(callBack);
}

void MainMenuWidget::unregisterForMenuItemSelectedEvent(onMenuItemSelectedCallBack callBack) {
    m_onMenuItemSelectedListeners.remove(callBack);
}

void MainMenuWidget::release() {
    m_username->release();
    m_usernamePrefix->release();
    m_appVersion->release();
    
    m_menu->release();
    TakeDownAppVersion();
    TakeDownMessage();
}

void MainMenuWidget::DisplayUsername() {
    if (m_username != nullptr) {
        m_username->release();
    }
    
    if (m_usernamePrefix != nullptr) {
        m_usernamePrefix->release();
    }
    
    std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    std::string usernamePrefix = "username:   ";
   
    float usernameDisplaySize = 12.5 * username.size();
    float usernamePrefixDisplaySize = 12.5 * usernamePrefix.size();

    m_username = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernameDisplaySize, 60, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_username->setDropShadowColor(dropShadowColor);
    m_username->setX(80);
    
    m_usernamePrefix = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", usernamePrefixDisplaySize, 60, UIComponent::ANCHOR_TOP_RIGHT, usernamePrefix);
    m_usernamePrefix->setDropShadowColor(dropShadowColor);
    m_usernamePrefix->setX(80 + usernameDisplaySize);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &MainMenuWidget::OnUsernamePressed);
    m_username->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    m_usernamePrefix->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_parentComponent->addChild(m_username);
    m_parentComponent->addChild(m_usernamePrefix);
}



void MainMenuWidget::UpdateMessage(const std::string& message, int offset) {
    if (m_message != nullptr) {
        m_message->setTextString("");
        m_message->release();
        m_message = nullptr;
    }
    
    m_message = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", 1, 1, UIComponent::ANCHOR_BOTTOM_RIGHT, message);
    m_message->setDropShadowColor(dropShadowColor);
    
    if (!m_usingRainbowMessageColor) {
        m_message->setColor(YELLOW_TEXT_COLOR);
    }else {
        UpdateLabelColor(offset, m_message);
    }
    
    m_message->setY(60);
    m_message->setX(offset);
    m_parentComponent->addChild(m_message);
}

void MainMenuWidget::TakeDownMessage() {
    if (m_message != nullptr) {
        m_message->release();
    }
}

void MainMenuWidget::SetVisible(bool visible) {
    if (visible) {
        DisplayAppVersion();
        DisplayUsername();
    } else {
        m_username->setTextString("");
        m_usernamePrefix->setTextString("");
        m_appVersion->setTextString("");
    }
    
    m_menu->setVisible(visible);
}
