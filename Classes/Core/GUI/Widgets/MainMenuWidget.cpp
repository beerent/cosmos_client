#include "Core/GUI/Widgets/MainMenuWidget.h"
#include "Core/GUI/Components/UIComponentFactory.h"
#include "Core/Debug/DebugLogic.h"

#include <Core/User/UserProvider.h>

float labelYPosition = 100.0;
float labelXPosition = 5.0f;
const float LABEL_HEIGHT = 90.0;
const float LABEL_SPACING = 0.0 + LABEL_HEIGHT;
const float LABEL_WIDTH = 585.0;

const glm::vec3 dropShadowColor(0.0f, 0.0f, 0.0f);

MainMenuWidget::MainMenuWidget(UIComponentFactory *uiComponentFactory, UIComponent *parentComponent) : m_menu(nullptr), m_username(nullptr), m_appVersion(nullptr), m_usernamePressedCallback(nullptr) {
	m_uiComponentFactory = uiComponentFactory;
	m_parentComponent = parentComponent;
    
    m_closeKeyboardCallback.bind(this, &MainMenuWidget::onInputEvent);
    InputManager::getInstance()->registerForInputEvents(m_closeKeyboardCallback);
}

MainMenuWidget::~MainMenuWidget() {
    InputManager::getInstance()->unregisterForInputEvents(m_closeKeyboardCallback);
}

void MainMenuWidget::init() {
	m_menu = m_uiComponentFactory->createUIComponent(StringManager::getIDForString("uiSGPMenuBackGroundArchetype"));
    m_menu->setAnchor(UIComponent::ANCHOR_CENTER);
    m_menu->setWidth(600);
    m_menu->setHeight(460);
    
	m_parentComponent->addChild(m_menu);

	UILabel* label = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_CENTER, "Know Your Cosmos");
    label->setDropShadowColor(dropShadowColor);
    label->setY(12.0);   
    m_menu->addChild(label);
    
    label = m_uiComponentFactory->createUILabel("KYCQuestionButtonArchetype", LABEL_WIDTH, LABEL_HEIGHT, UIComponent::ANCHOR_TOP_LEFT, "Challenge Mode");
    label->setDropShadowColor(dropShadowColor);
    label->setX(labelXPosition);
    label->setY(labelYPosition + LABEL_SPACING);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
	callBack.bind(this, &MainMenuWidget::OnLoadChallengeMenu);
	label->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_menu->addChild(label);

    DisplayUsername();
    DisplayAppVersion();
}

void MainMenuWidget::onInputEvent(InputManager::InputEvent event, InputManager::InputEventData data) {
    if (IEngine::getEngine()->GetKeyboardManager()->KeyboardIsActive()) {
        IEngine::getEngine()->GetKeyboardManager()->OnEnterPressed();
    }
}

void MainMenuWidget::DisplayAppVersion() {
    std::string appVersion = IEngine::getEngine()->GetDeviceUtil()->GetBuildVersion();
    float offset = appVersion.size() * 12.5;
    m_appVersion = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", offset, 10, UIComponent::ANCHOR_BOTTOM_RIGHT, appVersion);
    m_appVersion->setDropShadowColor(dropShadowColor);
    m_appVersion->setX(80);
    m_parentComponent->addChild(m_appVersion);
}

void MainMenuWidget::TakeDownAppVersion() {
    m_appVersion->release();
    delete m_appVersion;
    m_appVersion = nullptr;
}

void MainMenuWidget::OnLoadChallengeMenu(UITouchButton::ButtonState state) {
    std::list<onMenuItemSelectedCallBack>::iterator it = m_onMenuItemSelectedListeners.begin();
    while(it != m_onMenuItemSelectedListeners.end()) {
        (*it)(LOAD_CHALLENGE_MENU_LEVEL);
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
    m_appVersion->release();
    
    m_menu->release();
    TakeDownAppVersion();
}

void MainMenuWidget::DisplayUsername() {
    if (m_username != nullptr) {
        m_username->release();
    }
    
    std::string username = IEngine::getEngine()->GetUserProvider()->GetUser().GetUsername();
    
    float displaySize = 12.5 * username.size();

    m_username = m_uiComponentFactory->createUILabel("KYCHeaderLabelArchetype", displaySize, 40, UIComponent::ANCHOR_TOP_RIGHT, username);
    m_username->setDropShadowColor(dropShadowColor);
    m_username->setX(80);
    
    UITouchButton::onButtonStateChangedCallBack callBack;
    callBack.bind(this, &MainMenuWidget::OnUsernamePressed);
    m_username->registerForButtonEvent(UITouchButton::DEPRESSED, callBack);
    
    m_parentComponent->addChild(m_username);
}

void MainMenuWidget::SetVisible(bool visible) {
    if (visible) {
        DisplayAppVersion();
        DisplayUsername();
    } else {
        m_username->setTextString("");
        m_appVersion->setTextString("");
    }
    
    m_menu->setVisible(visible);
}
